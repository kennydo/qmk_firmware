/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2020 Ploopy Corporation
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Tap Dance declarations
enum {
    TD_MOUSE_BTN5,
};

typedef enum {
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
} td_state_t;

// Create a global instance of the tapdance state type
static td_state_t td_state;

// Function to determine the current tapdance state
td_state_t cur_dance(tap_dance_state_t *state);

void mouse_btn5_td_finished(tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            tap_code(KC_BTN5);
            break;
        case TD_SINGLE_HOLD:
            layer_on(1);
            break;
        default:
            break;
    }
}

void mouse_btn5_td_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_HOLD:
            layer_off(1);
            break;
        default:
            break;
    }
}

// Tap Dance definitions
tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_MOUSE_BTN5] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mouse_btn5_td_finished, mouse_btn5_td_reset),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( /* Base */
        KC_BTN1, KC_BTN3, KC_BTN2,
          KC_BTN4, TD(TD_MOUSE_BTN5)
    ),
    [1] = LAYOUT(
        LCTL(KC_UP), KC_TRANSPARENT, KC_TRANSPARENT,
          KC_TRANSPARENT, KC_TRANSPARENT
    ),
};

td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        // Interrupted means some other button was pressed in the tapping term
        if (state->interrupted || !state->pressed) {
            return TD_SINGLE_TAP;
        } else {
            // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
            return TD_SINGLE_HOLD;
        }
    }

    return TD_UNKNOWN;
}