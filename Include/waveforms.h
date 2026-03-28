#ifndef WAVEFORMS_H
#define WAVEFORMS_H

#define DELAY_MS 0
#define DELAY_US 1

#include "TM4C123GH6PM.h"
#include "timer.h"
#include <stdint.h>


void squarePulse(GPIOA_Type *port, uint32_t pin_mask, uint32_t width);

#endif