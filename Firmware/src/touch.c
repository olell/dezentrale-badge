#include "touch.h"
#include "ch32v003_touch.h"

#define TOUCH_CHANNELS 2

#define FABS(x) ((x) < 0 ? -(x) : (x))

touch_channel_t channels[TOUCH_CHANNELS] = {
    {
        .port = GPIOD,
        .pin = 5,
        .adc = 5,
    },
    {
        .port = GPIOD,
        .pin = 6,
        .adc = 6
    }
};
touch_reading_t readings[TOUCH_CHANNELS];


void touchInit() {
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1;
	InitTouchADC();

    for (uint8_t i = 0; i < TOUCH_CHANNELS; i ++) {
        readings[i].baseline = 0;
        readings[i].max = 0;
        readings[i].current = 0;
    }
}

void touchUpdate() {
    for (uint8_t i = 0; i < TOUCH_CHANNELS; i ++) {
        touch_reading_t *r = &readings[i];

        r->current = ReadTouchPin(channels[i].port , channels[i].pin, channels[i].adc, 3);

        if (r->baseline == 0) r->baseline = r->current;
        else r->baseline = r->baseline * 0.99 + r->current * 0.01;
        
        r->max = r->current > r->max ? r->current : r->max;
    }
}

uint8_t touchRead(uint8_t channel) {
    if (channel >= TOUCH_CHANNELS) return 0;

    touch_reading_t *r = &readings[channel];

    // check for plausible baseline/max ration
    float touch_diff = 1 - (r->max / r->baseline);
    float norm;
    if (FABS(touch_diff) < 0.2) { // invalid reading
        norm = 0;
    } else {
        norm = (r->current - r->baseline) / (r->max - r->baseline) * 255.0;
        norm = norm > 255 ? 255 : norm;
        norm = norm < 0 ? 0 : norm;
    }

    return (uint8_t) norm;
}