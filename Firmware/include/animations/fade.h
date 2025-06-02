/**
  * This can be used as a fade to create custom animations.
  * Simply replace the word "fade" with your animation name
  * and create a file which is also called like your animation.
  *
  * After creating an animation include it like the other animations
  * in the main.c file and register it at 
  *
**/

#ifndef _ANIM_FADE_H
#define _ANIM_FADE_H

#include "systick.h" // for millis()
#include "animation.h" // for animation_t
#include "matrix.h" // for matrix function
#include "touch.h"

static void fade_init(void) {
    // setup your animation
}

static void fade_tick() {
    uint8_t touch_value = touchRead(0) >> 4;
    for (uint8_t y = 0; y < HEIGHT; y ++) {
        for (uint8_t x = 0; x < WIDTH; x ++) {
            matrixSetPixel(x, y, touch_value);
        }
    }
}

static void fade_on_press(uint8_t count) {
    // handle `count` amount of button presses
}

static const animation_t fade_animation = {
    .init = fade_init,
    .tick = fade_tick,
    .on_press = fade_on_press,
    .tick_interval = 100,
    .flags = ANIM_USES_TOUCH
};

#endif