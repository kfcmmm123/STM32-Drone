#include "App_flight.h"

Gyro_Accel_Struct gyro_accel_data = { 0 };
Euler_Struct euler_angle = { 0 }; 
Gyro_Struct last_gyro = { 0 };

float gyro_z_sum = 0; 

/**
 * @brief Calculate Euler angle based on gyro data
 */
void App_flight_get_euler_angle(void)
{
    Int_MPU6050_Get_Data(&gyro_accel_data);

    // Apply low pass filter for gyro data (more stable)
    gyro_accel_data.gyro.gyro_x = Common_Filter_LowPass(gyro_accel_data.gyro.gyro_x, last_gyro.gyro_x);
    gyro_accel_data.gyro.gyro_y = Common_Filter_LowPass(gyro_accel_data.gyro.gyro_y, last_gyro.gyro_y);
    gyro_accel_data.gyro.gyro_z = Common_Filter_LowPass(gyro_accel_data.gyro.gyro_z, last_gyro.gyro_z);

    last_gyro.gyro_x = gyro_accel_data.gyro.gyro_x; 
    last_gyro.gyro_y = gyro_accel_data.gyro.gyro_y; 
    last_gyro.gyro_z = gyro_accel_data.gyro.gyro_z; 

    // Apply Kalman filter for accel data (very unstable)
    gyro_accel_data.accel.accel_x = Common_Filter_KalmanFilter(&kfs[0], gyro_accel_data.accel.accel_x);
    gyro_accel_data.accel.accel_y = Common_Filter_KalmanFilter(&kfs[1], gyro_accel_data.accel.accel_y);
    gyro_accel_data.accel.accel_z = Common_Filter_KalmanFilter(&kfs[2], gyro_accel_data.accel.accel_z);

    // Attitude Estimation => Complementary Filter (Angular velocity integration + Acceleration estimation)
    euler_angle.pitch = atan2(gyro_accel_data.accel.accel_x * 1.0, gyro_accel_data.accel.accel_z) / 3.14159 * 180;
    euler_angle.roll = atan2(gyro_accel_data.accel.accel_y * 1.0, gyro_accel_data.accel.accel_z) / 3.14159 * 180;

    gyro_z_sum += (gyro_accel_data.gyro.gyro_z * 2000.0 / 32768.0) * 0.006;
    euler_angle.yaw = gyro_z_sum; 
}
