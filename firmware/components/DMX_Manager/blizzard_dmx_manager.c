/* Copyright (C) Blizzard Lighting LLC. - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Christian Krueger <christian@blizzardlighting.com>, January 2018
 *
 * Protocol Manager -> blizzard_dmx_manager.c
 * 
 * For this product, dmx will only be outputting. This files sets up the 
 * DMX serial communications and lets the driver in modified driver go to work.
 */

#include "blizzard_dmx_manager.h"

static const char *Tag = "DMX";

volatile uint8_t Dmx[MAX_DMX_SLOTS];
volatile uint8_t ValidFrameFlag = DMX_NO_FRAME;
volatile uint8_t ValidFrameCount = 0;

/*---------------------------- Initializers ----------------------------------*/
/*
 * Function: init_dmx_manager
 * ----------------------------
 *  configures the GPIOs associated with DMX and well as installs
 *  the custom driver -uart.c in ./modified-drivers
 *
 *  returns: DMX_GPIO_ERROR DMX_UART_ERROR DMX_SET_PIN_ERROR
 * 	DMX_INSTALL_DRIVER_ERROR DMX_TX_IDLE_ERROR SUCCESS
 */ 
uint8_t init_dmx_manager() {
	esp_err_t retVal;

	uart_config_t uartConfig = {
		.baud_rate = DMX_DATA_BAUD,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_2,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE //do not use rts cts
	};

	//GPIO setup
	//This sets the direction for the MAX 485 chip
	//DIRECTION_PIN should be wired directly to
	//RE and DE on the MAX485 chip
	gpio_config_t ioConfig = {
		.intr_type = GPIO_PIN_INTR_DISABLE,
		.mode = GPIO_MODE_OUTPUT,
		.pin_bit_mask = (1 << DIRECTION_PIN),
		.pull_down_en = DISABLE,
		.pull_up_en = DISABLE
	};

	ESP_LOGI(Tag, "DMX is getting ready for 4th quarter...");

	blackout();

	resetDMXDriver();

	retVal = gpio_config(&ioConfig);
	if(retVal != ESP_OK){
		ESP_LOGE(Tag, "Error configuring gpio - dmx init: %d", retVal);
		return DMX_GPIO_ERROR;
	}


	retVal = uart_param_config(DMX_UART, &uartConfig);
	if(retVal != ESP_OK){
		ESP_LOGE(Tag, "Error configuring uart - dmx init: %d", retVal);
		return DMX_UART_ERROR;
	}

	retVal = uart_set_pin(DMX_UART, DMX_TX_PIN, DMX_RX_PIN,
							UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	if(retVal != ESP_OK){
		ESP_LOGE(Tag, "Error setting gpio pin - dmx init: %d", retVal);
		return DMX_SET_PIN_ERROR;
	}

	retVal = uart_driver_install(DMX_UART, MAX_DMX_SLOTS, MAX_DMX_SLOTS, 0, NULL, ESP_INTR_FLAG_LOWMED);
	if(retVal != ESP_OK){
		ESP_LOGE(Tag, "Error installing uart driver - dmx init: %d", retVal);
		return DMX_INSTALL_DRIVER_ERROR;
	}

	retVal = uart_set_tx_idle_num(DMX_UART, DMX_MAB_SIZE);
	if(retVal != ESP_OK){
		ESP_LOGE(Tag, "Error setting tx idle number: %d", retVal);
		return DMX_TX_IDLE_ERROR;
	}

	ESP_LOGI(Tag, "DMX is jumping around!");
	return SUCCESS;
}

/*---------------------------- Controllers ----------------------------------*/
/*
 * Function: start_dmx(uint8_t direction)
 * ----------------------------
 *  Stops the DMX driver and then re-initializes it depending on the direction
 *
 *  Direction: SEND | RECEIVE
 * 
 *  returns: DMX_SET_INTERRUPT_ERROR DMX_SET_PIN_ERROR DMX_DIRECTION_ERROR SUCCESS
 */
uint8_t start_dmx(uint8_t direction) {
	esp_err_t retVal;

	if(stop_dmx() != SUCCESS){
		ESP_LOGE(Tag, "Error stopping dmx - start dmx");
		return DMX_SET_INTERRUPT_ERROR;
	}

	retVal = gpio_set_level(DIRECTION_PIN, direction);
	if(retVal != ESP_OK){
		ESP_LOGE(Tag, "Error setting GPIO level - start dmx: %d", retVal);
		return DMX_SET_PIN_ERROR;
	}

	switch (direction) {
		case SEND:
			retVal = uart_enable_tx_intr(DMX_UART, ENABLE, DMX_EMPTY_THRESH);
			if(retVal != ESP_OK){
				ESP_LOGE(Tag, "Error enabling tx interrpts - start dmx: %d", retVal);
				return DMX_SET_INTERRUPT_ERROR;
			}
			ESP_LOGI(Tag, "DMX sending");
			break;
		case RECEIVE:
			ValidFrameFlag = DMX_NO_FRAME;
			ValidFrameCount = 0;
			retVal = uart_enable_intr_mask(DMX_UART, DMX_RECEIVE_INTERRUPT_MASK);
			if(retVal != ESP_OK){
				ESP_LOGE(Tag, "Error enabling interrpt mask - start dmx: %d", retVal);
				return DMX_SET_INTERRUPT_ERROR;
			}
			ESP_LOGI(Tag, "DMX receiving");
			break;
		default:
			ESP_LOGI(Tag, "Invalid DMX direction");
			return DMX_DIRECTION_ERROR;
	}

	return SUCCESS;

}

/*
 * Function: stop_dmx()
 * ----------------------------
 *  Stops the DMX driver - really it just clears the interrupts
 *
 *  Direction: SEND | RECEIVE
 *
 *  returns: DMX_SET_INTERRUPT_ERROR SUCCESS
 */
uint8_t stop_dmx() {
	esp_err_t retVal;

	retVal = uart_disable_intr_mask(DMX_UART, DMX_ALL_INTERRUPT_MASK);
	if(retVal != ESP_OK){
		ESP_LOGE(Tag, "Error disabling interrupt mask - stop dmx: %d", retVal);
		return DMX_SET_INTERRUPT_ERROR;
	}

	resetDMXDriver();
	return SUCCESS;
}

/*
 * Function: uint8_t read_dmx_sync(uint16_t timeout)
 * ----------------------------
 *  Reads in a dmx frame or times out
 *
 *
 *  returns: dmx array
 */
uint8_t read_dmx_sync(uint32_t timeout){
	uint8_t retVal;

	retVal = uart_enable_intr_mask(DMX_UART, DMX_RECEIVE_INTERRUPT_MASK);
	if(retVal != ESP_OK){
		ESP_LOGE(Tag, "Error enabling interrpt mask - start dmx: %d", retVal);
		return DMX_SET_INTERRUPT_ERROR;
	}

	while(ValidFrameFlag == DMX_NO_FRAME && timeout--){
		vTaskDelay(1);
	}

	retVal = (ValidFrameFlag == DMX_VALID_FRAME) ? SUCCESS : DMX_NO_RX_ERROR;

	stop_dmx();

	return retVal;
}

/*
 * Function: copy_to_dmx(uint8_t* temp_dmx)
 * ----------------------------
 *  copies passed in array to the main dmx array
 *
 *
 *  returns: DMX_NULL_ARRAY_ERROR SUCCESS
 */
uint8_t copy_to_dmx(uint8_t* temp_dmx){
	uint16_t i;

	if(temp_dmx == NULL){
		ESP_LOGE(Tag, "Null temp dmx array - copy to dmx");
		return DMX_NULL_ARRAY_ERROR;
	}

	for(i = 1; i < MAX_DMX_SLOTS; i++){
		Dmx[i] = temp_dmx[i - 1];
	}

	return SUCCESS;
}

/*
 * Function: copy_to_dmx_full(uint8_t* temp_dmx)
 * ----------------------------
 *  copies passed in array to the main dmx array
 *
 *
 *  returns: DMX_NULL_ARRAY_ERROR SUCCESS
 */
uint8_t copy_to_dmx_full(uint8_t* temp_dmx){
	uint16_t i;

	if(temp_dmx == NULL){
		ESP_LOGE(Tag, "Null temp dmx array - copy to dmx full");
		return DMX_NULL_ARRAY_ERROR;
	}

	for(i = 0; i < MAX_DMX_SLOTS; i++){
		Dmx[i] = temp_dmx[i];
	}

	return SUCCESS;
}

/*
 * Function: copy_from_dmx(uint8_t* temp_dmx)
 * ----------------------------
 *  copies main dmx to passed in array

 *  returns: DMX_NULL_ARRAY_ERROR SUCCESS
 */
uint8_t copy_from_dmx(uint8_t* temp_dmx){
	uint16_t i;

	if(temp_dmx == NULL){
		ESP_LOGE(Tag, "Null temp dmx array - copy from dmx");
		return DMX_NULL_ARRAY_ERROR;
	}

	for(i = 1; i < MAX_DMX_SLOTS; i++){
		temp_dmx[i - 1] = Dmx[i];
	}

	return SUCCESS;
}

/*
 * Function: blackout()
 * ----------------------------
 *  clears dmx buffer
 * 
 *  returns: void
 */
void blackout(){
	uint16_t i;

	for(i = 0; i < MAX_DMX_SLOTS; i++){
		Dmx[i] = 0;
	}	
}