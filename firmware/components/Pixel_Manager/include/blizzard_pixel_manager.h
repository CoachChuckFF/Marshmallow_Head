/* Copyright (C) Blizzard Lighting LLC. - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Christian Krueger <christian@blizzardlighting.com>, January 2018
 *
 * Timer Manager-> blizzard_timer_manager.h
 */

#ifndef BLIZZARD_PIXEL_MANAGER_H
#define BLIZZARD_PIXEL_MANAGER_H

#include <stdio.h>
#include "FastLED.h"
#include "Arduino.h"

#include "blizzard_global_defines.h"

#define PIXEL_DATA_PIN 25

#define RED_INDEX 2
#define GREEN_INDEX 1
#define BLUE_INDEX 0

#define INVALID_PIXEL_COUNT 93

uint8_t init_pixel_manager(void);

void paint_pixels(void);
void set_pixel(uint16_t pixel, uint8_t red, uint8_t green, uint8_t blue, uint8_t mode);
void set_pixel_x_y(uint32_t x, uint32_t y, uint8_t red, uint8_t green, uint8_t blue, uint8_t mode);
void set_all_pixels(uint8_t red, uint8_t green, uint8_t blue);
void set_pixel_row(uint32_t y, uint8_t red, uint8_t green, uint8_t blue, uint8_t mode);
void set_pixel_col(uint32_t x, uint8_t red, uint8_t green, uint8_t blue, uint8_t mode);
void set_pixel_x_y(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
void clear_pixels(void);


/* Intermediates */
CRGB get_rainbow_color(uint8_t index);
void paint_sprite(uint16_t length, uint8_t frame[][CONFIG_PIXEL_ROWS][3], uint32_t x_translation, uint32_t y_translation, bool flipped);


#endif
