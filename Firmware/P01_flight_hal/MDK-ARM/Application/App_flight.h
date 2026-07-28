#ifndef __APP_FLIGHT_H__
#define __APP_FLIGHT_H__

#include "math.h"
#include "Com_filter.h"
#include "Com_pid.h"
#include "Int_motor.h"
#include "Int_mpu6050.h"

/**
 * @brief Initialize MPU6050 and start motors
 */
void App_flight_init(void); 

/**
 * @brief Calculate Euler angle based on gyro data
 */
void App_flight_get_euler_angle(void);

/**
 * @brief Calculate PID values based on Euler angles
 */
void App_flight_pid_process(void);

/**
 * @brief Control motors based on PID values 
 */
void App_flight_control_motor(void);

#endif // __APP_FLIGHT_H__
