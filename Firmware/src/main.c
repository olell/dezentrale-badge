#include <stdio.h>

#include "ch32fun.h"
#include "matrix.h"
#include "systick.h"
#include "ch32v003_standby.h"
#include "animation.h"

// include animations
#include "animations/cycle.h"

 // max milliseconds between two pressed to be counted as multi-press
#define MULTI_PRESS_SPEED 400

size_t current_animation_idx;
animation_t* current_animation;

uint8_t loop();

int main() {
    /* System init functions */
    SystemInit();
    funGpioInitAll();
    systickInit();
    matrixInit();

    // setup deepsleep and wakeup using button
    standby_gpio_init();
    standby_gpio_assign_button(GPIOD, 4);
    standby_gpio_assign_pin(STANDBY_GPIO_PORT_D, 4, STANDBY_TRIGGER_DIRECTION_FALLING);

    // register animations
    register_animation(&cycle_animation);

    if (get_animation_count() == 0) {
        printf("System cannot run without any animations!\n");
        while(1);
    }

    current_animation_idx = 0;
    current_animation = get_animation(current_animation_idx);

    for (;;) {
        // repeat the loop until it returns 0
        while (loop());
        // disable the systick & turn off the matrix
        systickClear();
        matrixPowerOff();

        // enter standby, it will hold until the button gets pressed
		standby_enter();

        // re-enables the systick for millis() and matrix updates
        systickInit();
        Delay_Ms(1000);
    }
}

uint8_t btn = 1, btn_prev = 1;
uint32_t btn_last_press = 0;
uint32_t btn_press = 0;
uint32_t btn_duration = 0;
uint8_t btn_cnt = 0;

/**
 * @brief repeats until it returns 0 then the controller goes to deep sleep
 */
uint8_t loop() {
    // Matrix animation
    current_animation->tick();

    Delay_Ms(10); // sleep 10ms to reduce cpu load

    // do some actions if the button was pressed
    btn_prev = btn;
    btn = funDigitalRead(PD4);
    if (btn_prev && !btn) { // "falling edge"
        // store the time when the button was pressed / last time pressed
        btn_last_press = btn_press;
        btn_press = millis();        
        // wait until the button was released
        while (!funDigitalRead(PD4));

        // store the time when the button was released
        btn_duration = millis() - btn_press;

        // if the button wasnt pressed long, it was
        // pressed at least once short
        if (btn_cnt == 0) btn_cnt = 1;

        // count how many time the button was pressed shortly
        if (btn_press - btn_last_press < MULTI_PRESS_SPEED) {
            btn_cnt += 1;
        }
    }

    // btn pressed at least once and more than MULTI_PRESS_SPEED ago
    if (btn_cnt > 0 && millis() - btn_press > MULTI_PRESS_SPEED) {
        if (btn_cnt == 4) { // go to sleep
            btn_cnt = 0;
            return 0;
        }
        else if (btn_cnt == 3) { // next animation
            current_animation_idx = (current_animation_idx + 1) % get_animation_count();
            current_animation = get_animation(current_animation_idx);
            current_animation->init();
        }
        else {
            current_animation->on_press(btn_cnt);
        }
        btn_cnt = 0;
    }

    return 1;
}