#ifndef _ANIM_PULSE_H
#define _ANIM_PULSE_H

#include "animation.h"  // for animation_t
#include "matrix.h"     // for matrix function
#include "systick.h"    // for millis()

// you can declare variables for your animation here
static uint8_t pulse_v, pulse_d;

static void pulse_init(void) {
    // setup your animation
    pulse_v = 0;
    pulse_d = 1;
}

static void pulse_tick() {
    // draw something to matrix

    if (pulse_d) pulse_v ++;
    if (!pulse_d) pulse_v --;
    if (pulse_v == MAX_BRIGHTNESS) pulse_d = 0;
    if (pulse_v == 0) pulse_d = 1;

    for (uint8_t y = 0; y < HEIGHT; y++) {
        for (uint8_t x = 0; x < WIDTH; x++) {
            matrixSetPixel(x, y, pulse_v);
        }
    }
}

static void pulse_on_press(uint8_t count) {
    // handle $count button presses
}

static const animation_t pulse_animation = {
    .init = pulse_init,
    .tick = pulse_tick,
    .on_press = pulse_on_press,
    .tick_interval = 100
};

#endif