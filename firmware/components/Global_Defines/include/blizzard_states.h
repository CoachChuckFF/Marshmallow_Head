/* Copyright (C) Blizzard Lighting LLC. - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Christian Krueger <christian@blizzardlighting.com>, January 2018
 *
 * Global Defines-> blizzard_states.h
 *
 */

typedef enum {
    BUTTON_IDLE_STATE,
    BUTTON_PRESSED_1_STATE,
    BUTTON_RELEASED_1_STATE,
    BUTTON_PRESSED_2_STATE,
    BUTTON_RELEASED_2_STATE,
    BUTTON_PRESSED_3_STATE,
    BUTTON_FORCE_AP_STATE,
    BUTTON_TIMEOUT_STATE, 
} MacroState;

typedef enum {
    CONTROL_RGB, 
    CONTROL_MACRO_1,
    CONTROL_MACRO_2,
    CONTROL_MACRO_3,
} ControlState;
