#include <stdint.h>
#include "systick.h"
#include "ch32fun.h"
#include "matrix.h"
#include <stdio.h>

const uint32_t systick_interval = ((uint32_t)FUNCONF_SYSTEM_CORE_CLOCK / 100000); // 10us

volatile uint32_t systick_divider;
volatile uint32_t systick_counter;

/**
 * @brief inits the systick
 */
void systickInit() {
    SysTick->CMP = systick_interval - 1;
    SysTick->CNT = 0;
    SysTick->CTLR = SYSTICK_CTLR_STE | SYSTICK_CTLR_STIE | SYSTICK_CTLR_STCLK;
    systick_counter = 0;
    NVIC_EnableIRQ(SysTicK_IRQn);
}
void systickClear() {
    NVIC_DisableIRQ(SysTicK_IRQn);
    NVIC_ClearPendingIRQ(SysTicK_IRQn);
}

void SysTick_Handler(void) __attribute__((interrupt));
void SysTick_Handler(void) {
	SysTick->CMP += systick_interval;
	SysTick->SR = 0x00000000;

    if (++ systick_divider == 100) {
        systick_divider = 0;
        systick_counter ++;
    }
    
    matrixDisplay();
}

/**
 * @brief Returns the milliseconds since systickInit()
 */
uint32_t millis() {
    return systick_counter;
}