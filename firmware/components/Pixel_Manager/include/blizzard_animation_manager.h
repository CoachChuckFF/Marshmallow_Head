/* Copyright (C) Blizzard Lighting LLC. - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Christian Krueger <christian@blizzardlighting.com>, January 2018
 *
 * Timer Manager-> blizzard_timer_manager.h
 */

#ifndef BLIZZARD_ANIMATION_MANAGER_H
#define BLIZZARD_ANIMATION_MANAGER_H

#include <stdio.h>
#include "FastLED.h"
#include "Arduino.h"

#include "blizzard_global_defines.h"
#include "blizzard_dmx_defines.h"
#include "blizzard_pixel_manager.h"

bool tick_frame(uint32_t tick_tick, uint8_t speed);

void solid(uint8_t red, uint8_t green, uint8_t blue);
void bstrobe(uint32_t tick_tick, uint8_t speed, uint8_t red, uint8_t green, uint8_t blue);
void plaid(uint32_t tick_tick, uint8_t speed, uint8_t density, uint8_t direction, uint8_t red, uint8_t green, uint8_t blue, bool cross, bool cols, bool rows);
void sparkles(uint32_t tick_tick, uint8_t speed, uint8_t saturation, uint8_t red, uint8_t green, uint8_t blue);
void rainbow(uint32_t tick_tick, uint8_t speed, uint8_t direction, uint8_t mode);

void arrows(uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y);
void danger(uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y);
void check(uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y);
void mount(uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y);
void camo(uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y);
void soc(uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y);
void blizzard(uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y);
void running_lights(uint32_t tick_tick, uint8_t speed, uint8_t red, uint8_t green, uint8_t blue);
void pac(uint32_t tick_tick, uint8_t speed, uint8_t set_frame);
void rs(uint32_t tick_tick, uint8_t speed, uint8_t set_frame);
void test(uint32_t tick_tick, uint8_t speed);
void don(uint32_t tick_tick, uint8_t speed);
void game(uint8_t x, uint8_t y);
void reset_sprites();
void forground_sprites(uint16_t length, uint8_t frame[][CONFIG_PIXEL_ROWS][3], uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y, bool reset);

#endif
