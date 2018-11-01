/* Copyright (C) Blizzard Lighting LLC. - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Christian Krueger <christian@blizzardlighting.com>, February 2018
 *
 * main.cpp
 *
 */
#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "blizzard_dmx_manager.h"
#include "blizzard_pixel_manager.h"
#include "blizzard_animation_manager.h"

extern volatile uint8_t Dmx[MAX_DMX_SLOTS];

uint32_t Tick_Tick = 0;
  uint8_t red;
uint8_t green;
uint8_t blue;
uint8_t background;
uint8_t background_speed;
uint8_t background_control;
uint8_t background_alt_control;
uint8_t foreground;
uint8_t foreground_speed;
uint8_t foreground_control;
uint8_t x_control;
uint8_t y_control;

void setup()
{

  assert(init_dmx_manager() == SUCCESS);
  assert(init_pixel_manager() == SUCCESS);

}

void set_dmx_vals(){
  red = Dmx[DMX_RED];
  green = Dmx[DMX_GREEN];
  blue = Dmx[DMX_BLUE];
  background = Dmx[DMX_BACKGROUND];
  background_speed = Dmx[DMX_BACKGROUND_SPEED];
  background_control = Dmx[DMX_BACKGROUND_CONTROL];
  background_alt_control = Dmx[DMX_BACKGROUND_ALT_CONTROL];
  foreground = Dmx[DMX_FOREGROUND];
  foreground_speed = Dmx[DMX_FOREGROUND_SPEED];
  foreground_control = Dmx[DMX_FOREGROUND_CONTROL];
  x_control = Dmx[DMX_X_CONTROL];
  y_control = Dmx[DMX_Y_CONTROL];
}

void loop(){
  
  switch(read_dmx_sync(DMX_TIMEOUT)){
    case SUCCESS:
      set_dmx_vals();
      clear_pixels();
      Tick_Tick++;

      if(background < BACKGROUND_SOLID){
        solid(red, green, blue);
      } else if(background < BACKGROUND_STROBE){
        bstrobe(Tick_Tick, background_speed, red, green, blue);
      } else if(background < BACKGROUND_COLS){
        plaid(Tick_Tick, background_speed, background_control, background_alt_control, red, green, blue, true, true, false);
      } else if(background < BACKGROUND_ROWS){
        plaid(Tick_Tick, background_speed, background_control, background_alt_control, red, green, blue, true, false, true);
      } else if(background < BACKGROUND_CROSS){
        plaid(Tick_Tick, background_speed, background_control, background_alt_control, red, green, blue, true, true, true);
      } else if(background < BACKGROUND_PLAID){
        plaid(Tick_Tick, background_speed, background_control, background_alt_control, red, green, blue, false, true, true);
      } else if(background < BACKGROUND_SPARKLES){
        sparkles(Tick_Tick, background_speed, background_control, red, green, blue);
      } else if(background < BACKGROUND_RAINBOW_FADE){
        rainbow(Tick_Tick, background_speed, background_alt_control, MODE_FADE);
      } else if(background < BACKGROUND_RAINBOW_STROBE){
        rainbow(Tick_Tick, background_speed, background_alt_control, MODE_FLASH);
      }

      //specials
      if(foreground == 69 && foreground_speed == 69 && foreground_control == 69){
        don(Tick_Tick, 0);
      } else if(foreground == 0x42 && foreground_speed == 0x4C){
        test(Tick_Tick, foreground_control);
      } else if(foreground < FOREGROUND_NONE){
        //do nothing
      } else if(foreground < FOREGROUND_ARROWS){
        arrows(Tick_Tick, foreground_speed, foreground_control, true, x_control, y_control);
      } else if(foreground < FOREGROUND_DANGER){
        danger(Tick_Tick, foreground_speed, foreground_control, true, x_control, y_control);
      } else if(foreground < FOREGROUND_CHECK){
        check(Tick_Tick, foreground_speed, foreground_control, true, x_control, y_control);
      } else if(foreground < FOREGROUND_MOUNT){
        mount(Tick_Tick, foreground_speed, foreground_control, true, x_control, y_control);
      } else if(foreground < FOREGROUND_CAMO){
        camo(Tick_Tick, foreground_speed, foreground_control, true, x_control, y_control);
      } else if(foreground < FOREGROUND_SOC){
        soc(Tick_Tick, foreground_speed, foreground_control, true, x_control, y_control);
      } else if(foreground < FOREGROUND_BLIZZARD){
        blizzard(Tick_Tick, foreground_speed, foreground_control, true, x_control, y_control);
      } else if(foreground < FOREGROUND_RUN){
        running_lights(Tick_Tick, foreground_speed, red, green, blue);
      } else if(foreground < FOREGROUND_PAC){
        pac(Tick_Tick, foreground_speed, foreground_control);
      } else if(foreground <FOREGROUND_RS){
        rs(Tick_Tick, foreground_speed, foreground_control);
      } else if(foreground < FOREGROUND_GAME){
        game(x_control, y_control);
      }

      paint_pixels();
    break;
    case DMX_NO_RX_ERROR:

    break;
  }
}