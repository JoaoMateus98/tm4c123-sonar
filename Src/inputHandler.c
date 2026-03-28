#include "inputHandler.h"

/*
 * buttonPressedOnce(port, sw_pin)
 *
 * Returns 1 only once for each full button press.
 * After a valid press is detected, the button must be released
 * before another press can return 1 again.
 */
uint32_t buttonPressedOnce(GPIOA_Type *port, uint8_t sw_pin)
{
    static uint32_t isButtonPressed = 0;
    uint32_t swState = (port->DATA & sw_pin) ? 1 : 0;

    if (swState == 0 && !isButtonPressed)
    {
        if (buttonStateStable(port, sw_pin, 0, 20))
        {
            isButtonPressed = 1;
            return 1;
        }
    }
    else if (swState == 1 && isButtonPressed)
    {
        if (buttonStateStable(port, sw_pin, 1, 100))
        {
            isButtonPressed = 0;
        }
    }

    return 0;
}

/*
 * buttonStateStable(port, pin_mask, expectedState, debounceTime)
 *
 * Checks whether a button input stays at the expected state
 * after a debounce delay.
 *
 * - expectedState = 0 checks for a stable low state
 * - expectedState = 1 checks for a stable high state
 * - debounceTime is in milliseconds
 *
 * Returns 1 if the state is still the same after the delay,
 * otherwise returns 0.
 */
uint32_t buttonStateStable(GPIOA_Type *port, uint32_t pin_mask, uint8_t expectedState,
                           uint32_t debounceTime)
{
    uint32_t currentState = (port->DATA & pin_mask) ? 1 : 0;

    if (currentState != expectedState)
    {
        return 0;
    }

    timer0A_delay(debounceTime, DELAY_MS);

    currentState = (port->DATA & pin_mask) ? 1 : 0;

    if (currentState == expectedState)
    {
        return 1;
    }

    return 0;
}