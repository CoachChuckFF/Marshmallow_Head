/* Copyright (C) Blizzard Lighting LLC. - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Christian Krueger <christian@blizzardlighting.com>, January 2018
 *
 * Timer Manager -> blizzard_timer_manager.c
 *
 */

#include "blizzard_pixel_manager.h"
#include "invalid_pixels.h"

#define PIXEL_COUNT (CONFIG_PIXEL_COLS * CONFIG_PIXEL_ROWS - INVALID_PIXEL_COUNT)

static const char *Tag = "PIXEL MANAGER";

CRGB Leds[PIXEL_COUNT];

/*
 * Function: init_pixel_manager()
 * ----------------------------
 *  Inits the pixel manager
 *
 *  returns: SUCCESS
 */
uint8_t init_pixel_manager()
{

  ESP_LOGI(Tag, "Blood makes the grass grow");

  FastLED.addLeds<WS2812, PIXEL_DATA_PIN, RGB>(Leds, PIXEL_COUNT);

  FastLED.setBrightness(CONFIG_PIXEL_BRIGHTNESS);

  ESP_LOGI(Tag, "Kill Kill Kill");

  return SUCCESS;
}

void paint_pixels(){
  FastLED.show();
}

void set_pixel(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue, uint8_t mode){
  CRGB color;

  if(pixel >= PIXEL_COUNT)
    return;

  switch(mode){
    case PIXEL_SET:
      Leds[pixel][RED_INDEX] = red;
      Leds[pixel][GREEN_INDEX] = green;
      Leds[pixel][BLUE_INDEX] = blue;

      break;
    case PIXEL_ADD:
      color[RED_INDEX] = red;
      color[GREEN_INDEX] = green;
      color[BLUE_INDEX] = blue;

      Leds[pixel] += color;
      break;
    case PIXEL_SUB:
      color[RED_INDEX] = red;
      color[GREEN_INDEX] = green;
      color[BLUE_INDEX] = blue;

      Leds[pixel] -= color;
      break;
    case PIXEL_ALPHA:
      if(!(red == 0 && green == 0 && blue ==0)){
        Leds[pixel][RED_INDEX] = red;
        Leds[pixel][GREEN_INDEX] = green;
        Leds[pixel][BLUE_INDEX] = blue;
      }
      break;
    default:
      Leds[pixel][RED_INDEX] = 0;
      Leds[pixel][GREEN_INDEX] = 0;
      Leds[pixel][BLUE_INDEX] = 0;
  }
}

void set_pixel_x_y(uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue, uint8_t mode){
  uint16_t index, sub_pixels = 0;

  if(x >= CONFIG_PIXEL_COLS){
    x %= CONFIG_PIXEL_COLS;
  }

  if(y >= CONFIG_PIXEL_ROWS){
    y %= CONFIG_PIXEL_ROWS;
  }

    index = y*CONFIG_PIXEL_COLS + x;
  for(int i = 0; i < INVALID_PIXEL_COUNT; i++){
    if(index < InvalidPixels[i]){
      break;
    } else if(index == InvalidPixels[i]){
      return;
    }
    sub_pixels++;
  }

  set_pixel((index - sub_pixels), red, green, blue, mode);
}

void set_all_pixels(uint8_t red, uint8_t green, uint8_t blue){
  for(int i = 0; i < PIXEL_COUNT; i++){
    Leds[i][RED_INDEX] = red;
    Leds[i][GREEN_INDEX] = green;
    Leds[i][BLUE_INDEX] = blue;
  }
}

void set_pixel_row(uint32_t y, uint8_t red, uint8_t green, uint8_t blue, uint8_t mode){
  for(uint16_t x = 0; x < CONFIG_PIXEL_COLS; x++){
    set_pixel_x_y(x, y, red, green, blue, mode);
  }
}

void set_pixel_col(uint32_t x, uint8_t red, uint8_t green, uint8_t blue, uint8_t mode){
  for(uint16_t y = 0; y < CONFIG_PIXEL_ROWS; y++){
    set_pixel_x_y(x, y, red, green, blue, mode);
  }
}

void clear_pixels(){
  set_all_pixels(0,0,0);
}

CRGB get_rainbow_color(uint8_t index){

  CRGB color;
  if (85 > index) {
    color.r=0;
    color.g=index * 3;
    color.b=(255 - index * 3);
  } 
  else if (170 > index) {
    index -= 85;
    color.r=index * 3;
    color.g=(255 - index * 3);
    color.b=0;
  }
  else {
    index -= 170;
    color.r=(255 - index * 3);
    color.g=0;
    color.b=index * 3;
  }

  return color;
}

void paint_sprite(uint16_t length, uint8_t frame[][CONFIG_PIXEL_ROWS][3], uint32_t x_translation, uint32_t y_translation, bool flipped){
  
  if(length > CONFIG_PIXEL_COLS){
    return;
  }

  for(uint32_t y = 0; y < CONFIG_PIXEL_ROWS; y++){
    for(uint32_t x = 0; x < length; x++){
      int xa = (!flipped) ? x : (length - x - 1);
      set_pixel_x_y(x + x_translation, y + y_translation, frame[xa][y][0],frame[xa][y][1],frame[xa][y][2], PIXEL_ALPHA);
    }
  }
}
