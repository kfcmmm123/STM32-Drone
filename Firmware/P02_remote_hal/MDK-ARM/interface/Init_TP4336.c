#include "Init_TP4336.h"

/**
 * @brief  Initialize TP4336 module
 */
void Init_TP4336(void)
{
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_RESET);
    vTaskDelay(100);
    HAL_GPIO_WritePin(POWER_KEY_GPIO_Port, POWER_KEY_Pin, GPIO_PIN_SET);
}