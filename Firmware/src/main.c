#include <stdio.h>

#include "ch32fun.h"
#include "matrix.h"
#include "systick.h"
#include "ch32v003_standby.h"
#include "animation.h"
#include "touch.h"

// include animations here, do not forget to register them below
#include "animations/cycle.h"
#include "animations/pulse.h"
#include "animations/rolling_text.h"
#include "animations/face.h"
#include "animations/fade.h"
#include "animations/bootloader.h"

 // max milliseconds between two pressed to be counted as multi-press
#define MULTI_PRESS_SPEED 500

size_t current_animation_idx;
const animation_t* current_animation;

// button interrupt handler
uint32_t btn_last_press = 0;
uint32_t btn_press = 0;
uint32_t btn_duration = 0;
uint32_t now = 0;
uint8_t btn_cnt = 0;

void EXTI7_0_IRQHandler( void ) __attribute__((interrupt));
void EXTI7_0_IRQHandler( void ) {
    now = millis();

    // debounce code
    if (now - btn_press < 50) {
        EXTI->INTFR = EXTI_Line4;
        return;
    };

    btn_last_press = btn_press;
    btn_press = now;

    if (btn_cnt == 0) btn_cnt = 1;

    // count how many time the button was pressed shortly
    if (btn_press - btn_last_press < MULTI_PRESS_SPEED) {
        btn_cnt += 1;
    }

    EXTI->INTFR = EXTI_Line4;
}

int main() {
    /* System init functions */
    SystemInit();
    
    funGpioInitC();
    funGpioInitD();

    systickInit();
    matrixInit();

    // setup deepsleep and wakeup using button
    standby_gpio_init();
    standby_gpio_assign_button(GPIOD, 4);
    standby_gpio_assign_pin(STANDBY_GPIO_PORT_D, 4, STANDBY_TRIGGER_DIRECTION_FALLING);

    // setup button interrupt
	AFIO->EXTICR = AFIO_EXTICR_EXTI4_PD;
	EXTI->INTENR = EXTI_INTENR_MR4;
	EXTI->RTENR = EXTI_RTENR_TR4;
    EXTI->FTENR &= ~EXTI_FTENR_TR4;

    NVIC_EnableIRQ( EXTI7_0_IRQn );

    touchInit();

    // register animations here, the animations are cycled through
    // by tripple pressing the button in this order
    register_animation(&rolling_text_animation);
    register_animation(&fade_animation);
    register_animation(&face_animation);
    register_animation(&pulse_animation);
    register_animation(&cycle_animation);

    if (get_animation_count() == 0) {
        printf("System cannot run without any animations!\n");
        while(1);
    }

    current_animation_idx = 0;
    current_animation = get_animation(current_animation_idx);
    current_animation->init();

    for (;;) {

        // repeat the loop until it breaks to sleep
        for (;;) {
            // Matrix animation
            current_animation->tick();

            Delay_Ms(current_animation->tick_interval);
            touchUpdate();
            
            // btn pressed at least once and more than MULTI_PRESS_SPEED ago
            // press count is captured by thea interrupt handler
            if (btn_cnt > 0 && millis() - btn_press > MULTI_PRESS_SPEED) {
                if (btn_cnt == 5) { // go to bootloader blink animation
                    current_animation = &bootloader_animation;
                    current_animation->init();
                }
                else if (btn_cnt == 4) { // go to sleep
                    btn_cnt = 0;
                    break;
                }
                else if (btn_cnt == 3) { // next animation
                    matrixClear();
                    current_animation_idx = (current_animation_idx + 1) % get_animation_count();
                    current_animation = get_animation(current_animation_idx);
                    current_animation->init();
                }
                else {
                    current_animation->on_press(btn_cnt);
                }
                btn_cnt = 0;
            }

        }

        /*
         * SLEEP CODE
         */

        // disable the systick & turn off the matrix
        systickClear();
        matrixPowerOff();
        NVIC_DisableIRQ(EXTI7_0_IRQn);

        // enter standby, it will hold until the button gets pressed
		standby_enter();

        // re-enables the systick for millis() and matrix updates
        systickInit();
        NVIC_EnableIRQ(EXTI7_0_IRQn);
        Delay_Ms(1000);
    }
}