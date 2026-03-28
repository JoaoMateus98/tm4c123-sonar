#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#define DELAY_MS 0
#define DELAY_US 1

#include "TM4C123GH6PM.h"
#include "timer.h"
#include <stdint.h>

uint32_t buttonPressedOnce(GPIOA_Type *port, uint8_t sw_pin);
uint32_t buttonStateStable(GPIOA_Type *port, uint32_t pin_mask, uint8_t expectedState,
                           uint32_t debounceTime);

#endif