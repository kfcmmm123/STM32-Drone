#include "App_flight.h"

Gyro_Accel_Struct gyro_accel_data = { 0 };
Euler_Struct euler_angle = { 0 }; 
Gyro_Struct last_gyro = { 0 };

float gyro_z_sum = 0; 

// Pitch angle 
PID_Struct pitch_pid = { .kp = -7.00, .ki = 0.00, .kd = 0.00};
PID_Struct gyro_y_pid = { .kp = 3.00, .ki = 0.00, .kd = 0.50};

// Row angle
PID_Struct row_pid = { .kp = -7.00, .ki = 0.00, .kd = 0.00};
PID_Struct gyro_x_pid = { .kp = 3.00, .ki = 0.00, .kd = 0.50};

// Yaw angle
PID_Struct yaw_pid = { .kp = -3.00, .ki = 0.00, .kd = 0.00};
PID_Struct gyro_z_pid = { .kp = -5.00, .ki = 0.00, .kd = 0.00};

extern Remote_data remote_data; 
extern Flight_State flight_state;

extern uint16_t fix_height;

PID_Struct height_pid = { .kp = -0.60, .ki = 0.00, .kd = -0.20};

Motor_Struct left_top_motor = { .tim = &htim3, .channel = TIM_CHANNEL_1, .speed = 0 };
Motor_Struct left_bottom_motor = { .tim = &htim4, .channel = TIM_CHANNEL_4, .speed = 0 };
Motor_Struct right_top_motor = { .tim = &htim2, .channel = TIM_CHANNEL_2, .speed = 0 };
Motor_Struct right_bottom_motor = { .tim = &htim1, .channel = TIM_CHANNEL_3, .speed = 0 };


/**
 * @brief Initialize MPU6050 and start motors
 */
void App_flight_init(void)
{
    Int_MPU6050_Init();

    Init_motor_start(&left_top_motor);
    Init_motor_start(&left_bottom_motor);
    Init_motor_start(&right_top_motor);
    Init_motor_start(&right_bottom_motor);

    Int_VL53L1X_Init(); 
}

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

/**
 * @brief Calculate PID values based on Euler angles
 */
void App_flight_pid_process(void)
{
    // Pitch angle 
    pitch_pid.desire = (remote_data.pit - 500) / 50.0;
    pitch_pid.measure = euler_angle.pitch; 
    
    gyro_y_pid.measure = gyro_accel_data.gyro.gyro_y * 2000.0 / 32768.0;

    Com_PID_Calc_Chain(&pitch_pid, &gyro_y_pid); 

    // Row angle
    row_pid.desire = (remote_data.rol - 500) / 50.0;
    row_pid.measure = euler_angle.roll; 

    gyro_x_pid.measure = gyro_accel_data.gyro.gyro_x * 2000.0 / 32768.0;

    Com_PID_Calc_Chain(&row_pid, &gyro_x_pid);

    // Yaw angle
    yaw_pid.desire = (remote_data.yaw - 500) / 50.0;
    yaw_pid.measure = euler_angle.yaw; 

    gyro_z_pid.measure = gyro_accel_data.gyro.gyro_z * 2000.0 / 32768.0;

    Com_PID_Calc_Chain(&yaw_pid, &gyro_z_pid);
}

/**
 * @brief Control motors based on PID values 
 */
void App_flight_control_motor(void)
{
    switch (flight_state)
    {
    case IDLE:
        left_top_motor.speed = 0;
        left_bottom_motor.speed = 0;
        right_top_motor.speed = 0;
        right_bottom_motor.speed = 0;
        break;

    case NORMAL:
        left_top_motor.speed = remote_data.thr + gyro_y_pid.output - gyro_x_pid.output + gyro_z_pid.output;
        left_bottom_motor.speed = remote_data.thr - gyro_y_pid.output - gyro_x_pid.output - gyro_z_pid.output;
        right_top_motor.speed = remote_data.thr + gyro_y_pid.output + gyro_x_pid.output - gyro_z_pid.output;
        right_bottom_motor.speed = remote_data.thr - gyro_y_pid.output + gyro_x_pid.output + gyro_z_pid.output;
        break;

    case FIX_HEIGHT:
        left_top_motor.speed = remote_data.thr + gyro_y_pid.output - gyro_x_pid.output + gyro_z_pid.output + height_pid.output;
        left_bottom_motor.speed = remote_data.thr - gyro_y_pid.output - gyro_x_pid.output - gyro_z_pid.output + height_pid.output;
        right_top_motor.speed = remote_data.thr + gyro_y_pid.output + gyro_x_pid.output - gyro_z_pid.output + height_pid.output;
        right_bottom_motor.speed = remote_data.thr - gyro_y_pid.output + gyro_x_pid.output + gyro_z_pid.output + height_pid.output;
        break;

    case FAIL: 
        break; 

    default:
        break;
    }

    left_top_motor.speed = Com_Limit(left_top_motor.speed, 600, 0);
    left_bottom_motor.speed = Com_Limit(left_bottom_motor.speed, 600, 0);
    right_top_motor.speed = Com_Limit(right_top_motor.speed, 600, 0);
    right_bottom_motor.speed = Com_Limit(right_bottom_motor.speed, 600, 0);

    if (remote_data.thr < 50)
    {
        left_top_motor.speed = 0;
        left_bottom_motor.speed = 0;
        right_top_motor.speed = 0;
        right_bottom_motor.speed = 0;
    }

    Init_motor_set_speed(&left_top_motor);
    Init_motor_set_speed(&left_bottom_motor);
    Init_motor_set_speed(&right_top_motor);
    Init_motor_set_speed(&right_bottom_motor);
}

/**
 * @brief Calculate PID values for fix height mode
 */
void App_flight_fix_height_pid_process(void)
{
    height_pid.desire = fix_height; 
    height_pid.measure = Int_VL53L1X_Get_Distance();

    Com_PID_Calc(&height_pid);
}
