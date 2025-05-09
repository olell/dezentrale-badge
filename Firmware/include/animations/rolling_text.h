#ifndef _ANIM_ROLLING_TEXT_H
#define _ANIM_ROLLING_TEXT_H

#include "systick.h" // for millis()
#include "animation.h" // for animation_t
#include "matrix.h" // for matrix function
#include "font.h"
#include "stdio.h"
#include "string.h"

// you can declare variables for your animation here
static const uint8_t rolling_text_strings_count = 2;
static const char* rolling_text_strings[] = {
    "DEZENTRALE   ",
    "YOUR NAME?   ",
};
static uint8_t rolling_text_string_index = 0;
static const char* rolling_text_display_string;
static uint8_t rolling_text_display_string_length;

static uint8_t rolling_text_current_letter;
static uint8_t rolling_text_current_letter_col;

static uint8_t rolling_text_ring_led;


static void rolling_text_init(void) {
    // setup your animation
    rolling_text_display_string = rolling_text_strings[0];
    rolling_text_string_index = 0;
    rolling_text_display_string_length = strlen(rolling_text_display_string);
    

    rolling_text_current_letter = 0;
    rolling_text_current_letter_col = 0;
    rolling_text_ring_led = 0;
}

static void rolling_text_tick() {
    // draw something to matrix

    // shift out current pixels
    for (uint8_t x = 1; x < WIDTH; x ++) {
        for (uint8_t y = 0; y < HEIGHT - 1; y ++) {
            matrixSetPixel(x - 1, y, matrixGetPixel(x, y));
            matrixSetPixel(x, y, 0);
        }
    }

    for (uint8_t y = 0; y < 6; y++) {
        uint8_t byte = font[(rolling_text_display_string[rolling_text_current_letter] - 32) * 6 + y];
        uint8_t bit = byte >> (8 - rolling_text_current_letter_col);
        matrixSetPixel(5, y + 1, MAX_BRIGHTNESS * (bit & 1));
    }

    rolling_text_current_letter_col++;
    if (rolling_text_current_letter_col == 4) {
        rolling_text_current_letter_col = 0;
        rolling_text_current_letter = (rolling_text_current_letter + 1) % rolling_text_display_string_length;
    }

    if (rolling_text_current_letter_col % 2 == 0) {
        matrixSetPixel(rolling_text_ring_led, 6, 0);
        rolling_text_ring_led = (rolling_text_ring_led + 1) % 6;
        matrixSetPixel(rolling_text_ring_led, 6, MAX_BRIGHTNESS);
    }
}

static void rolling_text_on_press(uint8_t count) {
    // handle $count button presses
    if (count == 1) {
        rolling_text_string_index = (rolling_text_string_index + 1) % rolling_text_strings_count;
        rolling_text_display_string = rolling_text_strings[rolling_text_string_index];
        rolling_text_display_string_length = strlen(rolling_text_display_string);
        rolling_text_current_letter = 0;
        rolling_text_current_letter_col = 0;
        rolling_text_ring_led = 0;
    }
}

static const animation_t rolling_text_animation = {
    .init = rolling_text_init,
    .tick = rolling_text_tick,
    .on_press = rolling_text_on_press,
    .tick_interval = 200,
};

#endif