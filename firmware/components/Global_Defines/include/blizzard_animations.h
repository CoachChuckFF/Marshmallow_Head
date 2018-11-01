/* Copyright (C) Blizzard Lighting LLC. - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Christian Krueger <christian@blizzardlighting.com>, January 2018
 *
 * Global Defines-> blizzard_animations.h
 *
 */

#ifndef BLIZZARD_ANIMATIONS_H
#define BLIZZARD_ANIMATIONS_H

#ifdef __cplusplus
extern "C" {
#endif

#define PIXEL_SET       0
#define PIXEL_ADD       1
#define PIXEL_SUB       2
#define PIXEL_ALPHA     3

#define DIR_NONE        25
#define DIR_LR          50
#define DIR_RL          75
#define DIR_UD          100
#define DIR_DU          125
#define DIR_SUR         150
#define DIR_SDR         175
#define DIR_SUL         200
#define DIR_SDL         225
#define DIR_MAN         256

#define ROW_MOD_2       50
#define ROW_MOD_3       100
#define ROW_MOD_4       150
#define ROW_MOD_6       200
#define NO_ROWS         256    

#define COL_MOD_2       36
#define COL_MOD_3       72
#define COL_MOD_6       108
#define COL_MOD_9       144
#define COL_MOD_18      180
#define COL_MOD_27      216
#define NO_COLS         256

#ifdef __cplusplus
}
#endif

#endif