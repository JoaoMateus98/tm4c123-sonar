#include "timer.h"

/*
 * timer0A_delay(delay, timeUnit)
 *
 * Creates a blocking delay using Timer0A.
 *
 * - DELAY_MS delays in milliseconds
 * - DELAY_US delays in microseconds
 *
 * The timer is configured as a 16-bit periodic down-counter and
 * uses a 50 MHz clock, so each loop waits for either 1 ms or 1 us
 * depending on the selected time unit.
 */
void timer0A_delay(uint32_t delay, uint8_t timeUnit)
{
    SYSCTL->RCGCTIMER |= 0x01; // Enable clock for Timer0

    TIMER0->CTL = 0;     // Disable Timer0A during setup
    TIMER0->CFG = 0x04;  // 16-bit timer mode
    TIMER0->TAMR = 0x02; // Periodic down-counter mode

    if (timeUnit == DELAY_MS)
    {
        TIMER0->TAILR = 50000 - 1; // 1 ms at 50 MHz
    }
    else if (timeUnit == DELAY_US)
    {
        TIMER0->TAILR = 50 - 1; // 1 us at 50 MHz
    }

    TIMER0->ICR = 0x01;  // Clear timeout flag
    TIMER0->CTL |= 0x01; // Start Timer0A

    for (uint32_t i = 0; i < delay; i++)
    {
        while ((TIMER0->RIS & 0x01) == 0)
        {
        }

        TIMER0->ICR = 0x01; // Clear timeout flag for next interval
    }
}