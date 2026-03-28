#include "gpio.h"

/*
 * GPIOB_init(pin_index, direction)
 *
 * Configures one Port B pin as a basic digital GPIO pin.
 *
 * - pin_index selects PB0 through PB7
 * - OUTPUT sets the pin as a digital output
 * - INPUT sets the pin as a digital input with an internal pull-up resistor
 *
 * The function enables the Port B clock, sets the pin direction,
 * disables analog and alternate functions, clears the pin's mux field,
 * and enables the digital function.
 */
void GPIOB_init(uint8_t pin_index, uint8_t direction)
{
    volatile uint32_t delay;
    uint32_t pin_mask;

    if (pin_index > 7)
    {
        return; // Invalid Port B pin
    }

    pin_mask = (1U << pin_index);

    SYSCTL->RCGCGPIO |= (1U << 1); // Enable clock for Port B
    delay = SYSCTL->RCGCGPIO;      // Give clock time to start

    if (direction == OUTPUT)
    {
        GPIOB->DIR |= pin_mask;  // Set selected pin as output
        GPIOB->PUR &= ~pin_mask; // Disable pull-up on output pin
    }
    else if (direction == INPUT)
    {
        GPIOB->DIR &= ~pin_mask; // Set selected pin as input
        GPIOB->PUR |= pin_mask;  // Enable pull-up resistor on input pin
    }

    GPIOB->DEN |= pin_mask;                     // Enable digital function on selected pin
    GPIOB->AFSEL &= ~pin_mask;                  // Disable alternate function on selected pin
    GPIOB->AMSEL &= ~pin_mask;                  // Disable analog function on selected pin
    GPIOB->PCTL &= ~(0x0Fu << (4 * pin_index)); // Clear mux bits for selected pin
}

/*
 * SW2_init()
 *
 * Configures PF0 as a digital input for the onboard switch.
 *
 * PF0 is locked by default, so it must be unlocked before it can be changed.
 * The pin is set up as a GPIO input with an internal pull-up resistor, so:
 * - not pressed = 1
 * - pressed     = 0
 */
void SW2_init(void)
{
    volatile uint32_t delay;

    SYSCTL->RCGCGPIO |= (1U << 5); // Enable clock for Port F
    delay = SYSCTL->RCGCGPIO;      // Give clock time to start

    GPIOF->LOCK = 0x4C4F434B; // Unlock Port F
    GPIOF->CR |= 0x01;        // Allow changes to PF0

    GPIOF->AMSEL &= ~0x01;      // Disable analog on PF0
    GPIOF->PCTL &= ~0x0000000F; // Clear mux bits so PF0 works as GPIO
    GPIOF->AFSEL &= ~0x01;      // Disable alternate function on PF0

    GPIOF->DIR &= ~0x01; // Set PF0 as input
    GPIOF->PUR |= 0x01;  // Enable pull-up resistor on PF0
    GPIOF->DEN |= 0x01;  // Enable digital function on PF0
}