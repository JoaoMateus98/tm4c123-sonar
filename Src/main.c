#include "gpio.h"
#include "inputHandler.h"
#include "waveforms.h"

#define GPIOB_PULSE_PIN 0x01
#define GPIOB_BUTTON_PIN 0x02

int main()
{
    GPIOB_init(0, OUTPUT);
    GPIOB_init(1, INPUT);

    while (1)
    {
        if (buttonPressedOnce(GPIOB, GPIOB_BUTTON_PIN))
        {
            squarePulse(GPIOB, GPIOB_PULSE_PIN, 100);
        }
    }
}