#ifndef __INIT_MOTOR_H__
#define __INIT_MOTOR_H__

#include "tim.h"
#include "Com_debug.h"

typedef struct 
{
    TIM_HandleTypeDef *tim;
    uint16_t channel;
    uint16_t speed;
}Motor_Struct;

/**
* @brief Initialize motor (max = 1000, default = 200)
*
* @param motor 
*/
void Init_motor_set_speed(Motor_Struct *motor);

/**
 * @brief Start motor
 * 
 * @param motor
 */
void Init_motor_start(Motor_Struct *motor);

#endif /* __INIT_MOTOR_H__ */
