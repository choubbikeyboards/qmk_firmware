#pragma once

// USB Device descriptor
#define PRODUCT_ID      0x0000
#define DEVICE_VER      0x0001
#define PRODUCT         scaffold V1
#define DESCRIPTION     small split keyboard with joysticks

#define MATRIX_ROWS 8 // left cols+right cols
#define MATRIX_COLS 6 // per side

#define MATRIX_ROW_PINS { D3, E6, B4, B5 }
#define MATRIX_COL_PINS { D7, B6, B2, B3, B1, C6 }
#define UNUSED_PINS

#define INDEX_STICK_R_FLIP_X false
#define INDEX_STICK_R_FLIP_Y false
#define INDEX_STICK_R_PIN_X F5
#define INDEX_STICK_R_PIN_Y F4
#define THUMB_STICK_R_FLIP_X true
#define THUMB_STICK_R_FLIP_Y false
#define THUMB_STICK_R_PIN_X F6
#define THUMB_STICK_R_PIN_Y F7

#define INDEX_STICK_L_FLIP_X false
#define INDEX_STICK_L_FLIP_Y true
#define INDEX_STICK_L_PIN_X F4
#define INDEX_STICK_L_PIN_Y F5
#define THUMB_STICK_L_FLIP_X false
#define THUMB_STICK_L_FLIP_Y true
#define THUMB_STICK_L_PIN_X F6
#define THUMB_STICK_L_PIN_Y F7

#define DIODE_DIRECTION COL2ROW

#define DEBOUNCE 5

