/* Copyright (C) Blizzard Lighting LLC. - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Christian Krueger <christian@blizzardlighting.com>, February 2018
 *
 * Pixel Manager -> animation_manager.cpp
 *
 */

#include <stdlib.h>
#include "blizzard_animation_manager.h"
#include "color_macros.h"
#include "pixel_maps.h"

#define RS_HIT_X 3
#define RS_HB_X 35

#define PIXEL_COUNT (CONFIG_PIXEL_COLS * CONFIG_PIXEL_ROWS - INVALID_PIXEL_COUNT)

bool tick_frame(uint32_t tick_tick, uint8_t speed){
    if(speed == 0) return true;
    return (tick_tick % speed == 0);
}

//Forground Effects

void arrows(uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y){
    forground_sprites(PIXEL_ARROWS_COLS, PixelArrows[0], tick_tick, speed, direction, flipped, x, y, false);
}

void danger(uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y){
    forground_sprites(PIXEL_DANGER_COLS, PixelDanger[0], tick_tick, speed, direction, flipped, x, y, false);
}

void check(uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y){
    forground_sprites(PIXEL_CHECK_COLS, PixelCheck[0], tick_tick, speed, direction, flipped, x, y, false);
}

void mount(uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y){
    forground_sprites(PIXEL_MOUNT_COLS, PixelMount[0], tick_tick, speed, direction, flipped, x, y, false);
}

void camo(uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y){
    forground_sprites(PIXEL_CAMO_COLS, PixelCamo[0], tick_tick, speed, direction, flipped, x, y, false);
}

void soc(uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y){
    forground_sprites(PIXEL_SOC_COLS, PixelSoc[0], tick_tick, speed, direction, flipped, x, y, false);
}

void blizzard(uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y){
    forground_sprites(PIXEL_BLIZZARD_COLS, PixelBlizzard[0], tick_tick, speed, direction, flipped, x, y, false);
}

void pac(uint32_t tick_tick, uint8_t speed, uint8_t set_frame){
    static uint8_t frame = 0;
    static uint32_t tock_tock = 0;

    if(set_frame > 255 - 3){
        frame = 255 - set_frame;
        tock_tock = 0;
    } else if(tick_frame(tick_tick, speed)){
        frame++;
        tock_tock++;
    }

    if(frame > 2){
        frame = 0;
    }

    forground_sprites(PIXEL_PAC_COLS, PixelPac[frame], tock_tock, 0, DIR_LR - 5, true, 0, 0, false);
}

void rs(uint32_t tick_tick, uint8_t speed, uint8_t set_frame){
    static uint8_t frame = 0;

    if(set_frame > 255 - 5){
        frame = 255 - set_frame;
    } else if(tick_frame(tick_tick, speed)){
        frame++;
    }

    if(frame > 4){
        frame = 0;
    }

    switch(frame){
        case 0:
            forground_sprites(PIXEL_HB_COLS, PixelHb[1], tick_tick, speed, DIR_MAN - 1, true, RS_HB_X, 0, false);
        break;
        case 1: 
            forground_sprites(PIXEL_HIT_COLS, PixelHit[0], tick_tick, speed, DIR_MAN - 1, true, RS_HIT_X-1, 1, false);
            forground_sprites(PIXEL_HB_COLS, PixelHb[0], tick_tick, speed, DIR_MAN - 1, true, RS_HB_X, 0, false);
        break;
        case 2: 
            forground_sprites(PIXEL_HIT_COLS, PixelHit[2], tick_tick, speed, DIR_MAN - 1, true, RS_HIT_X-1, 1, false);
            forground_sprites(PIXEL_HB_COLS, PixelHb[0], tick_tick, speed, DIR_MAN - 1, true, RS_HB_X, 0, false);
        break;
        case 3:
            forground_sprites(PIXEL_HIT_COLS, PixelHit[1], tick_tick, speed, DIR_MAN - 1, true, RS_HIT_X, 0, false);
            forground_sprites(PIXEL_HB_COLS, PixelHb[2], tick_tick, speed, DIR_MAN - 1, true, RS_HB_X, 0, false);
        break;
        case 4:
            forground_sprites(PIXEL_HB_COLS, PixelHb[2], tick_tick, speed, DIR_MAN - 1, true, RS_HB_X, 0, false);
        break;
    }
    
}

void don(uint32_t tick_tick, uint8_t speed){
    forground_sprites(PIXEL_DON_COLS, PixelDon[0], tick_tick, speed, DIR_LR - 5, true, 0, 0, false);
}

void test(uint32_t tick_tick, uint8_t speed){
    forground_sprites(PIXEL_TEST_COLS, PixelTest[0], tick_tick, speed, DIR_LR - 5, true, 0, 0, false);
}

void game(uint8_t x, uint8_t y){
    set_pixel_x_y(x, y, 255, 255, 255, PIXEL_SET);
}

void reset_sprites(){
    forground_sprites(0, NULL, 0, 0, 0, false, 0, 0, true);
}

void forground_sprites(uint16_t length, uint8_t frame[][CONFIG_PIXEL_ROWS][3], uint32_t tick_tick, uint8_t speed, uint8_t direction, bool flipped, uint8_t x, uint8_t y, bool reset){
    static uint32_t tock_tock = 0;
    static uint32_t tack_tack = 0;

    if(reset){
        tock_tock = 0;
        tack_tack = 0;
        return;
    }

    if(tick_frame(tick_tick, speed)){
        if(direction < DIR_NONE){
            tock_tock = 0;
            tack_tack = 0;
        } else if(direction < DIR_LR){
            tock_tock++; 
            tack_tack = 0;
        } else if(direction < DIR_RL){
            tock_tock--; 
            tack_tack = 0;
        } else if(direction < DIR_UD){
            tock_tock = 0; 
            tack_tack++;
        } else if(direction < DIR_DU){
            tock_tock = 0; 
            tack_tack--;
        } else if(direction < DIR_SUR){
            tock_tock++; 
            tack_tack++;
        } else if(direction < DIR_SDR){
            tock_tock++; 
            tack_tack--;
        } else if(direction < DIR_SUL){
            tock_tock--; 
            tack_tack++;
        } else if(direction < DIR_SDL){
            tock_tock--; 
            tack_tack--;
        } else {
            paint_sprite(length, frame, x, y, flipped);
            return;
        }
    } else if(direction >= DIR_SDL){
        paint_sprite(length, frame, x, y, flipped);
        return;
    }

    paint_sprite(length, frame, tock_tock, tack_tack, flipped);

}

//Background Effects

void solid(uint8_t red, uint8_t green, uint8_t blue){
    set_all_pixels(red, green, blue);
}

void bstrobe(uint32_t tick_tick, uint8_t speed, uint8_t red, uint8_t green, uint8_t blue){
    static bool on = true;

    if(tick_frame(tick_tick, speed)){
        on = !on;
    }

    if(on){
        set_all_pixels(red, green, blue);
    } else {    
        clear_pixels();
    }
}

void plaid(uint32_t tick_tick, uint8_t speed, uint8_t density, uint8_t direction, uint8_t red, uint8_t green, uint8_t blue, bool cross, bool cols, bool rows){
    static uint32_t tock_tock = 5000;
    static uint32_t tack_tack = 5000;
    uint8_t col_divs = 0xFF;
    uint8_t row_divs = 0xFF;

    
    if(tick_frame(tick_tick, speed)){
        if(cols && !rows){
            if(direction < DIR_NONE + 60){
                tock_tock = 0;
                tack_tack = 0;
            } else if(direction < DIR_LR + 120){
                tock_tock++; 
                tack_tack = 0;
            } else {
                tock_tock--; 
                tack_tack = 0;
            }
        } else if(rows && !cols){ 
            if(direction < DIR_NONE + 60){
                tock_tock = 0;
                tack_tack = 0;
            } else if(direction < DIR_UD + 120){
                tock_tock = 0; 
                tack_tack++;
            } else {
                tock_tock = 0; 
                tack_tack--;
            }
        } else if(cols && rows){
            if(direction < DIR_NONE){
                tock_tock = 0;
                tack_tack = 0;
            } else if(direction < DIR_LR){
                tock_tock++; 
                tack_tack = 0;
            } else if(direction < DIR_RL){
                tock_tock--; 
                tack_tack = 0;
            } else if(direction < DIR_UD){
                tock_tock = 0; 
                tack_tack++;
            } else if(direction < DIR_DU){
                tock_tock = 0; 
                tack_tack--;
            } else if(direction < DIR_SUR){
                tock_tock++; 
                tack_tack++;
            } else if(direction < DIR_SDR){
                tock_tock++; 
                tack_tack--;
            } else if(direction < DIR_SUL){
                tock_tock--; 
                tack_tack++;
            } else if(direction < DIR_SDR){
                tock_tock--; 
                tack_tack--;
            }
        }
    }

    if(cols){
        if(density < COL_MOD_2){
            col_divs = 2;
        } else if(density < COL_MOD_3){
            col_divs = 3;
        } else if(density < COL_MOD_6){
            col_divs = 6;
        } else if(density < COL_MOD_9){
            col_divs = 9;
        } else if(density < COL_MOD_18){
            col_divs = 18;
        } else if(density < COL_MOD_27){
            col_divs = 27;
        }

        for(int i = 0; i < CONFIG_PIXEL_COLS; i++){
            if((tock_tock + i) % col_divs == 0){
                set_pixel_col(i, red, green, blue, PIXEL_SET);
            }
        }
    }

    if(rows){
        if(density < ROW_MOD_2){
            row_divs = 2;
        } else if(density < ROW_MOD_3){
            row_divs = 3;
        } else if(density < ROW_MOD_4){
            row_divs = 4;
        } else if(density < ROW_MOD_6){
            row_divs = 6;
        }

        for(int i = 0; i < CONFIG_PIXEL_ROWS; i++){
            if((tack_tack + i) % row_divs == 0){
                set_pixel_row(i, red, green, blue, (cross) ? PIXEL_SET : PIXEL_ADD);
            }
        }
    }

}

void sparkles(uint32_t tick_tick, uint8_t speed, uint8_t saturation, uint8_t red, uint8_t green, uint8_t blue){
    if(tick_frame(tick_tick, speed)){
        srand(tick_tick);
    }

    for(int i = 0; i < saturation; i++){
        set_pixel(rand() % PIXEL_COUNT, red, green, blue, PIXEL_ADD);
    }
}

void running_lights(uint32_t tick_tick, uint8_t speed, uint8_t red, uint8_t green, uint8_t blue){
    static uint8_t tock_tock = 0;
    if(tick_frame(tick_tick, speed)){
        tock_tock++;
    }
    for(int i = 0; i < PIXEL_COUNT; i++) {
        float level = sin(i+tock_tock) * 127 + 128;
        set_pixel(i,
                ((level)/255)*red,
                ((level)/255)*green,
                ((level)/255)*blue,
                PIXEL_SET
        );
    }
}

void rainbow(uint32_t tick_tick, uint8_t speed, uint8_t direction, uint8_t mode){
    static uint32_t tock_tock = 0;
    static uint32_t tack_tack = 0;

    switch(mode){
        case MODE_FADE:
            if(direction < DIR_NONE + 25){
                tock_tock = 0;
                tack_tack = 0;
                for(int i = 0, j = 0; i < CONFIG_PIXEL_COLS; i++, j+=21){
                    CRGB color = get_rainbow_color((uint8_t) ((tock_tock + i + j)));
                    set_pixel_col(i, color[RED_INDEX], color[GREEN_INDEX], color[BLUE_INDEX], PIXEL_SET);
                }
            } else if(direction < DIR_LR + 50){
                if(tick_frame(tick_tick, speed)){
                    tock_tock++; 
                    tack_tack = 0;
                }
                for(int i = 0, j = 0; i < CONFIG_PIXEL_COLS; i++, j+=21){
                    CRGB color = get_rainbow_color((uint8_t) ((tock_tock + i + j)));
                    set_pixel_col(i, color[RED_INDEX], color[GREEN_INDEX], color[BLUE_INDEX], PIXEL_SET);
                }
            } else if(direction < DIR_RL + 75){
                if(tick_frame(tick_tick, speed)){
                    tock_tock--; 
                    tack_tack = 0;
                }
                for(int i = 0, j = 0; i < CONFIG_PIXEL_COLS; i++, j+=21){
                    CRGB color = get_rainbow_color((uint8_t) ((tock_tock + i + j)));
                    set_pixel_col(i, color[RED_INDEX], color[GREEN_INDEX], color[BLUE_INDEX], PIXEL_SET);
                }
            } else if(direction < DIR_UD + 100){
                if(tick_frame(tick_tick, speed)){
                    tock_tock = 0; 
                    tack_tack++;
                }
                for(int i = 0, j = 0; i < CONFIG_PIXEL_ROWS; i++, j+=21){
                    CRGB color = get_rainbow_color((uint8_t) ((tack_tack + i + j)));
                    set_pixel_row(i, color[RED_INDEX], color[GREEN_INDEX], color[BLUE_INDEX], PIXEL_SET);
                }
            } else {
                if(tick_frame(tick_tick, speed)){
                    tock_tock = 0;
                    tack_tack--;
                }
                for(int i = 0, j = 0; i < CONFIG_PIXEL_ROWS; i++, j+=21){
                    CRGB color = get_rainbow_color((uint8_t) ((tack_tack + i + j)));
                    set_pixel_row(i, color[RED_INDEX], color[GREEN_INDEX], color[BLUE_INDEX], PIXEL_SET);
                }
            }
        break;
        case MODE_FLASH:
            if(tick_frame(tick_tick, speed)){
                tock_tock++;
            }
            tock_tock %= 7;
            set_all_pixels(Rainbow[tock_tock][0],Rainbow[tock_tock][1],Rainbow[tock_tock][2]);
        break;
    }
}
