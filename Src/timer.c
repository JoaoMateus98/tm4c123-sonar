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

uint32_t inputEdgeTimer1A()
{
    uint32_t risingEdge, fallingEdge;

    SYSCTL->RCGCTIMER |= 0x02; // enable Timer1 clock
    TIMER1->CTL = 0;           // disable Timer1A
    TIMER1->CFG = 0x04;        // 16-bit mode
    TIMER1->TAMR = 0x17;       // capture, edge-time, count-up
    TIMER1->CTL &= ~0x0C;      // clear TAEVENT bits -> 00 = positive edge
    TIMER1->TAILR = 0xFFFE;    // avoid 0xFFFF errata in count-up edge-time mode
    TIMER1->TAPR = 0xFF;       // extend range with prescaler
    TIMER1->CTL |= 0x01;       // Start Timer1A
    while ((TIMER1->RIS & 0x04) == 0)
    {
        // wait for event
    }
    TIMER1->ICR = 0x01; // Clear timeout flag for next interval
}

/*
3. In the GPTM Timer Mode (GPTMTnMR) register, write the TnCMR field to 0x1 and the TnMR
field to 0x3 and select a count direction by programming the TnCDIR bit.
4. Configure the type of event that the timer captures by writing the TnEVENT field of the GPTM
Control (GPTMCTL) register.
5. If a prescaler is to be used, write the prescale value to the GPTM Timer n Prescale Register
(GPTMTnPR).
6. Load the timer start value into the GPTM Timer n Interval Load (GPTMTnILR) register.
7. If interrupts are required, set the CnEIM bit in the GPTM Interrupt Mask (GPTMIMR) register.
8. Set the TnEN bit in the GPTM Control (GPTMCTL) register to enable the timer and start counting.
9. Poll the CnERIS bit in the GPTMRIS register or wait for the interrupt to be generated (if
enabled). In both cases, the status flags are cleared by writing a 1 to the CnECINT bit of the GPTM
Interrupt Clear (GPTMICR) register. The time at which the event happened can be obtained
by reading the GPTM Timer n (GPTMTnR) register.
In Input Edge Timing mode, the timer continues running after an edge event has been detected,
but the timer interval can be changed at any time by writing the GPTMTnILR register and clearing
the TnILD bit in the GPTMTnMR register. The change takes effect at the next cycle after the write
*/