#ifndef _ANIM_ROLLING_TEXT_H
#define _ANIM_ROLLING_TEXT_H

#include "systick.h" // for millis()
#include "animation.h" // for animation_t
#include "matrix.h" // for matrix function
#include "font.h"
#include "stdio.h"

// you can declare variables for your animation here
static const char* display_string = "DEZENTRALE   ";
static const uint8_t display_string_length = 13;

static uint8_t current_letter;
static uint8_t current_letter_col;
static uint32_t last_update;

static uint8_t ring_led;


static void rolling_text_init(void) {
    // setup your animation
    last_update = millis();

    current_letter = 0;
    current_letter_col = 0;

    ring_led = 0;
}

static void rolling_text_tick() {
    // draw something to matrix
    // this code is allowed to be a little bit blocking
    // but must not take more than 50ms!
    // (otherwise the button will not properly work anymore)

    if (millis() - last_update > 200) {
        last_update = millis();

        // shift out current pixels
        for (uint8_t x = 1; x < WIDTH; x ++) {
            for (uint8_t y = 0; y < HEIGHT - 1; y ++) {
                matrixSetPixel(x - 1, y, matrixGetPixel(x, y));
                matrixSetPixel(x, y, 0);
            }
        }

        for (uint8_t y = 0; y < 6; y++) {
            uint8_t byte = font[(display_string[current_letter] - 32) * 6 + y];
            uint8_t bit = byte >> (8 - current_letter_col);
            matrixSetPixel(5, y + 1, MAX_BRIGHTNESS * (bit & 1));
        }

        current_letter_col++;
        if (current_letter_col == 4) {
            current_letter_col = 0;
            current_letter = (current_letter + 1) % display_string_length;
        }

        if (current_letter_col % 2 == 0) {
            matrixSetPixel(ring_led, 6, 0);
            ring_led = (ring_led + 1) % 6;
            matrixSetPixel(ring_led, 6, MAX_BRIGHTNESS);
        }

    }

}

static void rolling_text_on_press(uint8_t count) {
    // handle $count button presses
}

static const animation_t rolling_text_animation = {
    .init = rolling_text_init,
    .tick = rolling_text_tick,
    .on_press = rolling_text_on_press
};

#endif