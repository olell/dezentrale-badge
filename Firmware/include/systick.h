#include <stdint.h>

#ifndef _SYSTICK_H
#define _SYSTICK_H

void systickInit();
void systickClear();
float f_millis();
uint32_t millis();

#endif