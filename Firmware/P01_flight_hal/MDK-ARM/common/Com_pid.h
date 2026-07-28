#ifndef __COM_PID_H__
#define __COM_PID_H__

#include "main.h"

#define PID_PERIOD 0.006

typedef struct 
{
    float kp;
    float ki;
    float kd;
    float err; 
    float desire; 
    float measure;
    float last_err;
    float integral; 
    float output; 
} PID_Struct;

/**
 * @brief single PID calculation 
 */
void Com_PID_Calc(PID_Struct *pid);

/**
 * @brief chain PID calculation 
 */
void Com_PID_Calc_Chain(PID_Struct *out_pid, PID_Struct *in_pid);

/**
 * @brief Limit the motor speed 
 * @param speed 
 * @param max_speed
 * @param min_speed 
 * @return int16_t
 */
int16_t Com_Limit(int16_t speed, int16_t max_speed, int16_t min_speed);
#endif // __COM_PID_H__