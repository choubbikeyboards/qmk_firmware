#include "joystick_scaffold.h"

void joystick_init(void)
{
    joystickTimer       = 0;

    joystick_state.config.mode_rt           = JOYSTICK_MODE_MOUSE_FINE;
    //joystick_state.config.mode_rt           = JOYSTICK_MODE_NONE;
    joystick_state.config.mode_ri           = JOYSTICK_MODE_ARROWS;
    joystick_state.config.mode_ri           = JOYSTICK_MODE_SCROLL;
    //joystick_state.config.mode_ri           = JOYSTICK_MODE_NONE;
    joystick_state.config.mode_lt           = JOYSTICK_MODE_MOUSE_COARSE;
    //joystick_state.config.mode_lt           = JOYSTICK_MODE_MOUSE_FINE;
    //joystick_state.config.mode_lt           = JOYSTICK_MODE_NONE;
    joystick_state.config.mode_li           = JOYSTICK_MODE_MOUSE_FINE;
    //joystick_state.config.mode_li           = JOYSTICK_MODE_NONE;
    joystick_state.config.deadZone          = JOYSTICK_DEAD_ZONE;
    joystick_state.config.fineZone          = JOYSTICK_FINE_ZONE;
    joystick_state.config.speed             = JOYSTICK_SPEED;
    joystick_state.config.fineSpeed         = JOYSTICK_FINE_SPEED;
    joystick_state.config.axisSeparation    = JOYSTICK_AXIS_SEPARATION;
    joystick_state.config.eightAxis         = JOYSTICK_EIGHT_AXIS;
    if(isLeftHand)
    {
        joystick_state.config.centerIndex.x     = analogReadPin(INDEX_STICK_L_PIN_X);
        joystick_state.config.centerIndex.y     = analogReadPin(INDEX_STICK_L_PIN_Y);
        joystick_state.config.centerThumb.x     = analogReadPin(THUMB_STICK_L_PIN_X);
        joystick_state.config.centerThumb.y     = analogReadPin(THUMB_STICK_L_PIN_Y);
    }
    else
    {
        joystick_state.config.centerIndex.x     = analogReadPin(INDEX_STICK_R_PIN_X);
        joystick_state.config.centerIndex.y     = analogReadPin(INDEX_STICK_R_PIN_Y);
        joystick_state.config.centerThumb.x     = analogReadPin(THUMB_STICK_R_PIN_X);
        joystick_state.config.centerThumb.y     = analogReadPin(THUMB_STICK_R_PIN_Y);
    }
    joystick_state.vector_slave_index.x     = 0;
    joystick_state.vector_slave_index.y     = 0;
    joystick_state.vector_slave_thumb.x     = 0;
    joystick_state.vector_slave_thumb.y     = 0;
}

// Get single vector from analog values
int16_t joystick_get_component(pin_t pin, uint16_t center, bool flip)
{
    uint16_t analogValue = analogReadPin(pin);
    // Compute direction
    bool directionIsPositive = (analogValue > center);
    // Compute distance from the center
    uint16_t distance = directionIsPositive ? (analogValue - center) : (center - analogValue);
    if (distance < joystick_state.config.deadZone)
        distance    = 0;
    else
        if (distance < joystick_state.config.deadZone + joystick_state.config.fineZone)
            distance    = 1;
        else
            distance    -= joystick_state.config.deadZone + joystick_state.config.fineZone;
    // Compute component (range of [0 to 1023])
    if (flip)
        return directionIsPositive ? distance : -(int16_t)distance;
    else
        return directionIsPositive ? -(int16_t)distance : distance;
}

void joystick_get_components(joystick_vector_t* vector, joystick_vector_t center, joystick_mode_t mode, pin_t pinX, pin_t pinY, bool flipX, bool flipY)
{
    uint16_t analogX = analogReadPin(pinX);
    uint16_t analogY = analogReadPin(pinY);
    bool positiveX = analogX > center.x;
    bool positiveY = analogY > center.y;
    if(mode == JOYSTICK_MODE_MOUSE_FINE || mode == JOYSTICK_MODE_MOUSE_COARSE)
    {
        analogX = positiveX ? analogX - center.x : center.x - analogX;
        analogY = positiveY ? analogY - center.y : center.y - analogY;
        if(analogX < joystick_state.config.deadZone && analogY < joystick_state.config.deadZone)
        {
            vector->x = 0;
            vector->y = 0;
        }
        else
        {
            // change response curve: x=x^2, same for y
            analogX = (analogX)/2; // get value below 256, from 10b measured analog value (-512:511)
            analogY = (analogY)/2;
//            analogX = analogX * analogX;
  //          analogY = analogY * analogY;
//            analogX = 1;
  //          analogY = 10;
            // invert analog value if not positive or flip requested, but not both
            vector->x = positiveX ^ flipX ? analogX : -analogX;
            vector->y = positiveY ^ flipY ? analogY : -analogY;
        }
    }
    else // only fine mouse movements get x^2 curve, others are linear
    {
        vector->x   = joystick_get_component(pinX, center.x, flipX);
        vector->y   = joystick_get_component(pinY, center.y, flipY);
    }    
}

#ifdef SPLIT_KEYBOARD
// executed by slave: collect joystick state and write joystick data in transport buffer
void joystick_state_raw(uint16_t* slave_state)
{
    joystick_vector_t vector;
    // copy data from this side
    if(isLeftHand)
    {
        joystick_get_components(&vector, joystick_state.config.centerIndex, joystick_state.config.mode_li, INDEX_STICK_L_PIN_X, INDEX_STICK_L_PIN_Y, INDEX_STICK_L_FLIP_X, INDEX_STICK_L_FLIP_Y);
        slave_state[0] = vector.x;
        slave_state[1] = vector.y;
        joystick_get_components(&vector, joystick_state.config.centerThumb, joystick_state.config.mode_lt, THUMB_STICK_L_PIN_X, THUMB_STICK_L_PIN_Y, THUMB_STICK_L_FLIP_X, THUMB_STICK_L_FLIP_Y);
        slave_state[2] = vector.x; 
        slave_state[3] = vector.y; 
    }
    else
    {
        joystick_get_components(&vector, joystick_state.config.centerIndex, joystick_state.config.mode_ri, INDEX_STICK_R_PIN_X, INDEX_STICK_R_PIN_Y, INDEX_STICK_R_FLIP_X, INDEX_STICK_R_FLIP_Y);
        slave_state[0] = vector.x;
        slave_state[1] = vector.y;
        joystick_get_components(&vector, joystick_state.config.centerThumb, joystick_state.config.mode_rt, THUMB_STICK_R_PIN_X, THUMB_STICK_R_PIN_Y, THUMB_STICK_R_FLIP_X, THUMB_STICK_R_FLIP_Y);
        slave_state[2] = vector.x; 
        slave_state[3] = vector.y; 
    }
}

// executed by master: update joystick_state with data from slave
void joystick_update_raw(uint16_t* slave_state)
{
    // copy data from the other side
    joystick_state.vector_slave_index.x = slave_state[0];
    joystick_state.vector_slave_index.y = slave_state[1];
    joystick_state.vector_slave_thumb.x = slave_state[2];
    joystick_state.vector_slave_thumb.y = slave_state[3];
}
#endif

// update function vectors with new [x/y] components according to mode
void joystick_update_vectors(uint16_t x, uint16_t y, joystick_mode_t mode)
{
    if(mode != JOYSTICK_MODE_NONE)
        switch(mode)
        {
            case JOYSTICK_MODE_MOUSE_COARSE:
                joystick_state.pointerVector.x += x*JOYSTICK_SPEED;
                joystick_state.pointerVector.y += y*JOYSTICK_SPEED;
                break;
            case JOYSTICK_MODE_MOUSE_FINE:
                joystick_state.pointerVector.x += x;
                joystick_state.pointerVector.y += y;
                break;
            case JOYSTICK_MODE_ARROWS:
                joystick_state.arrowVector.x += x;
                joystick_state.arrowVector.y += y;
                break;
            case JOYSTICK_MODE_SCROLL:
                joystick_state.scrollVector.x += x;
                joystick_state.scrollVector.y -= y;
                break;
            default:
                break;
        }
}

// Clear vector values, except their decimal part (keep accumulated micro-movements)
void joystick_clear_vectors(void)
{
    joystick_state.pointerVector.x  = joystick_state.pointerVector.x % JOYSTICK_FINE_SPEED;
    joystick_state.pointerVector.y  = joystick_state.pointerVector.y % JOYSTICK_FINE_SPEED;
    joystick_state.scrollVector.x   = joystick_state.scrollVector.x  % JOYSTICK_SCROLL_SPEED;
    joystick_state.scrollVector.y   = joystick_state.scrollVector.y  % JOYSTICK_SCROLL_SPEED;
    joystick_state.arrowVector.x    = joystick_state.arrowVector.x;
    joystick_state.arrowVector.y    = joystick_state.arrowVector.y;
}

// Get x/y components for joysticks on the master side by reading analog pins; get vectors for master/slave sticks
void joystick_get_vectors(void)
{
    joystick_vector_t vector;
    if(isLeftHand)
    {
        if(joystick_state.config.mode_li != JOYSTICK_MODE_NONE)
        {
            joystick_get_components(&vector, joystick_state.config.centerIndex, joystick_state.config.mode_li, INDEX_STICK_L_PIN_X, INDEX_STICK_L_PIN_Y, INDEX_STICK_L_FLIP_X, INDEX_STICK_L_FLIP_Y);
            joystick_update_vectors(vector.x, vector.y, joystick_state.config.mode_li);
        }
        if(joystick_state.config.mode_lt != JOYSTICK_MODE_NONE)
        {
            joystick_get_components(&vector, joystick_state.config.centerThumb, joystick_state.config.mode_lt, THUMB_STICK_L_PIN_X, THUMB_STICK_L_PIN_Y, THUMB_STICK_L_FLIP_X, THUMB_STICK_L_FLIP_Y);
            joystick_update_vectors(vector.x, vector.y, joystick_state.config.mode_lt);
        }
        joystick_update_vectors(joystick_state.vector_slave_index.x, joystick_state.vector_slave_index.y, joystick_state.config.mode_ri);
        joystick_update_vectors(joystick_state.vector_slave_thumb.x, joystick_state.vector_slave_thumb.y, joystick_state.config.mode_rt);
    }
    else
    {
        if(joystick_state.config.mode_ri != JOYSTICK_MODE_NONE)
        {
            joystick_get_components(&vector, joystick_state.config.centerIndex, joystick_state.config.mode_ri, INDEX_STICK_R_PIN_X, INDEX_STICK_R_PIN_Y, INDEX_STICK_R_FLIP_X, INDEX_STICK_R_FLIP_Y);
            joystick_update_vectors(vector.x, vector.y, joystick_state.config.mode_ri);
        }
        if(joystick_state.config.mode_rt != JOYSTICK_MODE_NONE)
        {
            joystick_get_components(&vector, joystick_state.config.centerThumb, joystick_state.config.mode_rt, THUMB_STICK_R_PIN_X, THUMB_STICK_R_PIN_Y, THUMB_STICK_R_FLIP_X, THUMB_STICK_R_FLIP_Y);
            joystick_update_vectors(vector.x, vector.y, joystick_state.config.mode_rt);
        }
        joystick_update_vectors(joystick_state.vector_slave_index.x, joystick_state.vector_slave_index.y, joystick_state.config.mode_li);
        joystick_update_vectors(joystick_state.vector_slave_thumb.x, joystick_state.vector_slave_thumb.y, joystick_state.config.mode_lt);
    }
}

// press MOUSE_BTN1/MOUSE_BTN8
void joystick_press_buttons(uint8_t buttons)
{
    joystick_state.buttons  = joystick_state.buttons | buttons;
}

// release MOUSE_BTN1/MOUSE_BTN8
void joystick_release_buttons(uint8_t buttons)
{
    joystick_state.buttons  = joystick_state.buttons & (~buttons);
}

//void joystick_mode_set(joystick_mode_t mode) { joystick_state.config.mode = mode; }

//joystick_mode_t joystick_mode_get(void) { return joystick_state.config.mode; }

//void joystick_mode_cycle(bool reverse) {
//    joystick_mode_t mode = joystick_mode_get();
//    if (reverse) {
//        mode = (mode == 0) ? (_JOYSTICK_MODE_LAST - 1) : (mode - 1);
//    } else {
//        mode = (mode == (_JOYSTICK_MODE_LAST - 1)) ? 0 : (mode + 1);
//    }
//    joystick_mode_set(mode);
//}

// Fix direction within one of 8 axes (or 4 if 8-axis is disabled)
joystick_direction_t joystick_get_discretized_direction(joystick_vector_t vector, bool eightAxis)
{
    joystick_direction_t direction;
    uint16_t               absX                = abs(vector.x);
    uint16_t               absY                = abs(vector.y);
    uint16_t               maxComponent        = (absX > absY) ? absX : absY;
    bool                   outsideDiagonalZone = ((abs(absX - absY) / (float)maxComponent) >= joystick_state.config.axisSeparation);
    if (vector.x == 0 && vector.y == 0)
        direction.up = direction.down = direction.left = direction.right = false;
    else
    {
        direction.up    = (vector.y < 0);
        direction.down  = (vector.y > 0);
        direction.left  = (vector.x < 0);
        direction.right = (vector.x > 0);
        // Let only the dominant direction remain under the right conditions
        if (outsideDiagonalZone || !eightAxis)
        {
            if (absX > absY)
                direction.up = direction.down = false;
            else
                direction.left = direction.right = false;
        }
    }
    return direction;
}

joystick_direction_t scrollDirection;  // Declaring global to save stack space
int16_t component;

// main function called from pointing_device_task()
void joystick_process(void)
{
    if (timer_elapsed(joystickTimer) > JOYSTICK_TIMEOUT)
    {
        joystickTimer = timer_read();
        joystick_clear_vectors();
        joystick_get_vectors();
        // Mouse report
        if(joystick_state.pointerVector.x/JOYSTICK_FINE_SPEED < -127)
            joystick_state.report.x = -127;
        else if(joystick_state.pointerVector.x/JOYSTICK_FINE_SPEED > 127)
            joystick_state.report.x = 127;
        else
            joystick_state.report.x = joystick_state.pointerVector.x / JOYSTICK_FINE_SPEED;

        if(joystick_state.pointerVector.y/JOYSTICK_FINE_SPEED < -127)
            joystick_state.report.y = -127;
        else if(joystick_state.pointerVector.y/JOYSTICK_FINE_SPEED > 127)
            joystick_state.report.y = 127;
        else
            joystick_state.report.y = joystick_state.pointerVector.y / JOYSTICK_FINE_SPEED;
        // Arrow report
        joystick_state.direction = joystick_get_discretized_direction(joystick_state.arrowVector, joystick_state.config.eightAxis);
        // Scroll report
        if(joystick_state.scrollVector.x < -127*JOYSTICK_SCROLL_SPEED)
            joystick_state.report.h = -127;
        else if(joystick_state.scrollVector.x > 127*JOYSTICK_SCROLL_SPEED)
            joystick_state.report.h = 127;
        else
            joystick_state.report.h = joystick_state.scrollVector.x / JOYSTICK_SCROLL_SPEED;

        if(joystick_state.scrollVector.y < -127*JOYSTICK_SCROLL_SPEED)
            joystick_state.report.v = -127;
        else if(joystick_state.scrollVector.y > 127*JOYSTICK_SCROLL_SPEED)
            joystick_state.report.v = 127;
        else
            joystick_state.report.v = joystick_state.scrollVector.y / JOYSTICK_SCROLL_SPEED;
    }
}

void update_keycode_status(uint16_t keycode, bool last, bool current)
{
    if (last != current)
    {
        if (current)
            register_code16(keycode);
        else
            unregister_code16(keycode);
    }
}

// QMK function overwrite
void pointing_device_init(void)
{
    joystick_init();
}

// QMK function overwrite
void pointing_device_task(void)
{
    report_mouse_t report = pointing_device_get_report();

    // Get stick values and convert them to a pointer/arrow/scrollwheel report
    joystick_process();
    // Set mouse pointer report
    report.x = joystick_state.report.x;
    report.y = joystick_state.report.y;
    // Press or release keys depending on directions detected on arrow thumbstick
    update_keycode_status(KC_UP, joystick_state.lastDirection.up, joystick_state.direction.up);
    update_keycode_status(KC_DOWN, joystick_state.lastDirection.down, joystick_state.direction.down);
    update_keycode_status(KC_LEFT, joystick_state.lastDirection.left, joystick_state.direction.left);
    update_keycode_status(KC_RIGHT, joystick_state.lastDirection.right, joystick_state.direction.right);
    joystick_state.lastDirection = joystick_state.direction;
    // Set mouse scroll wheel report
    report.h = joystick_state.report.h;
    report.v = joystick_state.report.v;
    report.buttons = joystick_state.buttons;

    pointing_device_set_report(report);
    pointing_device_send();
}

