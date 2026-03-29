#ifndef GPIO_H
#define GPIO_H

// used to set GPIO directions
#define INPUT 0
#define OUTPUT 1

#include "TM4C123GH6PM.h"
#include <stdint.h>

void GPIOB_init(uint8_t pin_index, uint8_t direction);
void SW2_init(void);
void Timer1A_CapturePin_Init(void);

#endif