//
// Keymap intended for US-Intl OS layout, allows using dead keys
//

#include QMK_KEYBOARD_H

// BASE LAYOUT: QWERTY OR COLEMAK-DH? Keep one of these 2 lines commented
#define COLEMAK_DH_SCAFFOLD
//#define QWERTY_SCAFFOLD

// These 2 are for the OLED display of your keymap. Hide bottom row or 2nd index column by commenting or deleting one of these lines
#define HAS_ADD_BOTTOM_ROW
#define HAS_ADD_INDEX_COL

#ifdef JOYSTICK_SCAFFOLD_ENABLE
#    include "joystick_scaffold.h"
#endif

// mod-taps with 16b keycodes (code in keycode processing function)
#define GUI_PLUS LGUI_T(KC_F18)
#define GUI_COLN LGUI_T(KC_F13)
#define GUI_UNDS LGUI_T(KC_F14)
#define CTL_LTHN LCTL_T(KC_F15)
#define ALT_GTHN LALT_T(KC_F16)

// normal mod-taps
#define KC_GUIZ LGUI_T(KC_Z)
#define GUI_INS LGUI_T(KC_INS)
#define KC_ALTR LALT_T(KC_R)
#define KC_RAMN RALT_T(KC_MINS)
#define KC_LSHS LSFT_T(KC_S)
#define KC_LSHD LSFT_T(KC_D)
#define KC_LSHK LSFT_T(KC_K)
#define KC_CTLT LCTL_T(KC_T)
#define KC_CTLF LCTL_T(KC_F)
#define KC_CTLJ LCTL_T(KC_J)
#define KC_CTLN LCTL_T(KC_N)
#define KC_LSHE LSFT_T(KC_E)
#define KC_ALTI LALT_T(KC_I)
#define KC_ALTS LALT_T(KC_S)
#define KC_ALTL LALT_T(KC_L)

// special characters requiring AltGr
#define KC_EUR  RALT(KC_5)      // €
#define KC_GBP  S(RALT(KC_4))   // £
#define KC_YEN  RALT(KC_4)      // yen sign on windows
#define KC_DEGR S(RALT(KC_SCLN)) // °

// Mouse clicks
#define KC_RCLK KC_F13 // TBD - mouse clicks
#define KC_MCLK KC_F14 // TBD - mouse clicks
#define KC_LCLK KC_F15 // TBD - mouse clicks

enum layers {
    _DEF_LAYER = 0,
    _LOWER,
    _RAISE,
    _ADJUST,
    _MOUSE
};

enum custom_keycodes {
  ND_QUOT= SAFE_RANGE,              // ' - not a dead key
  ND_DQT,                           // " - not a dead key
  ND_CIRC,                          // ^ - not a dead key
  ND_GRV,                           // ` - not a dead key
  ND_TILD,                          // ~ - not a dead key
  MSE_O      = LT(_MOUSE, KC_O),    // mod-tap: tap='o', hold=Mouse layer
  MSE_SCLN   = LT(_MOUSE, KC_SCLN), // mod-tap: tap=';', hold=Mouse layer
  RAISE_BSPC = LT(_RAISE, KC_BSPC), // mod-tap: tap=backspace, hold=raise
  LOWER_TAB  = LT(_LOWER, KC_TAB)   // mod-tap: tap=tab,       hold=lower
};

enum combos {
    L_BSPC,
    R_ESC,
    L_ESC,
    R_DEL,
    L_ENT,
    R_ENT,
    C_CED,
    QPG_QUIT,
    ZD_CUT,
    ZC_COPY,
    XD_PASTE
};

#ifdef COLEMAK_DH_SCAFFOLD
    const uint16_t PROGMEM wf_combo[]    = {KC_W, KC_F,       COMBO_END};
    const uint16_t PROGMEM uy_combo[]    = {KC_U, KC_Y,       COMBO_END};
    const uint16_t PROGMEM fp_combo[]    = {KC_F, KC_P,       COMBO_END};
    const uint16_t PROGMEM lu_combo[]    = {KC_L, KC_U,       COMBO_END};
    const uint16_t PROGMEM cd_combo[]    = {KC_C, KC_D,       COMBO_END};
    const uint16_t PROGMEM hk_combo[]    = {KC_H, KC_K,       COMBO_END};
    const uint16_t PROGMEM cc_combo[]    = {KC_K, KC_C,       COMBO_END};
    const uint16_t PROGMEM altf4_combo[] = {KC_Q, KC_P, KC_G, COMBO_END};
    const uint16_t PROGMEM cut_combo[]   = {KC_Z, KC_D,       COMBO_END};
    const uint16_t PROGMEM copy_combo[]  = {KC_Z, KC_C,       COMBO_END};
    const uint16_t PROGMEM paste_combo[] = {KC_X, KC_D,       COMBO_END};
#endif
#ifdef QWERTY_SCAFFOLD
    const uint16_t PROGMEM wf_combo[]    = {KC_W, KC_E,       COMBO_END};
    const uint16_t PROGMEM uy_combo[]    = {KC_I, KC_O,       COMBO_END};
    const uint16_t PROGMEM fp_combo[]    = {KC_E, KC_R,       COMBO_END};
    const uint16_t PROGMEM lu_combo[]    = {KC_U, KC_I,       COMBO_END};
    const uint16_t PROGMEM cd_combo[]    = {KC_C, KC_V,       COMBO_END};
    const uint16_t PROGMEM hk_combo[]    = {KC_N, KC_M,       COMBO_END};
    const uint16_t PROGMEM cc_combo[]    = {KC_N, KC_C,       COMBO_END};
    const uint16_t PROGMEM altf4_combo[] = {KC_Q, KC_R, KC_G, COMBO_END};
    const uint16_t PROGMEM cut_combo[]   = {KC_Z, KC_V,       COMBO_END};
    const uint16_t PROGMEM copy_combo[]  = {KC_Z, KC_C,       COMBO_END};
    const uint16_t PROGMEM paste_combo[] = {KC_X, KC_V,       COMBO_END};
#endif

    combo_t key_combos[COMBO_COUNT] = {
        // basic keycodes
        [L_BSPC]   = COMBO(fp_combo,    KC_BSPC),
        [L_ESC]    = COMBO(wf_combo,    KC_ESC),
        [L_ENT]    = COMBO(cd_combo,    KC_ENT),
        [R_ESC]    = COMBO(uy_combo,    KC_ESC),
        [R_DEL]    = COMBO(lu_combo,    KC_DEL),
        [R_ENT]    = COMBO(hk_combo,    KC_ENT),
        [C_CED]    = COMBO(cc_combo,    RALT(KC_COMM)),
        // special actions
        [QPG_QUIT] = COMBO_ACTION(altf4_combo),
        [ZD_CUT]   = COMBO_ACTION(cut_combo),
        [ZC_COPY]  = COMBO_ACTION(copy_combo),
        [XD_PASTE] = COMBO_ACTION(paste_combo),
    };

    // special actions triggered by combos
    void process_combo_event(uint16_t combo_index, bool pressed) {
        switch(combo_index) {
            case QPG_QUIT:
              if (pressed) {
                register_code(KC_LALT);
                tap_code(KC_F4);
                unregister_code(KC_LALT);
              }
              break;
            case ZD_CUT:
              if (pressed)
                tap_code16(LCTL(KC_X));
              break;
            case ZC_COPY:
              if (pressed)
                tap_code16(LCTL(KC_C));
              break;
            case XD_PASTE:
              if (pressed)
                tap_code16(LCTL(KC_V));
              break;
        }
    }

    const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] =
    {
    // Base Layer
      [_DEF_LAYER] = LAYOUT(
#ifdef COLEMAK_DH_SCAFFOLD
        KC_Q,    KC_W,    KC_F,    KC_P,       KC_B,       KC_J,      KC_L,    KC_U,    KC_Y,    KC_GRV,
        KC_A,    KC_ALTR, KC_LSHS, KC_CTLT,    KC_G,       KC_M,      KC_CTLN, KC_LSHE, KC_ALTI, MSE_O,
        KC_GUIZ, KC_X,    KC_C,    KC_D,       KC_V,       KC_K,      KC_H,    KC_K,    KC_J,    KC_QUOT,
                 KC_B,    KC_G,    KC_V,                              KC_M,    KC_COMM, KC_DOT, 
#endif
#ifdef QWERTY_SCAFFOLD
// QWERTY
        KC_Q,    KC_W,    KC_E,    KC_R,       KC_T,       KC_Y,       KC_U,          KC_I,    KC_O,    KC_P,
        KC_A,    KC_ALTS, KC_LSHD, KC_CTLF,    KC_G,       KC_H,       KC_CTLJ,       KC_LSHK, KC_ALTL, MSE_SCLN,
        KC_GUIZ, KC_X,    KC_C,    KC_V,       KC_B,       KC_N,       KC_M,          KC_COMM, KC_DOT,  KC_SLSH,
                 KC_T,    KC_G,    KC_B,                               KC_N,          KC_H,    KC_Y, 
#endif
                          KC_NO,   KC_SPACE,  LOWER_TAB,   RAISE_BSPC, OSM(MOD_LSFT), KC_NO
      ),

    // Lower layer: num pad, symbols, navigation, {}
      [_LOWER] = LAYOUT(
        KC_MINS,  KC_7,         KC_8,         KC_9,          KC_0,    ND_CIRC,       KC_CIRC,  KC_PIPE,        KC_SLSH,  KC_UNDS,
        GUI_PLUS, LALT_T(KC_4), LSFT_T(KC_5), LCTL_T(KC_6),  KC_ASTR, S(RALT(KC_4)), CTL_LTHN, LSFT_T(KC_EQL), ALT_GTHN, KC_RAMN,
        KC_SLSH,  KC_1,         KC_2,         KC_3,          KC_DOT,  KC_NO,         KC_LCBR,  KC_CIRC,        KC_RCBR,  KC_ENT,
                  KC_ASTR,      KC_0,         KC_DOT,                                KC_ASTR,  KC_SCLN,        KC_COLN, 
                                KC_TRNS,      KC_TRNS,       KC_TRNS, KC_TRNS,       KC_TRNS,  KC_TRNS
      ),

    // raise layer: symbols, navigation, ()
      [_RAISE] = LAYOUT(
        KC_EXLM,  KC_AT,    KC_HASH, KC_DLR,  ND_CIRC,   ND_DQT,  KC_BSLS,  KC_PIPE, KC_SLSH, KC_UNDS,
        KC_QUES,  KC_AMPR,  KC_PERC, KC_EUR,  ND_TILD,   ND_QUOT, KC_LEFT,  KC_UP,   KC_RGHT, KC_RAMN,
        GUI_COLN, KC_ASTR,  KC_CIRC, KC_GBP,  ND_GRV,    KC_COLN, KC_LPRN,  KC_DOWN, KC_RPRN, KC_ENT,
                  KC_TILD,  KC_DEGR, KC_YEN,                      KC_ASTR,  KC_SCLN, KC_COLN,
                            KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS,  KC_TRNS
      ),

    // adjust layer: function keys, navigation, []
      [_ADJUST] = LAYOUT(
        KC_PAUS,  KC_F7,         KC_F8,         KC_F9,         KC_F10,    KC_NO,   KC_H,            KC_J,            KC_K,           KC_L,
        KC_PSCR,  LALT_T(KC_F4), LSFT_T(KC_F5), LCTL_T(KC_F6), KC_F11,    KC_NO,   LCTL_T(KC_HOME), LSFT_T(KC_PGUP), LALT_T(KC_END), KC_RAMN,
        GUI_INS,  KC_F1,         KC_F2,         KC_F3,         KC_F12,    KC_NO,   KC_LBRC,         KC_PGDN,         KC_RBRC,        KC_ENT,
                  KC_F10,        KC_F11,        KC_F12,                            KC_ASTR,         KC_SCLN,         KC_COLN,
                                 KC_TRNS,       KC_TRNS,       KC_TRNS,   KC_TRNS, KC_TRNS,         KC_TRNS
      ),

    // mouse layer: only mouse buttons
      [_MOUSE] = LAYOUT(
        KC_TRNS,  KC_F15,   KC_F14,   KC_F13,   KC_TRNS,           KC_TRNS,  KC_F13,   KC_F14,   KC_F15,   KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,           KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
        KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,           KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,  KC_TRNS,
                  KC_TRNS,  KC_TRNS,  KC_TRNS,                               KC_TRNS,  KC_TRNS,  KC_TRNS,
                                 KC_TRNS, KC_TRNS, KC_TRNS,     KC_TRNS, KC_TRNS, KC_TRNS
      )
    };

    bool process_record_user(uint16_t keycode, keyrecord_t *record) {
      switch (keycode) {
        case LOWER_TAB:
              if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
              } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
              }
              return true;
        case RAISE_BSPC:
              if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
              } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
              }
              return true;
        case ND_QUOT:
              if (record->event.pressed) {
                SEND_STRING("' ");
              }
              break;
        case ND_DQT:
              if (record->event.pressed) {
                SEND_STRING("\" ");
              }
              break;
        case ND_CIRC:
              if (record->event.pressed) {
                SEND_STRING("^ ");
              }
              break;
        case ND_GRV:
              if (record->event.pressed) {
                SEND_STRING("` ");
              }
              break;
        case ND_TILD:
              if (record->event.pressed) {
                SEND_STRING("~ ");
              }
              break;
        case GUI_COLN:
              if (record->tap.count > 0) {
                if (record->event.pressed) {
                  tap_code16(KC_COLN);
                }
                return false;
              }
              break;
        case GUI_UNDS:
              if (record->tap.count > 0) {
                if (record->event.pressed) {
                  tap_code16(KC_UNDS);
                }
                return false;
              }
              break;
        case CTL_LTHN:
              if (record->tap.count > 0) {
                if (record->event.pressed) {
                  tap_code16(KC_LT);
                }
                return false;
              }
              break;
        case GUI_PLUS:
              if (record->tap.count > 0) {
                if (record->event.pressed) {
                  tap_code16(KC_PLUS);
                }
                return false;
              }
              break;
        case ALT_GTHN:
              if (record->tap.count > 0) {
                if (record->event.pressed) {
                  tap_code16(KC_GT);
                }
                return false;
              }
              break;
        case KC_F13:
            if (record->event.pressed)
                joystick_press_buttons(MOUSE_BTN1);
            else
                joystick_release_buttons(MOUSE_BTN1);
            return false;
        case KC_F14:
            if (record->event.pressed)
                joystick_press_buttons(MOUSE_BTN2);
            else
                joystick_release_buttons(MOUSE_BTN2);
            return false;
        case KC_F15:
            if (record->event.pressed)
                joystick_press_buttons(MOUSE_BTN3);
            else
                joystick_release_buttons(MOUSE_BTN3);
            return false;
          }
        return true;
    };

    layer_state_t layer_state_set_user(layer_state_t state) {
        return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
    }

#ifdef OLED_DRIVER_ENABLE


static const char PROGMEM charF1Media24x16[24*19] = {
    // ch_f1f12.png
    0xff, 0xff, 0xc3, 0xc3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0xff, 0x80, 0x00, 0x00, 
    0xff, 0xff, 0xc3, 0xc3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc2, 0xb1, 0x8e, 0x00, 0x00, 
    0xff, 0xff, 0xc3, 0xc3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x89, 0x76, 0x00, 0x00, 
    0xff, 0xff, 0xc3, 0xc3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x38, 0x24, 0xff, 0x20, 0x00, 0x00, 
    0xff, 0xff, 0xc3, 0xc3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0x89, 0x71, 0x00, 0x00, 
    0xff, 0xff, 0xc3, 0xc3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7c, 0x92, 0x61, 0x00, 0x00, 
    0xff, 0xff, 0xc3, 0xc3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1, 0x39, 0x07, 0x00, 0x00, 
    0xff, 0xff, 0xc3, 0xc3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x89, 0x76, 0x00, 0x00, 
    0xff, 0xff, 0xc3, 0xc3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 
    0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x89, 0x7e, 0x00, 0x00, 
    0xff, 0xff, 0xc3, 0xc3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 
    0xff, 0xff, 0x00, 0x00, 0x82, 0xff, 0x80, 0x00, 0x7e, 0x81, 0x7e, 0x00, 
    0xff, 0xff, 0xc3, 0xc3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 
    0xff, 0xff, 0x00, 0x00, 0x82, 0xff, 0x80, 0x00, 0x82, 0xff, 0x80, 0x00, 
    0xff, 0xff, 0xc3, 0xc3, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x00, 0x00, 
    0xff, 0xff, 0x00, 0x00, 0x82, 0xff, 0x80, 0x00, 0xc2, 0xb1, 0x8e, 0x00, 
    // ch_media.png
    // previous track
    // play/pause
    // next track
    // stop
    // volume+
    // volume-
    // mute
    0x00, 0x80, 0xc0, 0xe0, 0xf0, 0x00, 0x00, 0x80, 0xc0, 0xe0, 0xf0, 0x00, 
    0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x00, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x00, 
    0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0xe0, 0xe0, 0x00, 0xe0, 0xe0, 0x00, 
    0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x00, 0x1f, 0x1f, 0x00, 0x1f, 0x1f, 0x00, 
    0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 0xf0, 0xe0, 0xc0, 0x80, 0x00, 0x00, 
    0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x00, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x00, 
    0x00, 0x00, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0x00, 0x00, 
    0x00, 0x00, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x00, 0x00, 
    0xe0, 0xe0, 0xe0, 0x70, 0xb8, 0x9c, 0xee, 0xef, 0x80, 0x80, 0x00, 0x00, 
    0x07, 0x07, 0x07, 0x0e, 0x1d, 0x39, 0x77, 0xf7, 0x01, 0x01, 0x00, 0x00, 
    0xe0, 0xe0, 0xe0, 0x70, 0xb8, 0xbc, 0xbe, 0xbf, 0x80, 0x80, 0x00, 0x00, 
    0x07, 0x07, 0x07, 0x0e, 0x1d, 0x3d, 0x7d, 0xfd, 0x01, 0x01, 0x00, 0x00, 
    0xe0, 0xe0, 0xe0, 0xf0, 0xb8, 0x5c, 0xee, 0xdf, 0xc0, 0xe0, 0x40, 0x00, 
    0x07, 0x07, 0x07, 0x0f, 0x1d, 0x3a, 0x77, 0xfb, 0x03, 0x07, 0x02, 0x00
};

// print 12x16 character
void print16b(char c, int x, int y, bool add4BlankPix)
{
    if(c < ' ' || c > '~')
    {
        // Character c can't be displayed, out of the table
        return;
    }
    else
    {
        // Display top half of the character
        oled_set_cursor(x, y);
        if(add4BlankPix)
            oled_write_raw_P(charF1Media24x16, 4);
        oled_write_raw_P(charF1Media24x16+(c-' ')*24, 12);
        // Display bottom half of the character
        oled_set_cursor(x, y+1);
        if(add4BlankPix)
            oled_write_raw_P(charF1Media24x16, 4);
        oled_write_raw_P(charF1Media24x16+(c-' ')*24+12, 12);
    }
}

// print 12x16 character (F1-F12 and media keys)
void print16bFMedia(int c, int x, int y, bool add4BlankPix)
{
    if(c < 1 || c > 19)
    {
        // Character c can't be displayed, out of the table
        return;
    }
    else
    {
        // Display top half of the character
        oled_set_cursor(x, y);
        if(add4BlankPix)
            oled_write_raw_P(charF1Media24x16, 4);
        oled_write_raw_P(charF1Media24x16+(c-1)*24, 12);
        // Display bottom half of the character
        oled_set_cursor(x, y+1);
        if(add4BlankPix)
            oled_write_raw_P(charF1Media24x16, 4);
        oled_write_raw_P(charF1Media24x16+(c-1)*24+12, 12);
    }
}


oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_0;
}

// override core function: makes the slave side process keypresses, so we then know what layer to display on the oled
bool should_process_keypress(void) { return true; }

static void render_status(void) {
return;
    switch (get_highest_layer(layer_state)) {
        case _DEF_LAYER:
            if(is_keyboard_left())
            {
#ifdef COLEMAK_DH_SCAFFOLD
                print16b('Q', 0, 0, false);
                print16b('W', 2, 0, false);
                print16b('F', 4, 0, false);
                print16b('P', 6, 0, false);
#   ifdef HAS_ADD_INDEX_COL
                print16b('B', 8, 0, false);
#   endif
                //
                print16b('A', 0, 2, false);
                print16b('R', 2, 2, false);
                print16b('S', 4, 2, false);
                print16b('T', 6, 2, false);
#   ifdef HAS_ADD_INDEX_COL    
                print16b('G', 8, 2, false);
#   endif
                //
                print16b('Z', 0, 4, false);
                print16b('X', 2, 4, false);
                print16b('C', 4, 4, false);
                print16b('D', 6, 4, false);
#   ifdef HAS_ADD_INDEX_COL
                print16b('V', 8, 4, false);
#   endif
#   ifdef HAS_ADD_BOTTOM_ROW
                print16b('B', 2, 6, false);
                print16b('G', 4, 6, false);
                print16b('V', 6, 6, false);
#   endif

//                print16b('s', 10, 6, true);
//                print16b('s', 12, 6, true);
//                print16b(' ', 19, 6, true);
#endif
#ifdef QWERTY_SCAFFOLD
                print16b('Q', 0, 0, false);
                print16b('W', 3, 0, false);
                print16b('E', 6, 0, false);
                print16b('R', 9, 0, false);
#   ifdef HAS_ADD_INDEX_COL
                print16b('T', 12, 0, false);
#   endif
                //
                print16b('A', 0, 2, false);
                print16b('S', 3, 2, false);
                print16b('D', 6, 2, false);
                print16b('F', 9, 2, false);
#   ifdef HAS_ADD_INDEX_COL
                print16b('G', 12, 2, false);
#   endif
                //
                print16b('Z', 0, 4, false);
                print16b('X', 3, 4, false);
                print16b('C', 6, 4, false);
                print16b('V', 9, 4, false);
#   ifdef HAS_ADD_INDEX_COL
                print16b('B', 12, 4, false);
#   endif

//               print16b('s', 16, 4, true);
//               print16b(' ', 19, 4, true);
#endif
            }
            else
            {
                print16b('Y', 7, 0, false);
                print16b('U', 10, 0, false);
                print16b('I', 13, 0, false);
                print16b('O', 16, 0, false);
                print16b('P', 19, 0, false);
                //
                print16b('H', 7, 3, false);
                print16b('J', 10, 3, false);
                print16b('K', 13, 3, false);
                print16b('L', 16, 3, false);
                print16b(':', 19, 3, false);
                //
                print16b(' ', 0, 6, false);
                print16b('v', 3, 6, false);
                print16b('N', 7, 6, false);
                print16b('M', 10, 6, false);
                print16b(',', 13, 6, false);
                print16b('.', 16, 6, false);
                print16b('/', 19, 6, false);
            }
            break;
        case _LOWER:
            if(is_keyboard_left())
            {
                print16b('-', 0, 0, false);
                print16b('7', 3, 0, false);
                print16b('8', 6, 0, false);
                print16b('9', 9, 0, false);
                print16b('/', 12, 0, false);
                //
                print16b('+', 0, 3, false);
                print16b('4', 3, 3, false);
                print16b('5', 6, 3, false);
                print16b('6', 9, 3, false);
                print16b('*', 12, 3, false);
                //
                print16b('0', 0, 6, false);
                print16b('1', 3, 6, false);
                print16b('2', 6, 6, false);
                print16b('3', 9, 6, false);
                print16b('n', 12, 6, false);
                print16b('s', 16, 6, false);
                print16b(' ', 19, 6, false);
            }
            else
            {
                print16b('^', 7, 0, false);
                print16b('^', 10, 0, false);
                print16b('&', 13, 0, false);
                print16b('*', 16, 0, false);
                print16b('%', 19, 0, false);
                //
                print16b('b', 7, 3, false);
                print16b('H', 10, 3, false);
                print16b('K', 13, 3, false);
                print16b('L', 16, 3, false);
                print16b('a', 19, 3, false);
                //
                print16b(' ', 0, 6, false);
                print16b('v', 3, 6, false);
                print16b('*', 7, 6, false);
                print16b('{', 10, 6, false);
                print16b('J', 13, 6, false);
                print16b('}', 16, 6, false);
                print16b('O', 19, 6, false);
            }
            break;
        case _RAISE:
            if(is_keyboard_left())
            {
                print16b('!', 0, 0, false);
                print16b('@', 3, 0, false);
                print16b('#', 6, 0, false);
                print16b('$', 9, 0, false);
                print16b('^', 12, 0, false);
                //
                print16b('?', 0, 3, false);
                print16b('<', 3, 3, false);
                print16b('=', 6, 3, false);
                 print16b('>', 9, 3, false);
                print16b('~', 12, 3, false);
                //
                print16b(':', 0, 6, false);
                print16b('.', 3, 6, false);
                print16b(',', 6, 6, false);
                print16b(';', 9, 6, false);
                print16b('`', 12, 6, false);
                print16b('s', 16, 6, false);
                print16b(' ', 19, 6, false);
            }
            else
            {
                print16b('"', 7, 0, false);
                print16b('\\', 10, 0, false);
                print16b('|', 13, 0, false);
                print16b('/', 16, 0, false);
                print16b('-', 19, 0, false);
                //
                print16b('\'', 7, 3, false);
                print16b('d', 10, 3, false);
                print16b('f', 13, 3, false);
                print16b('g', 16, 3, false);
                print16b('_', 19, 3, false);
                //
                print16b(' ', 0, 6, false);
                print16b('v', 3, 6, false);
               // print16b(''', 7, 6, false);
                print16b('(', 10, 6, false);
                print16b('e', 13, 6, false);
                print16b(')', 16, 6, false);
                print16b(';', 19, 6, false);
            }
            break;
        case _ADJUST:
            if(is_keyboard_left())
            {
                print16b('y', 0, 0, false);
                print16bFMedia(7, 3, 0, false);
                print16bFMedia(8, 6, 0, false);
                print16bFMedia(9, 9, 0, false);
                print16bFMedia(17, 12, 0, false);
                //
                print16b('w', 0, 3, false);
                print16bFMedia(4, 3, 3, false);
                print16bFMedia(5, 6, 3, false);
                print16bFMedia(6, 9, 3, false);
                print16bFMedia(19, 12, 3, false);
                //
                print16b('x', 0, 6, false);
                print16bFMedia(1, 3, 6, false);
                print16bFMedia(2, 6, 6, false);
                print16bFMedia(3, 9, 6, false);
                print16bFMedia(18, 12, 6, false);
                print16b('s', 16, 6, false);
                print16b(' ', 19, 6, false);
            }
            else
            {
                //prev/f10/f11/f12/caps lock
                print16bFMedia(13, 7, 0, false);
                print16bFMedia(10, 10, 0, false);
                print16bFMedia(11, 13, 0, false);
                print16bFMedia(12, 16, 0, false);
                print16b('z', 19, 0, false);
                //play/home/pgUp/end/stop
                print16bFMedia(14, 7, 3, false);
                print16b('h', 10, 3, false);
                print16b('j', 13, 3, false);
                print16b('k', 16, 3, false);
                print16bFMedia(16, 19, 3, false);
                //backspace/enter/next/[/pgDown/]/nothing
                print16b(' ', 0, 6, false);
                print16b('v', 3, 6, false);
                print16bFMedia(15, 7, 6, false);
                print16b('[', 10, 6, false);
                print16b('i', 13, 6, false);
                print16b(']', 16, 6, false);
             //   print16b(' ', 19, 6, false);
            }
            break;
        default:
            break;
    }

}

void oled_task_user(void) {
    render_status();
}
#endif

