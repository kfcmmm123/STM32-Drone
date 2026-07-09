#include "Int_joystick.h"

uint16_t adc_buffer[4] = { 0 }; // Buffer to store ADC values for 4 channels

/**
 * @brief  Initialize the joystick interface and ADC peripheral
 */
void Int_joystick_init(void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc_buffer, 4); // Start ADC in DMA mode to read 4 channels
}

/**
 * @brief  Get the current joystick values 
 * 
 * @param joystick: Pointer to a Joystick_Struct to store the current values
 */
void Int_joystick_get(Joystick_Struct *joystick)
{
    joystick->thr = (int16_t)adc_buffer[0]; // Read throttle value from ADC channel 0
    joystick->yaw = (int16_t)adc_buffer[1]; // Read yaw value from ADC channel 1
    joystick->rol = (int16_t)adc_buffer[2]; // Read roll value from ADC channel 2
    joystick->pit = (int16_t)adc_buffer[3]; // Read pitch value from ADC channel 3
}
