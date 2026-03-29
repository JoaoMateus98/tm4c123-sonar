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

/*
 * Timer1A_Init()
 *
 * Configures Timer1A for 16-bit input edge-time capture mode.
 * The timer is set to:
 * - capture mode
 * - edge-time mode
 * - up-counting
 * - initially capture rising edges
 */
void Timer1A_Init(void)
{
    volatile uint32_t delay;

    SYSCTL->RCGCTIMER |= 0x02; // enable clock for Timer1
    delay = SYSCTL->RCGCTIMER; // allow clock time to start

    TIMER1->CTL &= ~0x01;   // disable Timer1A during setup
    TIMER1->CFG = 0x04;     // 16-bit configuration
    TIMER1->TAMR = 0x17;    // capture mode, edge-time, up-count
    TIMER1->CTL &= ~0x0C;   // TAEVENT = 00 -> rising edge
    TIMER1->TAILR = 0xFFFE; // avoid 0xFFFF errata
    TIMER1->TAPR = 0xFF;    // prescaler extension
    TIMER1->ICR = 0x04;     // clear capture flag
    TIMER1->CTL |= 0x01;    // enable Timer1A
}

/*
 * Timer1A_GetPulseWidth()
 *
 * Measures one pulse width by capturing:
 * - rising edge timestamp
 * - falling edge timestamp
 *
 * Since the timer counts up, pulse width = fallingEdge - risingEdge.
 *
 * Returns:
 *   pulse width in timer ticks
 */
uint32_t Timer1A_GetPulseWidth(void)
{
    uint32_t risingEdge, fallingEdge;
    uint32_t timeout;

    TIMER1->ICR = 0x04;

    TIMER1->CTL &= ~0x01;
    TIMER1->CTL &= ~0x0C; // rising edge
    TIMER1->CTL |= 0x01;

    timeout = 1000000;
    while (((TIMER1->RIS & 0x04) == 0) && timeout)
    {
        timeout--;
    }
    if (timeout == 0)
    {
        return 0;
    }

    risingEdge = TIMER1->TAR;
    TIMER1->ICR = 0x04;

    TIMER1->CTL &= ~0x01;
    TIMER1->CTL &= ~0x0C;
    TIMER1->CTL |= 0x04; // falling edge
    TIMER1->ICR = 0x04;
    TIMER1->CTL |= 0x01;

    timeout = 1000000;
    while (((TIMER1->RIS & 0x04) == 0) && timeout)
    {
        timeout--;
    }
    if (timeout == 0)
    {
        return 0;
    }

    fallingEdge = TIMER1->TAR;
    TIMER1->ICR = 0x04;

    return (fallingEdge - risingEdge);
}