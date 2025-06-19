#ifndef _TOUCH_H_
#define _TOUCH_H_

#include <stdint.h>
#include <ch32fun.h>

typedef struct {
    int current;
    int baseline;
    int max;
} touch_reading_t;

typedef struct {
    GPIO_TypeDef* port;
    uint8_t pin;
    uint8_t adc;
} touch_channel_t;

void touchInit();
void touchUpdate();

uint8_t touchRead(uint8_t channel);


#endif