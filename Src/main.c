#include "gpio.h"
#include "inputHandler.h"
#include "waveforms.h"

#define GPIOB_BUTTON_PIN 0x01
#define GPIOB_PULSE_PIN 0x04

int main()
{
    GPIOB_init(0, INPUT);
    GPIOB_init(2, OUTPUT);

    while (1)
    {
        if (buttonPressedOnce(GPIOB, GPIOB_BUTTON_PIN))
        {
            squarePulse(GPIOB, GPIOB_PULSE_PIN, 100);
        }
    }
}