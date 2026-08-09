#ifndef __INT_BAT_ADC_H__
#define __INT_BAT_ADC_H__

#include "adc.h"

/**
 * @brief Initialize the battery ADC
 */
void Int_bat_ADC_Init(void);

/**
 * @brief Read the battery voltage from the ADC
 * 
 * @return float 
 */
float Int_bat_ADC_Read(void);

#endif // __INT_BAT_ADC_H__
