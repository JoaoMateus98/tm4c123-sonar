#include "waveforms.h"

/*
 * squarePulse(port, pin_mask, width)
 *
 * Sends one blocking pulse on the selected GPIO pin.
 *
 * Parameters:
 * - port: GPIO port to write to
 * - pin_mask: bit mask for the selected pin
 * - width: pulse width in milliseconds
 *
 * How it works:
 * - Drives the selected pin high
 * - Waits for the requested pulse width
 * - Drives the selected pin low
 */
void squarePulse(GPIOA_Type *port, uint32_t pin_mask, uint32_t width)
{
    port->DATA |= pin_mask;         // Start pulse
    timer0A_delay(width, DELAY_MS); // Hold pin high
    port->DATA &= ~pin_mask;        // End pulse
}