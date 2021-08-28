#pragma once

typedef enum {
    JOYSTICK_MODE_MOUSE = 0,
    JOYSTICK_MODE_SCROLL,
    JOYSTICK_MODE_NONE
} joystick_mode_t;

// Parameters
#define JOYSTICK_DEAD_ZONE 15      // Values below this are ignored for strongest component (deadzone)
#define JOYSTICK_FINE_ZONE 50      // Values below this enable fine movement

#define JOYSTICK_MODE JOYSTICK_MODE_MOUSE
#define JOYSTICK_SPEED 256
#define JOYSTICK_SCROLL_SPEED 256

#define JOYSTICK_EIGHT_AXIS true
#define JOYSTICK_AXIS_SEPARATION 0.5f

// Implicit and derived constants
#define JOYSTICK_TIMEOUT 10          // Mouse report throttling time in ms
#define JOYSTICK_SCROLL_TIMEOUT 200  // Mouse scroll throttling time in ms
#define JOYSTICK_RANGE_START 0
#define JOYSTICK_RANGE_STOP 1023
#define JOYSTICK_RANGE_CENTER (JOYSTICK_RANGE_STOP - JOYSTICK_RANGE_START + 1) / 2
#define JOYSTICKS_CALIBRATED true // set to true only if you measured the center of each axis of each joystick you use.
// Don't mind the values for the sticks you didn't install, but be sure to put their role to JOYSTICK_MODE_NONE
#define JOYSTICK_RANGE_CENTER_LIX 470
#define JOYSTICK_RANGE_CENTER_LIY 515
#define JOYSTICK_RANGE_CENTER_LTX 472
#define JOYSTICK_RANGE_CENTER_LTY 515
#define JOYSTICK_RANGE_CENTER_RIX 509
#define JOYSTICK_RANGE_CENTER_RIY 543
#define JOYSTICK_RANGE_CENTER_RTX 505
#define JOYSTICK_RANGE_CENTER_RTY 489
#define JOYSTICK_RANGE_MOVEMENT (JOYSTICK_RANGE_CENTER - JOYSTICK_DEAD_ZONE)

#include "timer.h"
#include "analog.h"
#include "split_util.h"
#include "pointing_device.h"

typedef struct {
    int16_t x;
    int16_t y;
} joystick_vector_t;

typedef struct {
    joystick_mode_t     mode_rt; // mode of the joystick under right thumb
    joystick_mode_t     mode_ri; // mode of the joystick under right index
    joystick_mode_t     mode_lt; // mode of the joystick under left thumb
    joystick_mode_t     mode_li; // mode of the joystick under left index
    joystick_vector_t   centerThumb;
    joystick_vector_t   centerIndex;
    uint16_t            deadZone;
    uint16_t            fineZone;
    int16_t             speed;
    int16_t             scroll_speed;
    float               axisSeparation;
    bool                eightAxis;
} joystick_config_t;

typedef struct {
    bool up;
    bool right;
    bool down;
    bool left;
} joystick_direction_t;

typedef struct {
    joystick_config_t    config;
    joystick_vector_t    vector_slave_index;
    joystick_vector_t    vector_slave_thumb;
    joystick_vector_t    pointerVector;
    joystick_vector_t    scrollVector;
    uint8_t              buttons;
    report_mouse_t       report;
} joystick_state_t;

uint16_t joystickTimer;
uint16_t joystickScrollTimer;

joystick_state_t joystick_state;

//void joystick_mode_set(joystick_mode_t mode);

//joystick_mode_t joystick_mode_get(void);

//void joystick_mode_cycle(bool reverse);

void joystick_init(void);

// Get single vector from analog values
int16_t joystick_get_component(pin_t pin, uint16_t center, bool flip);

#ifdef SPLIT_KEYBOARD
// executed by slave: collect joystick state and write joystick data in transport buffer
void joystick_state_raw(uint16_t* slave_state);

// executed by master: update joystick_state with data from slave
void joystick_update_raw(uint16_t* slave_state);
#endif

// for manual calibration
void print_joysticks(void);

// update function vectors with new [x/y] components according to mode
void joystick_update_vectors(uint16_t x, uint16_t y, joystick_mode_t mode);

// Clear vector values, except their decimal part (keep accumulated micro-movements)
void joystick_clear_vectors(void);

// Get x/y components for joysticks on the master side by reading analog pins; get vectors for master/slave sticks
void joystick_get_vectors(void);

// press MOUSE_BTN1/MOUSE_BTN8
void joystick_press_buttons(uint8_t buttons);

// release MOUSE_BTN1/MOUSE_BTN8
void joystick_release_buttons(uint8_t buttons);

// Fix direction within one of 8 axes (or 4 if 8-axis is disabled)
joystick_direction_t joystick_get_discretized_direction(joystick_vector_t vector, bool eightAxis);

void joystick_process(void);

void update_keycode_status(uint16_t keycode, bool last, bool current);

