#pragma once

#define EE_HANDS

#define COMBO_COUNT 11
#define COMBO_TERM 200

#ifdef OLED_DRIVER_ENABLE
  #define OLED_DISPLAY_128X64
  #define OLED_TIMEOUT 1200000
  #define OLED_BRIGHTNESS 120
#endif

