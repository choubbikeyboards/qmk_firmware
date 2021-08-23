#pragma once

#include "quantum.h"

#define LAYOUT( \
    LP3, LR3, LM3, LI3, LIA3,      RIA3, RI3, RM3, RR3, RP3, \
    LP2, LR2, LM2, LI2, LIA2,      RIA2, RI2, RM2, RR2, RP2, \
    LP1, LR1, LM1, LI1, LIA1,      RIA1, RI1, RM1, RR1, RP1, \
         LR0, LM0, LI0,                  RI0, RM0, RR0, \
                 LT1, LT2, LT3,   RT3, RT2, RT1 \
) \
{ \
    { LT3,   LIA3,   LI3,   LM3,   LR3,   LP3   }, \
    { LT2,   LIA2,   LI2,   LM2,   LR2,   LP2   }, \
    { LT1,   LIA1,   LI1,   LM1,   LR1,   LP1   }, \
    { KC_NO, KC_NO,  LI0,   LM0,   LR0,   KC_NO }, \
    { RT3,   RIA3,   RI3,   RM3,   RR3,   RP3   }, \
    { RT2,   RIA2,   RI2,   RM2,   RR2,   RP2   }, \
    { RT1,   RIA1,   RI1,   RM1,   RR1,   RP1   }, \
    { KC_NO, KC_NO,  RI0,   RM0,   RR0,   KC_NO }, \
}

