#include "touch.h"
#include "ch32v003_touch.h"

#define TOUCH_CHANNELS 2

#define ABS(x) ((x) < 0 ? -(x) : (x))

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

/**
 * @brief initialises the capacitive touch
 */
void touchInit() {
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1;
	InitTouchADC();

    for (uint8_t i = 0; i < TOUCH_CHANNELS; i ++) {
        readings[i].baseline = 0;
        readings[i].max = 0;
        readings[i].current = 0;
    }
}

/**
 * @brief reads the capacitive touch pins
 */
void touchUpdate() {
    for (uint8_t i = 0; i < TOUCH_CHANNELS; i ++) {
        touch_reading_t *r = &readings[i];

        r->current = ReadTouchPin(channels[i].port, channels[i].pin, channels[i].adc, 3) * 100;

        if (r->baseline == 0) r->baseline = r->current;
        else r->baseline = (r->baseline / 100) * 99 + (r->current / 100);
        
        r->max = r->current > r->max ? r->current : r->max;
    }
}

/**
 * @brief returns the normalized value for the selected touch channel
 */
uint8_t touchRead(uint8_t channel) {
    if (channel >= TOUCH_CHANNELS) return 0;

    touch_reading_t *r = &readings[channel];

    // check for plausible baseline/max ratio
    int touch_diff = 100 - ((r->max*100) / r->baseline);
    int norm;
    if (ABS(touch_diff) < 20) { // invalid reading
        norm = 0;
    } else {
        norm = ((r->current - r->baseline)*255) / (r->max - r->baseline);
        norm = norm > 255 ? 255 : norm;
        norm = norm < 0 ? 0 : norm;
    }

    return (uint8_t) norm;
}