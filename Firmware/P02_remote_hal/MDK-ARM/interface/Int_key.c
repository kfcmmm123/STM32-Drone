#include "Int_key.h"

/**
 * @brief  Get key value
 * 
 * @return Key_type Key_NONE: No key pressed
 */
Key_type Int_key_get(void)
{
    if (HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET)
    {
        vTaskDelay(10); // Debounce delay
        if (HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET)
        {
            // Wait until the key is released
            while (HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(10); // Delay to avoid busy waiting
            }
            return KEY_UP;
        }
    }
    else if (HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET)
    {
        vTaskDelay(10); // Debounce delay
        if (HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET)
        {
            // Wait until the key is released
            while (HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(10); // Delay to avoid busy waiting
            }
            return KEY_DOWN;
        }
    }
    else if (HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET)
    {
        vTaskDelay(10); // Debounce delay
        if (HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET)
        {
            // Wait until the key is released
            while (HAL_GPIO_ReadPin(KEY_LEFT_GPIO_Port, KEY_LEFT_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(10); // Delay to avoid busy waiting
            }
            return KEY_LEFT;
        }
    }
    else if (HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET)
    {
        vTaskDelay(10); // Debounce delay
        if (HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET)
        {
            // Wait until the key is released
            while (HAL_GPIO_ReadPin(KEY_RIGHT_GPIO_Port, KEY_RIGHT_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(10); // Delay to avoid busy waiting
            }
            return KEY_RIGHT;
        }
    }

    // Top left and right keys
    else if (HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
    {
        vTaskDelay(10); // Debounce delay
        if (HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
        {
            // Wait until the key is released
            while (HAL_GPIO_ReadPin(KEY_LEFT_X_GPIO_Port, KEY_LEFT_X_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(10); // Delay to avoid busy waiting
            }
            return KEY_LEFT_X;
        }
    }
    else if (HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
    {
        // Top right key has a long press feature, so we need to check for long press duration
        TickType_t start_time = xTaskGetTickCount();
        vTaskDelay(10); // Debounce delay
        if (HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
        {
            // Wait until the key is released
            while (HAL_GPIO_ReadPin(KEY_RIGHT_X_GPIO_Port, KEY_RIGHT_X_Pin) == GPIO_PIN_RESET)
            {
                vTaskDelay(10); // Delay to avoid busy waiting
            }
            TickType_t end_time = xTaskGetTickCount();
            if ((end_time - start_time) >= 1000) // Long press detected (1 second)
            {
                return KEY_RIGHT_X_LONG;
            }
            return KEY_RIGHT_X;
        }
    }


    return KEY_NONE; // No key pressed
}
