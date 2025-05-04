#include <stdint.h>

#ifndef _SYSTICK_H
#define _SYSTICK_H

void systickInit();
void systickClear();
uint32_t millis();

#endif