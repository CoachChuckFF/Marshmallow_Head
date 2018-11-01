/* Copyright (C) Blizzard Lighting LLC. - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Christian Krueger <christian@blizzardlighting.com>, January 2018
 *
 * Global Defines-> blizzard_global_defines.h
 *
 */

#ifndef BLIZZARD_GLOBAL_DEFINES_H
#define BLIZZARD_GLOBAL_DEFINES_H

#include <inttypes.h>

#include "blizzard_states.h"
#include "blizzard_errors.h"
#include "blizzard_animations.h"
#include "blizzard_dmx_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

//types - do not fuck with them
#define ENABLE          1
#define DISABLE         0

#define SEND            1
#define RECEIVE         0

#define DATA_U8         0
#define DATA_U16        1
#define DATA_U32        2
#define DATA_STRING     3

//maxes
#define MAX_DMX_SLOTS 513

#ifdef __cplusplus
}
#endif

#endif