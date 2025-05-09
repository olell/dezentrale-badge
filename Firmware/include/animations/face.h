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

static uint8_t face_eyes_open = 0;
static uint8_t face_mood = MOOD_HAPPY;
static uint8_t face_open_cycle = 0;

static void face_init(void) {
    // setup your animation
}

static void face_tick() {
    // draw something to matrix

    // eyes
    if (face_eyes_open) {
        face_open_cycle ++;
        if (face_open_cycle > (millis() % 150)) {
            face_eyes_open = 0;
            face_open_cycle = 0;
        }
        //matrixSetPixel(1, 2, MAX_BRIGHTNESS);
        //matrixSetPixel(4, 2, MAX_BRIGHTNESS);
    }
    else {
        face_eyes_open = 1;
    }

    matrixSetPixel(1, 0, face_eyes_open ? MAX_BRIGHTNESS : 2);
    matrixSetPixel(4, 0, face_eyes_open ? MAX_BRIGHTNESS : 2);
    matrixSetPixel(1, 1, face_eyes_open ? MAX_BRIGHTNESS : 2);
    matrixSetPixel(4, 1, face_eyes_open ? MAX_BRIGHTNESS : 2);

    // mouth
    for (uint8_t i = 1; i < 5; i++) {
        if (face_mood == MOOD_HAPPY || face_mood == MOOD_OPEN_MOUTH) {
            matrixSetPixel(i, 5, MAX_BRIGHTNESS);
        }
        if (face_mood == MOOD_SAD || face_mood == MOOD_OPEN_MOUTH) {
            matrixSetPixel(i, 3, MAX_BRIGHTNESS);
        }
        if (face_mood == MOOD_NEUTRAL) {
            matrixSetPixel(i, 4, MAX_BRIGHTNESS);
        }
    }

    matrixSetPixel(0, 4, MAX_BRIGHTNESS);
    matrixSetPixel(5, 4, MAX_BRIGHTNESS);
}

static void face_on_press(uint8_t count) {
    // handle $count button presses
    if (count == 1) {
        face_mood++;
        if (face_mood > MOOD_OPEN_MOUTH) face_mood = MOOD_SAD;
        matrixClear();
    }
}

static const animation_t face_animation = {
    .init = face_init,
    .tick = face_tick,
    .on_press = face_on_press,
    .tick_interval = 200,
};

#endif