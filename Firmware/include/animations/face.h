#ifndef _ANIM_FACE_H
#define _ANIM_FACE_H

#include "systick.h" // for millis()
#include "animation.h" // for animation_t
#include "matrix.h" // for matrix function

// you can declare variables for your animation here

#define MOOD_SAD 0
#define MOOD_NEUTRAL 1
#define MOOD_HAPPY 2
#define MOOD_OPEN_MOUTH 3

static uint8_t eyes_open = 0;
static uint8_t mood = MOOD_HAPPY;
static uint32_t last_update;
static uint8_t open_cycle = 0;

static void face_init(void) {
    // setup your animation
    last_update = millis();
}

static void face_tick() {
    // draw something to matrix
    // this code is allowed to be a little bit blocking
    // but must not take more than 50ms!
    // (otherwise the button will not properly work anymore)

    
    if (millis() - last_update > 200) {
        last_update = millis();

        matrixClear();


        // eyes
        if (eyes_open) {
            open_cycle ++;
            if (open_cycle > (millis() % 150)) {
                eyes_open = 0;
                open_cycle = 0;
            }
            //matrixSetPixel(1, 2, MAX_BRIGHTNESS);
            //matrixSetPixel(4, 2, MAX_BRIGHTNESS);
        }
        else {
            eyes_open = 1;
        }

        matrixSetPixel(1, 0, eyes_open ? MAX_BRIGHTNESS : 2);
        matrixSetPixel(4, 0, eyes_open ? MAX_BRIGHTNESS : 2);
        matrixSetPixel(1, 1, eyes_open ? MAX_BRIGHTNESS : 2);
        matrixSetPixel(4, 1, eyes_open ? MAX_BRIGHTNESS : 2);

        // mouth
        for (uint8_t i = 1; i < 5; i++) {
            if (mood == MOOD_HAPPY || mood == MOOD_OPEN_MOUTH) {
                matrixSetPixel(i, 5, MAX_BRIGHTNESS);
            }
            if (mood == MOOD_SAD || mood == MOOD_OPEN_MOUTH) {
                matrixSetPixel(i, 3, MAX_BRIGHTNESS);
            }
            if (mood == MOOD_NEUTRAL) {
                matrixSetPixel(i, 4, MAX_BRIGHTNESS);
            }
        }

        matrixSetPixel(0, 4, MAX_BRIGHTNESS);
        matrixSetPixel(5, 4, MAX_BRIGHTNESS);

    }
}

static void face_on_press(uint8_t count) {
    // handle $count button presses
    if (count == 1) {
        mood++;
        if (mood > MOOD_OPEN_MOUTH) mood = MOOD_SAD;
    }
}

static const animation_t face_animation = {
    .init = face_init,
    .tick = face_tick,
    .on_press = face_on_press
};

#endif