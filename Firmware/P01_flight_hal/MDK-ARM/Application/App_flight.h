#ifndef __APP_FLIGHT_H__
#define __APP_FLIGHT_H__

#include "Int_mpu6050.h"
#include "Com_filter.h"
#include "math.h"

/**
 * @brief Calculate Euler angle based on gyro data
 */
void App_flight_get_euler_angle(void);

#endif // __APP_FLIGHT_H__
