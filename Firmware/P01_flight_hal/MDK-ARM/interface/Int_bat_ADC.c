#include "Int_bat_ADC.h"

/**
 * @brief Initialize the battery ADC
 */
void Int_bat_ADC_Init(void)
{
    HAL_GPIO_WritePin(BAT_ADC_EN_GPIO_Port, BAT_ADC_EN_Pin, GPIO_PIN_RESET);

    HAL_ADC_Start(&hadc1);
}

/**
 * @brief Read the battery voltage from the ADC
 * 
 * @return float 
 */
float Int_bat_ADC_Read(void)
{
    uint32_t adc_value = HAL_ADC_GetValue(&hadc1);

    // Convert ADC value to voltage (assuming 12-bit ADC and 3.3V reference)
    float voltage = (adc_value / 4095.0f) * 3.3f;

    return voltage;
}
