#include "Int_motor.h"

/**
* @brief Initialize motor (max = 1000, default = 200)
*
* @param speed 
*/
void Init_motor_set_speed(Motor_Struct *motor)
{
    if (motor->speed > 1000)
    {
        debug_printf("Motor speed is too high\r\n");
        return;
    }

    __HAL_TIM_SET_COMPARE(motor->tim, motor->channel, motor->speed);
}

/**
 * @brief Start motor
 * 
 * @param motor
 */
void Init_motor_start(Motor_Struct *motor)
{
    __HAL_TIM_SET_COMPARE(motor->tim, motor->channel, 0);
    HAL_TIM_PWM_Start(motor->tim, motor->channel);
}
