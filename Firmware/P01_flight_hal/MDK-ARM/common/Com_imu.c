#include "Com_IMU.h"

/* Calculation config */
float RtA = 57.2957795f; // radians to degrees conversion factor
// gyro raw range: +-2000 dps, scale factor = 4000 / 65536
float Gyro_G = 4000.0f / 65536.0f; // degrees/s per LSB
// convert degrees/s per LSB to radians/s per LSB
float Gyro_Gr = 4000.0f / 65536.0f / 180.0f * 3.1415926f; // radians/s per LSB
#define squa(Sq) (((float)(Sq)) * ((float)(Sq)))      /* square value */
/**
 * @description: Quick approximate calculation of 1 / sqrt(number).
 * @param {float} number - input value.
 */
static float Q_rsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (threehalfs - (x2 * y * y)); // 1st iteration
    return y;
}

static float normAccz; /* Z-axis accelerometer projection along gravity direction */
/**
 * @description: Calculate Euler angles from raw gyroscope and accelerometer data using
 *               a complementary quaternion-based filter.
 * @param {GyroAccel_Struct} *gyroAccel - IMU data containing gyro and accelerometer readings.
 * @param {Euler_Struct} *eulerAngle - Output Euler angles (yaw, pitch, roll).
 * @param {float} dt - Sample interval in seconds.
 * @return {void}
 */
void Common_IMU_GetEulerAngle(Gyro_Accel_Struct *gyroAccel,
                              Euler_Struct *eulerAngle,
                              float dt)
{
    volatile struct V
    {
        float x;
        float y;
        float z;
    } Gravity, Acc, Gyro, AccGravity;

    static struct V GyroIntegError = {0};
    static float KpDef = 0.8f;
    static float KiDef = 0.0003f;
    static Quaternion_Struct NumQ = {1, 0, 0, 0};
    float q0_t, q1_t, q2_t, q3_t;
    // float NormAcc;
    float NormQuat;
    float HalfTime = dt * 0.5f;

    Gravity.x = 2 * (NumQ.q1 * NumQ.q3 - NumQ.q0 * NumQ.q2);
    Gravity.y = 2 * (NumQ.q0 * NumQ.q1 + NumQ.q2 * NumQ.q3);
    Gravity.z = 1 - 2 * (NumQ.q1 * NumQ.q1 + NumQ.q2 * NumQ.q2);

    NormQuat = Q_rsqrt(squa(gyroAccel->accel.accel_x) +
                       squa(gyroAccel->accel.accel_y) +
                       squa(gyroAccel->accel.accel_z));

    Acc.x = gyroAccel->accel.accel_x * NormQuat;
    Acc.y = gyroAccel->accel.accel_y * NormQuat;
    Acc.z = gyroAccel->accel.accel_z * NormQuat;

    AccGravity.x = (Acc.y * Gravity.z - Acc.z * Gravity.y);
    AccGravity.y = (Acc.z * Gravity.x - Acc.x * Gravity.z);
    AccGravity.z = (Acc.x * Gravity.y - Acc.y * Gravity.x);

    GyroIntegError.x += AccGravity.x * KiDef;
    GyroIntegError.y += AccGravity.y * KiDef;
    GyroIntegError.z += AccGravity.z * KiDef;

    Gyro.x = gyroAccel->gyro.gyro_x * Gyro_Gr + KpDef * AccGravity.x + GyroIntegError.x; 
    Gyro.y = gyroAccel->gyro.gyro_y * Gyro_Gr + KpDef * AccGravity.y + GyroIntegError.y;
    Gyro.z = gyroAccel->gyro.gyro_z * Gyro_Gr + KpDef * AccGravity.z + GyroIntegError.z;

    q0_t = (-NumQ.q1 * Gyro.x - NumQ.q2 * Gyro.y - NumQ.q3 * Gyro.z) * HalfTime;
    q1_t = (NumQ.q0 * Gyro.x - NumQ.q3 * Gyro.y + NumQ.q2 * Gyro.z) * HalfTime;
    q2_t = (NumQ.q3 * Gyro.x + NumQ.q0 * Gyro.y - NumQ.q1 * Gyro.z) * HalfTime;
    q3_t = (-NumQ.q2 * Gyro.x + NumQ.q1 * Gyro.y + NumQ.q0 * Gyro.z) * HalfTime;

    NumQ.q0 += q0_t;
    NumQ.q1 += q1_t;
    NumQ.q2 += q2_t;
    NumQ.q3 += q3_t;

    NormQuat = Q_rsqrt(squa(NumQ.q0) + squa(NumQ.q1) + squa(NumQ.q2) + squa(NumQ.q3));
    NumQ.q0 *= NormQuat;
    NumQ.q1 *= NormQuat;
    NumQ.q2 *= NormQuat;
    NumQ.q3 *= NormQuat;

    float vecxZ = 2 * NumQ.q0 * NumQ.q2 - 2 * NumQ.q1 * NumQ.q3;     
    float vecyZ = 2 * NumQ.q2 * NumQ.q3 + 2 * NumQ.q0 * NumQ.q1;     
    float veczZ = 1 - 2 * NumQ.q1 * NumQ.q1 - 2 * NumQ.q2 * NumQ.q2; 

    float yaw_G = gyroAccel->gyro.gyro_z * Gyro_G; 
    if ((yaw_G > 0.5f) || (yaw_G < -0.5))   
    {
        eulerAngle->yaw += yaw_G * dt; 
    }

    eulerAngle->pitch = asin(vecxZ) * RtA; 

    eulerAngle->roll = atan2f(vecyZ, veczZ) * RtA; 

    normAccz = gyroAccel->accel.accel_x * vecxZ + gyroAccel->accel.accel_y * vecyZ + gyroAccel->accel.accel_z * veczZ; 
}

/**
 * @description: Get the projected Z-axis acceleration along the gravity vector.
 * @return {float} Projected Z-axis acceleration.
 */
float Common_IMU_GetNormAccZ(void)
{
    return normAccz;
}
