#include "Int_led.h"

/**
 * @brief Turn on LED
 * 
 * @param led Pointer to LED structure
 */
void Int_led_turn_on(LED_Struct *led)
{
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_RESET);
}

/**
 * @brief Turn off LED
 * 
 * @param led Pointer to LED structure
 */
void Int_led_turn_off(LED_Struct *led)
{
    HAL_GPIO_WritePin(led->port, led->pin, GPIO_PIN_SET);
}

/**
 * @brief Toggle LED state
 * 
 * @param led Pointer to LED structure
 */
void Int_led_toggle(LED_Struct *led)
{
    HAL_GPIO_TogglePin(led->port, led->pin);
}
