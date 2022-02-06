#pragma once

#define EE_HANDS

#ifdef JOYSTICK_SCAFFOLD_ENABLE
#   define SPLIT_TRANSACTION_IDS_USER USER_SYNC_A
// comment or remove these lines if thumb or/and index sticks are not present on the board
#   define USE_THUMB_STICKS
//#   define USE_INDEX_STICKS
#endif

#define COMBO_COUNT 11
#define COMBO_TERM 200

#ifdef OLED_DRIVER_ENABLE
#   define OLED_DISPLAY_128X64
#   define OLED_TIMEOUT 1200000
#   define OLED_BRIGHTNESS 120
#endif

