#ifndef _ANIM_PULSE_H
#define _ANIM_PULSE_H

#include "animation.h"  // for animation_t
#include "matrix.h"     // for matrix function
#include "systick.h"    // for millis()

// you can declare variables for your animation here
static uint8_t v, d;
static uint32_t last_update;

static void pulse_init(void) {
    // setup your animation
    v = 0;
    d = 1;
    last_update = millis();
}

static void pulse_tick() {
    // draw something to matrix
    // this code is allowed to be a little bit blocking
    // but must not take more than 50ms!
    // (otherwise the button will not properly work anymore)

    // non-blocking variant to do something every 100ms
    if (millis() - last_update > 100) {
        last_update = millis();

        if (d) v ++;
        if (!d) v --;
        if (v == MAX_BRIGHTNESS) d = 0;
        if (v == 0) d = 1;
    }

    for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < WIDTH; x++) {
            matrixSetPixel(x, y, v);
        }
    }
}

static void pulse_on_press(uint8_t count) {
    // handle $count button presses
}

static const animation_t pulse_animation = {
    .init = pulse_init,
    .tick = pulse_tick,
    .on_press = pulse_on_press
};

#endif