#include "gpio.h"
#include "inputHandler.h"
#include "waveforms.h"

#define GPIOB_BUTTON_PIN 0x01
#define GPIOB_PULSE_PIN 0x04

#define CENTIMETER 0
#define INCH 1

uint32_t getDistance(uint8_t unit);

int main()
{
    Timer1A_CapturePin_Init();
    Timer1A_Init();
    GPIOB_init(0, INPUT);
    GPIOB_init(2, OUTPUT);

    static uint32_t distance;

    while (1)
    {
        if (buttonPressedOnce(GPIOB, GPIOB_BUTTON_PIN))
        {
            squarePulse(GPIOB, GPIOB_PULSE_PIN, 20);

            distance = getDistance(INCH);
        }
    }
}

uint32_t getDistance(uint8_t unit)
{
    uint32_t pulseWidthTicks = Timer1A_GetPulseWidth();
    uint32_t pulseWidthInMicroSeconds;

    pulseWidthInMicroSeconds = pulseWidthTicks / 50;

    if (unit == CENTIMETER)
    {
        return pulseWidthInMicroSeconds / 58;
    }

    if (unit == INCH)
    {
        return pulseWidthInMicroSeconds / 148;
    }

    return 0;
}