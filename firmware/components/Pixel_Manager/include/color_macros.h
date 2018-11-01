/* Copyright (C) Blizzard Lighting LLC. - All Rights Reserved * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * 
 * Written by Christian Krueger <christian@blizzardlighting.com>
 */

#ifndef COLOR_MACROS_H
#define COLOR_MACROS_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t Rainbow[7][3] = {
    {255,0,0},
    {255,127,0},
    {255,255,0},
    {0,255,0},
    {0,0,255},
    {75,0,130},
    {148,0,211},
};

#ifdef __cplusplus
}
#endif

#endif