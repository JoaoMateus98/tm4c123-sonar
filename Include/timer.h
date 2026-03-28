#ifndef TIMER_H
#define TIMER_H

#include "TM4C123GH6PM.h"
#include <stdint.h>

#define DELAY_MS 0
#define DELAY_US 1

void timer0A_delay(uint32_t delay, uint8_t timeUnit);

#endif