#ifndef _ANIM_CYCLE_H
#define _ANIM_CYCLE_H

#include "systick.h" // for millis()
#include "animation.h" // for animation_t
#include "matrix.h" // for matrix function

// you can declare variables for your animation here
static uint8_t x, y, v;
static uint32_t last_update;

static void cycle_init(void) {
    // setup your animation
    x = 0;
    y = 0;
    v = 1;
    last_update = millis();
}

static void cycle_tick() {
    // draw something to matrix
    // this code is allowed to be a little bit blocking
    // but must not take more than 50ms!
    // (otherwise the button will not properly work anymore)

    // non-blocking variant to do something every 100ms
    if(millis() - last_update > 100) {
        x ++;
        last_update = millis();
    }

    if (x >= WIDTH) {
        x = 0;
        y ++;
        if (y >= HEIGHT) {
            y = 0;
            v = 1 - v;
        }
    }

    // set the pixel @ x, y to v
    matrixSetPixel(x, y, v * MAX_BRIGHTNESS);

}

static void cycle_on_press(uint8_t count) {
    // handle $count button presses
}

static const animation_t cycle_animation = {
    .init = cycle_init,
    .tick = cycle_tick,
    .on_press = cycle_on_press
};

#endif