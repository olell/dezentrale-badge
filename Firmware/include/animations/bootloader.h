/**
 * This is a special animation, it can only be reached by pressing
 * the button 5 times. If the animation is active, by pressing 
 * the button another time, the device will enter the USB bootloader
 */

#ifndef _ANIM_BOOTLOADER_H
#define _ANIM_BOOTLOADER_H

#include "systick.h" // for millis()
#include "animation.h" // for animation_t
#include "matrix.h" // for matrix function
#include "touch.h"

uint8_t bootloader_v = 0;

static void bootloader_init(void) {
    // setup your animation
}

static void bootloader_tick() {
    for (uint8_t y = 0; y < HEIGHT; y ++) {
        for (uint8_t x = 0; x < WIDTH; x ++) {
            matrixSetPixel(x, y, bootloader_v * MAX_BRIGHTNESS);
        }
    }
    bootloader_v = 1 - bootloader_v;
}

static void bootloader_on_press(uint8_t count) {
    // handle `count` amount of button presses
    if (count == 1) {
        funPinMode(PA1, FUN_OUTPUT);
        funDigitalWrite(PA1, 0);
        Delay_Ms(100);
        funPinMode(PA1, FUN_INPUT);
        
        FLASH->BOOT_MODEKEYR = 0x45670123;
        FLASH->BOOT_MODEKEYR = 0xCDEF89AB;
        FLASH->STATR = 0x4000;
        RCC->RSTSCKR |= 0x1000000;
        PFIC->CFGR = 0xBEEF0080;
    }
}

static const animation_t bootloader_animation = {
    .init = bootloader_init,
    .tick = bootloader_tick,
    .on_press = bootloader_on_press,
    .tick_interval = 100
};

#endif