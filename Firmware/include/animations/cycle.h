#ifndef _ANIM_CYCLE_H
#define _ANIM_CYCLE_H

#include "systick.h" // for millis()
#include "animation.h" // for animation_t
#include "matrix.h" // for matrix function

// you can declare variables for your animation here
static uint8_t cycle_x, cycle_y, cycle_v;

static void cycle_init(void) {
    // setup your animation
    cycle_x = 0;
    cycle_y = 0;
    cycle_v = 1;
}

static void cycle_tick() {
    // draw something to matrix

    if (++cycle_x >= WIDTH) {
        cycle_x = 0;
        
        if (++cycle_y >= HEIGHT) {
            cycle_y = 0;
            cycle_v = 1 - cycle_v;
        }
    }

    // set the pixel @ x, y to v
    matrixSetPixel(cycle_x, cycle_y, cycle_v * MAX_BRIGHTNESS);

}

static void cycle_on_press(uint8_t count) {
    // handle $count button presses
}

static const animation_t cycle_animation = {
    .init = cycle_init,
    .tick = cycle_tick,
    .on_press = cycle_on_press,
    .tick_interval = 100
};

#endif