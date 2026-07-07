#ifndef __INIT_LED_H__
#define __INIT_LED_H__

#include "main.h"

typedef struct 
{
    GPIO_TypeDef *port; /**< GPIO port for the LED */
    uint16_t pin;       /**< GPIO pin for the LED */
}LED_Struct;

/**
 * @brief Turn on LED
 * 
 * @param led Pointer to LED structure
 */
void Int_led_turn_on(LED_Struct *led);

/**
 * @brief Turn off LED
 * 
 * @param led Pointer to LED structure
 */
void Int_led_turn_off(LED_Struct *led);

/**
 * @brief Toggle LED state
 * 
 * @param led Pointer to LED structure
 */
void Int_led_toggle(LED_Struct *led);

#endif /* __INIT_LED_H__ */
