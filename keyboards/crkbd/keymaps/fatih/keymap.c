#include QMK_KEYBOARD_H

enum custom_keycodes {
   L_TH_1 = SAFE_RANGE, // layer tap hold
   LEAD
};

#define KC_VU KC_AUDIO_VOL_UP
#define KC_VD KC_AUDIO_VOL_DOWN

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_ESC ,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
 OSM(MOD_LSFT),   KC_Z ,    KC_X,    KC_C,    KC_V,    KC_B,                        KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, OSM(MOD_RCTL),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                     OSM(MOD_LGUI),  TT(1),  KC_SPC,     KC_ENT,   LEAD, OSM(MOD_RALT)
                                      //`--------------------------'  `--------------------------'
  ),

    [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_MINS, KC_PLUS, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        TO(0),    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_TILD,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
OSM(MOD_LSFT), KC_BSLS, XXXXXXX, KC_LPRN, KC_LBRC, KC_LCBR,                      KC_RCBR, KC_RBRC, KC_RPRN, KC_UNDS,  KC_EQL, KC_PIPE,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT,   MO(2), KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

    [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_F1  , KC_F2  , KC_F3  , KC_F4  , KC_F5  ,  KC_F6 ,                       KC_F7 ,  KC_F8 ,  KC_F9 , KC_F10 , KC_F11 ,  KC_F12,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        TO(0), XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       KC_LEFT,  KC_DOWN,  KC_UP,KC_RIGHT,XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, KC_BSLS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MPRV,  KC_VD ,   KC_VU, KC_MNXT, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,   MO(3),  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

    [3] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX , XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  )
};


uint8_t current_logo_state = 0;

void set_logo_state(uint8_t state) {
    current_logo_state = state;
}

uint8_t get_logo_state(void) {
    return current_logo_state;
}

// -------------------------- LEADER ------------------------------------

typedef struct {
    uint16_t sequence[5]; // Sequence of keycodes (max length 5)
    const char *output;   // String to send
} leader_sequence_t;

enum leader_actions {
  SET_JF_LOGO,
  SET_CRKBD_LOGO
};

#define ACTION_PREFIX ">"
#define GET_ACTION_STRING(action) ACTION_PREFIX #action

static const leader_sequence_t leader_sequences[] = {
    { {KC_G, KC_P, KC_NO}, "git pull" },
    { {KC_G, KC_S, KC_P, KC_A, KC_NO}, "git stash && git pull && git stash apply" },
    { {KC_Y, KC_NO}, SS_LGUI("a") SS_LGUI("c") },
    { {KC_J, KC_F, KC_NO}, GET_ACTION_STRING(SET_JF_LOGO) },
    { {KC_C, KC_R, KC_N, KC_NO}, GET_ACTION_STRING(SET_CRKBD_LOGO) },
};

static uint16_t leader_buffer[4];
static bool leader_active = false;
static uint8_t leader_pos = 0;

bool matches_prefix(const uint16_t *current_buffer, uint8_t current_length) {
    for (uint8_t i = 0; i < sizeof(leader_sequences) / sizeof(leader_sequences[0]); i++) {
        bool is_prefix = true;
        for (uint8_t j = 0; j < current_length; j++) {
            if (leader_sequences[i].sequence[j] != current_buffer[j]) {
                is_prefix = false;
                break;
            }
        }
        if (is_prefix) {
            return true;
        }
    }
    return false;
}

static bool held_key_pressed = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (leader_active) {
    if (record->event.pressed) {
      if (keycode == KC_ESC) {
        leader_active = false;
        leader_pos = 0;
        return false;
      }

      if (leader_pos < sizeof(leader_buffer) - 1) {
        leader_buffer[leader_pos++] = keycode;
        leader_buffer[leader_pos] = '\0';

        // Check if the buffer matches any prefix
        if (!matches_prefix(leader_buffer, leader_pos)) {
            leader_active = false;  // Exit leader mode
            leader_pos = 0;        // Reset buffer
            return false;
        }

        // Check for exact matches
        for (uint8_t i = 0; i < sizeof(leader_sequences) / sizeof(leader_sequences[0]); i++) {
            bool is_match = true;
            for (uint8_t j = 0; j < leader_pos; j++) {
                if (leader_sequences[i].sequence[j] != leader_buffer[j]) {
                    is_match = false;
                    break;
                }
            }

            // Match found and sequence is complete
            if (is_match && leader_sequences[i].sequence[leader_pos] == KC_NO) {
              if (leader_sequences[i].output[0] == ACTION_PREFIX[0]) {
                switch (atoi(leader_sequences[i].output + 1))
                {
                case SET_CRKBD_LOGO:
                  set_logo_state(0);
                SEND_STRING("ah"); // Send mapped string
                  break;
                case SET_JF_LOGO:
                  set_logo_state(1);
                SEND_STRING("oh"); // Send mapped string
                  break;
                default:
                  break;
                }
              } else {
                SEND_STRING(leader_sequences[i].output); // Send mapped string
              }
              leader_active = false;  // Exit leader mode
              leader_pos = 0;        // Reset buffer
              return false;
            }
        }
      }
    }
    return false;
  }

  if (keycode == LEAD && record->event.pressed) {
    leader_active = true;
    leader_pos = 0;
    return false;
  }

  switch (keycode) {
    case L_TH_1:
      if (record->event.pressed) {
        held_key_pressed = false;
        if (IS_LAYER_ON(1)) {
          layer_off(1);
        } else {
          layer_on(1);
        }
      } else {
        if (held_key_pressed) {
          layer_off(1);
        }
      }
      return false;
    default:
      if (record->event.pressed) {
        held_key_pressed = true;
      }
      return true;
  }
}

// void caps_word_set_user(bool active) {
//   if (active) {
//       // Do something when Caps Word activates.
//   } else {
//       // Do something when Caps Word deactivates.
//   }
// }


#ifdef OLED_ENABLE

oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    if (!is_keyboard_master()) {
        return OLED_ROTATION_180; // flips the display 180 degrees if offhand
    }
    return rotation;
}

static void oled_render_layer_state(void) {
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_ln_P(PSTR("Default"), false);
            break;
        case 1:
            oled_write_ln_P(PSTR("Lower"), false);
            break;
        case 2:
            oled_write_ln_P(PSTR("Raise"), false);
            break;
        case 3:
            oled_write_ln_P(PSTR("Adjust"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undef"), false);
            break;
    }
}

char     key_name = ' ';
uint16_t last_keycode;
uint8_t  last_row;
uint8_t  last_col;

static const char PROGMEM code_to_name[60] = {' ', ' ', ' ', ' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'R', 'E', 'B', 'T', '_', '-', '=', '[', ']', '\\', '#', ';', '\'', '`', ',', '.', '/', ' ', ' ', ' '};

static void set_keylog(uint16_t keycode, keyrecord_t *record) {
    // save the row and column (useful even if we can't find a keycode to show)
    last_row = record->event.key.row;
    last_col = record->event.key.col;

    key_name     = ' ';
    last_keycode = keycode;
    if (IS_QK_MOD_TAP(keycode)) {
        if (record->tap.count) {
            keycode = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
        } else {
            keycode = 0xE0 + biton(QK_MOD_TAP_GET_MODS(keycode) & 0xF) + biton(QK_MOD_TAP_GET_MODS(keycode) & 0x10);
        }
    } else if (IS_QK_LAYER_TAP(keycode) && record->tap.count) {
        keycode = QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
    } else if (IS_QK_MODS(keycode)) {
        keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);
    } else if (IS_QK_ONE_SHOT_MOD(keycode)) {
        keycode = 0xE0 + biton(QK_ONE_SHOT_MOD_GET_MODS(keycode) & 0xF) + biton(QK_ONE_SHOT_MOD_GET_MODS(keycode) & 0x10);
    }
    if (keycode > ARRAY_SIZE(code_to_name)) {
        return;
    }

    // update keylog
    key_name = pgm_read_byte(&code_to_name[keycode]);
}

static const char *depad_str(const char *depad_str, char depad_char) {
    while (*depad_str == depad_char)
        ++depad_str;
    return depad_str;
}

static void oled_render_keylog(void) {
    oled_write_char('0' + last_row, false);
    oled_write_P(PSTR("x"), false);
    oled_write_char('0' + last_col, false);
    oled_write_P(PSTR(", k"), false);
    const char *last_keycode_str = get_u16_str(last_keycode, ' ');
    oled_write(depad_str(last_keycode_str, ' '), false);
    oled_write_P(PSTR(":"), false);
    oled_write_char(key_name, false);
}

// static void render_bootmagic_status(bool status) {
//     /* Show Ctrl-Gui Swap options */
//     static const char PROGMEM logo[][2][3] = {
//         {{0x97, 0x98, 0}, {0xb7, 0xb8, 0}},
//         {{0x95, 0x96, 0}, {0xb5, 0xb6, 0}},
//     };
//     if (status) {
//         oled_write_ln_P(logo[0][0], false);
//         oled_write_ln_P(logo[0][1], false);
//     } else {
//         oled_write_ln_P(logo[1][0], false);
//         oled_write_ln_P(logo[1][1], false);
//     }
// }

void oled_render_logo(void) {
    // clang-format off
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};

    static const char PROGMEM jf_logo[] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,192,224,224,240,240,224,224,  0,  0,  0,128,192,192,192,192,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,192,192,192,192,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0, 96,248,252,254,255,255,255,127,191,223,239,247,251,252,254,255,255,255,127, 63, 31, 15,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,255,255,255,255,  0,128,192,224,240,240,240,240,240,240,240,224,224,192,  0,120,255,255,255,255,255,120,120,  0,120,120,255,255,255,255,123,123,123,128,192,224,240,240,240,240,240,240,240,240,224,192,  0,240,240,240,240,240,240,240,240,240,  0,240,240,240,240,240,240,240,240,240,240,224,224,240,240,240,240,240,240,224,128,  0,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,192,225,193,129,  1,  1, 30, 63, 63, 63, 63,191,223,239,247,251,253,254,254,254,126, 60,  0,  0,  0,  0,  0, 24,120,248,248,248,224,224,224,224,255,255,255,127, 31,  4, 63,255,255,255,241,224,192,224,225,255,255,255,127, 31,  0, 31,255,255,255,255,192,192,  0,  0,  0,255,255,255,255,  0,  0,  0, 63,255,255,255,241,224,192,224,224,255,255,255,127, 31,255,255,255,255,255,  1,  1,  0,  1,  0,255,255,255,255,  1,  0,  0,255,255,255,255,255,  0,  0,  0,255,255,255,255,255,  0,  0,  0,  0,  0, 
        0,  0,  0,  0,  0,  7,  7, 15, 15, 15, 14,  4,  0,  0,  0,  0,  1,  7,  7, 15, 15,  7,  3,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  3,  3,  3,  3,  1,  1,  0,  0,  0,  0,  0,  0,  0,  1,  3,  3,  3,  3,  3,  1,  1,  0,  0,  0,  0,  0,  0,  1,  3,  3,  3,  3,  0,  0,  0,  3,  3,  3,  3,  0,  0,  0,  0,  0,  0,  1,  1,  3,  3,  3,  3,  1,  1,  0,  0,  0,  3,  3,  3,  3,  3,  0,  0,  0,  0,  0,  3,  3,  3,  3,  0,  0,  0,  1,  3,  3,  3,  3,  0,  0,  0,  3,  3,  3,  3,  3,  0,  0,  0,  0,  0,
    };

    // clang-format on
    if (current_logo_state == 0) {
        oled_write_P(crkbd_logo, false);
    } else {
        oled_write_raw_P(jf_logo, sizeof(jf_logo));
    }
}

bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }
    if (is_keyboard_master()) {
        oled_render_layer_state();
        oled_render_keylog();
    } else {
        oled_render_logo();
    }
    return false;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        set_keylog(keycode, record);
    }
    return process_record_user(keycode, record);
}
#endif // OLED_ENABLE
