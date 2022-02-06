#ifdef JOYSTICK_SCAFFOLD_ENABLE

#include "joystick_scaffold.h"
#include "transactions.h"
#include "print.h"
#include "string.h"

typedef struct _slave_to_master_t {
    int16_t index_x;
    int16_t index_y;
    int16_t thumb_x;
    int16_t thumb_y;
} slave_to_master_t;

slave_to_master_t *p_s2m_joysticks_scaffold;
slave_to_master_t s2m_joysticks_scaffold;

void user_sync_a_slave_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    p_s2m_joysticks_scaffold = (slave_to_master_t*)out_data;
    if (!is_keyboard_master())
        joystick_state_raw();
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(USER_SYNC_A, user_sync_a_slave_handler);
}

void housekeeping_task_user(void) {
    if (is_keyboard_master()) {
        // Interact with slave every 500ms
        static uint32_t last_sync = 0;
        if (timer_elapsed32(last_sync) > 10)
        {
            if(transaction_rpc_exec(USER_SYNC_A, 0, NULL, sizeof(s2m_joysticks_scaffold), &s2m_joysticks_scaffold))
            {
                last_sync = timer_read32();
//                if (s2m_joysticks_scaffold.index_x != 0 || s2m_joysticks_scaffold.index_y != 0 || s2m_joysticks_scaffold.thumb_x != 0 || s2m_joysticks_scaffold.thumb_y != 0)
//                    uprintf("Slave side: %d, %d, %d, %d, %d\n", s2m_joysticks_scaffold.index_x, s2m_joysticks_scaffold.index_y, s2m_joysticks_scaffold.thumb_x, s2m_joysticks_scaffold.thumb_y);
         /*       else
                    print(".");*/
                joystick_update_raw();
            }
        }
    }
}

void joystick_init(void)
{
    joystickTimer       = 0;

    joystick_state.config.mode_rt           = JOYSTICK_MODE_SCROLL_1D;
    joystick_state.config.mode_ri           = JOYSTICK_MODE_MOUSE;
    joystick_state.config.mode_lt           = JOYSTICK_MODE_MOUSE;
    joystick_state.config.mode_li           = JOYSTICK_MODE_SCROLL_1D;
#ifndef USE_THUMB_STICKS
    joystick_state.config.mode_rt           = JOYSTICK_MODE_NONE;
    joystick_state.config.mode_lt           = JOYSTICK_MODE_NONE;
#endif
#ifndef USE_INDEX_STICKS
    joystick_state.config.mode_ri           = JOYSTICK_MODE_NONE;
    joystick_state.config.mode_li           = JOYSTICK_MODE_NONE;
#endif
#if JOYSTICKS_CALIBRATED
    joystick_state.config.deadZone          = JOYSTICK_DEAD_ZONE;
#endif
#if !JOYSTICKS_CALIBRATED
    joystick_state.config.deadZone          = JOYSTICK_DEAD_ZONE_UNCALIBRATED;
#endif
    joystick_state.config.fineZone          = JOYSTICK_FINE_ZONE;
    joystick_state.config.speed             = JOYSTICK_SPEED;
    joystick_state.config.scroll_speed      = JOYSTICK_SCROLL_SPEED;
    joystick_state.config.axisSeparation    = JOYSTICK_AXIS_SEPARATION;
    joystick_state.config.eightAxis         = JOYSTICK_EIGHT_AXIS;
    if(isLeftHand)
    {
#if !JOYSTICKS_CALIBRATED
        joystick_state.config.centerIndex.x     = analogReadPin(INDEX_STICK_L_PIN_X);
        joystick_state.config.centerIndex.y     = analogReadPin(INDEX_STICK_L_PIN_Y);
        joystick_state.config.centerThumb.x     = analogReadPin(THUMB_STICK_L_PIN_X);
        joystick_state.config.centerThumb.y     = analogReadPin(THUMB_STICK_L_PIN_Y);
#endif
#if JOYSTICKS_CALIBRATED
        joystick_state.config.centerIndex.x     = JOYSTICK_RANGE_CENTER_LIX;
        joystick_state.config.centerIndex.y     = JOYSTICK_RANGE_CENTER_LIY;
        joystick_state.config.centerThumb.x     = JOYSTICK_RANGE_CENTER_LTX;
        joystick_state.config.centerThumb.y     = JOYSTICK_RANGE_CENTER_LTY;
#endif
    }
    else
    {
#if !JOYSTICKS_CALIBRATED
        joystick_state.config.centerIndex.x     = analogReadPin(INDEX_STICK_R_PIN_X);
        joystick_state.config.centerIndex.y     = analogReadPin(INDEX_STICK_R_PIN_Y);
        joystick_state.config.centerThumb.x     = analogReadPin(THUMB_STICK_R_PIN_X);
        joystick_state.config.centerThumb.y     = analogReadPin(THUMB_STICK_R_PIN_Y);
#endif
#if JOYSTICKS_CALIBRATED
        joystick_state.config.centerIndex.x     = JOYSTICK_RANGE_CENTER_RIX;
        joystick_state.config.centerIndex.y     = JOYSTICK_RANGE_CENTER_RIY;
        joystick_state.config.centerThumb.x     = JOYSTICK_RANGE_CENTER_RTX;
        joystick_state.config.centerThumb.y     = JOYSTICK_RANGE_CENTER_RTY;
#endif
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
        distance    -= joystick_state.config.deadZone + joystick_state.config.fineZone;
    // Compute component (range of [0 to 1023])
    if (flip)
        return directionIsPositive ? distance : -(int16_t)distance;
    else
        return directionIsPositive ? -(int16_t)distance : distance;
}

void joystick_get_components(joystick_vector_t* vector, joystick_vector_t center, joystick_mode_t mode, pin_t pinX, pin_t pinY, bool flipX, bool flipY)
{
    uint32_t analogX;
    uint32_t analogY;
    bool positiveX;
    bool positiveY;
    uint32_t force;
    // dirty hack: reading only once the analog values seems to bring buggy results, mostly on the slave side (voltage too low?), reading twice seems to fix that.
    analogY = analogReadPin(pinY);
    analogX = analogReadPin(pinX);
    analogY = analogReadPin(pinY);
    analogX = analogReadPin(pinX);
    positiveX = analogX > center.x;
    positiveY = analogY > center.y;
    if(mode == JOYSTICK_MODE_MOUSE || mode == JOYSTICK_MODE_SCROLL_2D)// -- replace (true) to limit proportional response to a specific mode, mouse here
    {
        // get signed value with 0 being the cennter of the potentiometer
        analogX = positiveX ? analogX - center.x : center.x - analogX;
        analogY = positiveY ? analogY - center.y : center.y - analogY;

        // get 0 if pot in dead zone, tweaked value otherwise
        if(analogX < joystick_state.config.deadZone && analogY < joystick_state.config.deadZone)
        {
            vector->x = 0;
            vector->y = 0;
        }
        else
        {
//uprintf("%lu, (%d, %d), (%d,%u)/(%d,%u)\n", force, analogX, analogY, analogReadPin(pinX), center.x, analogReadPin(pinY), center.y);
            force =  (analogX*analogX) + (analogY*analogY);
            // change response curve, stronger acceleration the further away from the dead zone
            analogX = ((analogX * force)>>13)+1;
            analogY = ((analogY * force)>>13)+1;
//if(analogX > 32768 || analogY > 32768)
//uprintf("%lu, (%lu,%u)\n", force, analogY, center.y);
            // invert analog value if not positive or flip requested, but not both
            vector->x = positiveX ^ flipX ? analogX : -analogX;
            vector->y = positiveY ^ flipY ? analogY : -analogY;
        }
    }
    else if (mode == JOYSTICK_MODE_SCROLL_1D)
    {
        // get signed value with 0 being the cennter of the potentiometer
        analogX = 0;
        vector->x = 0;
        analogY = positiveY ? analogY - center.y : center.y - analogY;
        if (analogY < joystick_state.config.deadZone)
        {
            vector->y = 0;
        }
        else
        {
            analogY = analogY - joystick_state.config.deadZone + 1;
            force =  (analogY*analogY);
            // change response curve, stronger acceleration the further away from the dead zone
            analogY = ((analogY * force)>>13)+1;
            // invert analog value if not positive or flip requested, but not both
            vector->y = positiveY ^ flipY ? analogY : -analogY;
        }
    }
    /*
    else // if linear response is needed, it's here
    {
        vector->x   = joystick_get_component(pinX, center.x, flipX);
        vector->y   = joystick_get_component(pinY, center.y, flipY);
    }*/
}

#ifdef SPLIT_KEYBOARD
// executed by slave: collect joystick state and write joystick data in transport buffer
void joystick_state_raw(void)
{
    joystick_vector_t vector;

    p_s2m_joysticks_scaffold->index_x = 0;
    p_s2m_joysticks_scaffold->index_y = 0;
    p_s2m_joysticks_scaffold->thumb_x = 0; 
    p_s2m_joysticks_scaffold->thumb_y = 0; 
    // copy data from this side
    if(isLeftHand)
    {
#  ifdef USE_INDEX_STICKS
        joystick_get_components(&vector, joystick_state.config.centerIndex, joystick_state.config.mode_li, INDEX_STICK_L_PIN_X, INDEX_STICK_L_PIN_Y, INDEX_STICK_L_FLIP_X, INDEX_STICK_L_FLIP_Y);
        p_s2m_joysticks_scaffold->index_x = vector.x;
        p_s2m_joysticks_scaffold->index_y = vector.y;
#  endif
#  ifdef USE_THUMB_STICKS
        joystick_get_components(&vector, joystick_state.config.centerThumb, joystick_state.config.mode_lt, THUMB_STICK_L_PIN_X, THUMB_STICK_L_PIN_Y, THUMB_STICK_L_FLIP_X, THUMB_STICK_L_FLIP_Y);
        p_s2m_joysticks_scaffold->thumb_x = vector.x; 
        p_s2m_joysticks_scaffold->thumb_y = vector.y; 
#  endif
    }
    else
    {
#  ifdef USE_INDEX_STICKS
        joystick_get_components(&vector, joystick_state.config.centerIndex, joystick_state.config.mode_ri, INDEX_STICK_R_PIN_X, INDEX_STICK_R_PIN_Y, INDEX_STICK_R_FLIP_X, INDEX_STICK_R_FLIP_Y);
        p_s2m_joysticks_scaffold->index_x = vector.x;
        p_s2m_joysticks_scaffold->index_y = vector.y;
#  endif
#  ifdef USE_THUMB_STICKS
        joystick_get_components(&vector, joystick_state.config.centerThumb, joystick_state.config.mode_rt, THUMB_STICK_R_PIN_X, THUMB_STICK_R_PIN_Y, THUMB_STICK_R_FLIP_X, THUMB_STICK_R_FLIP_Y);
        p_s2m_joysticks_scaffold->thumb_x = vector.x; 
        p_s2m_joysticks_scaffold->thumb_y = vector.y; 
#  endif
    }

}

// executed by master: update joystick_state with data from slave
void joystick_update_raw(void)
{
    // copy data from the other side
    joystick_state.vector_slave_index.x = s2m_joysticks_scaffold.index_x;
    joystick_state.vector_slave_index.y = s2m_joysticks_scaffold.index_y;
    joystick_state.vector_slave_thumb.x = s2m_joysticks_scaffold.thumb_x;
    joystick_state.vector_slave_thumb.y = s2m_joysticks_scaffold.thumb_y;
}
#endif

// for manual calibration
void print_joysticks(void)
{
//    uprintf("master thumb (%u,%u)\n", analogReadPin(THUMB_STICK_L_PIN_X), analogReadPin(THUMB_STICK_L_PIN_Y));
//    uprintf("master index (%u,%u)\n", analogReadPin(INDEX_STICK_L_PIN_X), analogReadPin(INDEX_STICK_L_PIN_Y));
}

// update function vectors with new [x/y] components according to mode
void joystick_update_vectors(uint16_t x, uint16_t y, joystick_mode_t mode)
{
    if(mode != JOYSTICK_MODE_NONE)
        switch(mode)
        {
            case JOYSTICK_MODE_MOUSE:
                joystick_state.pointerVector.x += x;
                joystick_state.pointerVector.y += y;
                break;
            case JOYSTICK_MODE_SCROLL_1D:
            case JOYSTICK_MODE_SCROLL_2D:
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
    joystick_state.pointerVector.x  = joystick_state.pointerVector.x % joystick_state.config.speed;
    joystick_state.pointerVector.y  = joystick_state.pointerVector.y % joystick_state.config.speed;
    joystick_state.scrollVector.x   = joystick_state.scrollVector.x  % joystick_state.config.scroll_speed;
    joystick_state.scrollVector.y   = joystick_state.scrollVector.y  % joystick_state.config.scroll_speed;
}

// Get x/y components for joysticks on the master side by reading analog pins; get vectors for master/slave sticks
void joystick_get_vectors(void)
{
    joystick_vector_t vector;
    if(isLeftHand)
    {
#  ifdef USE_INDEX_STICKS
        // Get Master index stick value
        if(joystick_state.config.mode_li != JOYSTICK_MODE_NONE)
        {
            joystick_get_components(&vector, joystick_state.config.centerIndex, joystick_state.config.mode_li, INDEX_STICK_L_PIN_X, INDEX_STICK_L_PIN_Y, INDEX_STICK_L_FLIP_X, INDEX_STICK_L_FLIP_Y);
            joystick_update_vectors(vector.x, vector.y, joystick_state.config.mode_li);
        }
        // Get slave index stick value
        if(joystick_state.config.mode_ri != JOYSTICK_MODE_NONE)
            joystick_update_vectors(joystick_state.vector_slave_index.x, joystick_state.vector_slave_index.y, joystick_state.config.mode_ri);
#  endif
#  ifdef USE_THUMB_STICKS
        if(joystick_state.config.mode_lt != JOYSTICK_MODE_NONE)
        {
            joystick_get_components(&vector, joystick_state.config.centerThumb, joystick_state.config.mode_lt, THUMB_STICK_L_PIN_X, THUMB_STICK_L_PIN_Y, THUMB_STICK_L_FLIP_X, THUMB_STICK_L_FLIP_Y);
            joystick_update_vectors(vector.x, vector.y, joystick_state.config.mode_lt);
        }
        if(joystick_state.config.mode_rt != JOYSTICK_MODE_NONE)
            joystick_update_vectors(joystick_state.vector_slave_thumb.x, joystick_state.vector_slave_thumb.y, joystick_state.config.mode_rt);
#  endif
    }
    else
    {
#  ifdef USE_INDEX_STICKS
        if(joystick_state.config.mode_ri != JOYSTICK_MODE_NONE)
        {
            joystick_get_components(&vector, joystick_state.config.centerIndex, joystick_state.config.mode_ri, INDEX_STICK_R_PIN_X, INDEX_STICK_R_PIN_Y, INDEX_STICK_R_FLIP_X, INDEX_STICK_R_FLIP_Y);
            joystick_update_vectors(vector.x, vector.y, joystick_state.config.mode_ri);
        }
        if(joystick_state.config.mode_li != JOYSTICK_MODE_NONE)
            joystick_update_vectors(joystick_state.vector_slave_index.x, joystick_state.vector_slave_index.y, joystick_state.config.mode_li);
#  endif
#  ifdef USE_THUMB_STICKS
        if(joystick_state.config.mode_rt != JOYSTICK_MODE_NONE)
        {
            joystick_get_components(&vector, joystick_state.config.centerThumb, joystick_state.config.mode_rt, THUMB_STICK_R_PIN_X, THUMB_STICK_R_PIN_Y, THUMB_STICK_R_FLIP_X, THUMB_STICK_R_FLIP_Y);
            joystick_update_vectors(vector.x, vector.y, joystick_state.config.mode_rt);
        }
        if(joystick_state.config.mode_lt != JOYSTICK_MODE_NONE)
            joystick_update_vectors(joystick_state.vector_slave_thumb.x, joystick_state.vector_slave_thumb.y, joystick_state.config.mode_lt);
#  endif
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

// main function called from pointing_device_task()
void joystick_process(void)
{
    if (timer_elapsed(joystickTimer) > JOYSTICK_TIMEOUT)
    {
        joystickTimer = timer_read();
        // get pointer/scroll values to check that they move a bit (reset them if they don't, and force pointer/scroll movement if they are 0 and they start to change)
        joystick_vector_t pointer_vector_prev;
        joystick_vector_t scroll_vector_prev;
        memcpy(&pointer_vector_prev, &joystick_state.pointerVector, sizeof(joystick_state.pointerVector));
        memcpy(&scroll_vector_prev, &joystick_state.scrollVector, sizeof(joystick_state.scrollVector));
        joystick_clear_vectors();
        // update pointer/scroll vectors from master and slave stick value%
        joystick_get_vectors();
        // if poînter leaves a X=0 state, boost the value of X to force movement in case of small push of the stick (avoid unresponsive stick)
        if (joystick_state.pointerVector.x != 0 && pointer_vector_prev.x == 0)
            joystick_state.pointerVector.x = pointer_vector_prev.x > 0 ? joystick_state.pointerVector.x+joystick_state.config.speed : joystick_state.pointerVector.x-joystick_state.config.speed;
        // if pointer is stalling (all pointer sticks in dead zone in X), reset its movement value
        else if (joystick_state.pointerVector.x == pointer_vector_prev.x)
            joystick_state.pointerVector.x = 0;
        if (joystick_state.pointerVector.y != 0 && pointer_vector_prev.y == 0)
            joystick_state.pointerVector.y = pointer_vector_prev.y > 0 ? joystick_state.pointerVector.y+joystick_state.config.speed : joystick_state.pointerVector.y-joystick_state.config.speed;
        else if (joystick_state.pointerVector.y == pointer_vector_prev.y)
            joystick_state.pointerVector.y = 0;
        if (joystick_state.scrollVector.x != 0 && scroll_vector_prev.x == 0)
            joystick_state.scrollVector.x = scroll_vector_prev.x > 0 ? joystick_state.scrollVector.x+joystick_state.config.scroll_speed : joystick_state.scrollVector.x-joystick_state.config.scroll_speed;
        else if (joystick_state.scrollVector.x == scroll_vector_prev.x)
            joystick_state.scrollVector.x = 0;
        if (joystick_state.scrollVector.y != 0 && scroll_vector_prev.y == 0)
            joystick_state.scrollVector.y = scroll_vector_prev.y > 0 ? joystick_state.scrollVector.y+joystick_state.config.scroll_speed : joystick_state.scrollVector.y-joystick_state.config.scroll_speed;
        else if (joystick_state.scrollVector.y == scroll_vector_prev.y)
            joystick_state.scrollVector.y = 0;

        // Mouse report
        if(joystick_state.pointerVector.x/joystick_state.config.speed < -127)
            joystick_state.report.x = -127;
        else if(joystick_state.pointerVector.x/joystick_state.config.speed > 127)
            joystick_state.report.x = 127;
        else
            joystick_state.report.x = joystick_state.pointerVector.x / joystick_state.config.speed;

        if(joystick_state.pointerVector.y/joystick_state.config.speed < -127)
            joystick_state.report.y = -127;
        else if(joystick_state.pointerVector.y/joystick_state.config.speed > 127)
            joystick_state.report.y = 127;
        else
            joystick_state.report.y = joystick_state.pointerVector.y / joystick_state.config.speed;
        // Scroll report
        if(joystick_state.scrollVector.x < -127*joystick_state.config.scroll_speed)
            joystick_state.report.h = -127;
        else if(joystick_state.scrollVector.x > 127*joystick_state.config.scroll_speed)
            joystick_state.report.h = 127;
        else
            joystick_state.report.h = joystick_state.scrollVector.x / joystick_state.config.scroll_speed;

        if(joystick_state.scrollVector.y < -127*joystick_state.config.scroll_speed)
        {
//            uprintf("v-%d ", joystick_state.report.v);
            joystick_state.report.v = -127;
        }
        else if(joystick_state.scrollVector.y > 127*joystick_state.config.scroll_speed)
        {
//            uprintf("v+%d ", joystick_state.report.v);
            joystick_state.report.v = 127;
        }
        else
        {
            joystick_state.report.v = joystick_state.scrollVector.y / joystick_state.config.scroll_speed;
//            uprintf("v%d/%d ", joystick_state.report.v, joystick_state.scrollVector.y);
        }
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
    uint8_t tmp;
    report_mouse_t report = pointing_device_get_report();

    // Get stick values and convert them to a pointer/scrollwheel report
    joystick_process();
/*    if (joystick_state.report.h != 0)
        uprintf("h%d ", joystick_state.report.h);
    if (joystick_state.report.v != 0)
        uprintf("v%d ", joystick_state.report.v);
    else
        uprintf("v%d ", joystick_state.report.v);
    if (joystick_state.report.x != 0)
        uprintf("x%d ", joystick_state.report.x);
    if (joystick_state.report.y != 0)
        uprintf("y%d ", joystick_state.report.y);
        */
    // Set mouse pointer report
    report.x = joystick_state.report.x;
    report.y = joystick_state.report.y;
    // Set mouse scroll wheel report
    tmp = add_wheel_right;
    report.h = tmp;
    add_wheel_right -= tmp;
    tmp = add_wheel_left;
    report.h -= tmp;
    add_wheel_left -= tmp;
    report.h += joystick_state.report.h;
    tmp = add_wheel_up;
    report.v = tmp;
    add_wheel_up -= tmp;
    tmp = add_wheel_down;
    report.v -= tmp;
    add_wheel_down -= tmp;
    report.v += joystick_state.report.v;
    report.buttons = joystick_state.buttons;

    pointing_device_set_report(report);
    pointing_device_send();
}

#endif

