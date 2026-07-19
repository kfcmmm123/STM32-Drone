#include "App_flight.h"

Gyro_Accel_Struct gyro_accel_data = { 0 };
Euler_Struct euler_angle = { 0 }; 


/**
 * @brief Calculate Euler angle based on gyro data
 */
void App_flight_get_euler_angle(void)
{
    Int_MPU6050_Get_Data(&gyro_accel_data);

}