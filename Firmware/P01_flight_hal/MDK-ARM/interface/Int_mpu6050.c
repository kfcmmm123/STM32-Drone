#include "Int_mpu6050.h"

int32_t acc_x_offset = { 0 };
int32_t acc_y_offset = { 0 };
int32_t acc_z_offset = { 0 };

int32_t gyro_x_offset = { 0 };
int32_t gyro_y_offset = { 0 };
int32_t gyro_z_offset = { 0 };


void Int_MPU6050_Write_Reg(uint8_t reg, uint8_t data)
{
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR_WRITE, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
}

void Int_MPU6050_Read_Reg(uint8_t reg, uint8_t *data)
{
    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR_READ, reg, I2C_MEMADD_SIZE_8BIT, data, 1, 100);
}

/**
 * @brief Calculate zero offset 
 */
void Int_MPU6050_Calculate_Offset(void)
{
    // Wait till the flight is steady => readings changes within range of 200 for 100 times
    Accel_Struct current_accel = { 0 };
    Accel_Struct last_accel = { 0 };
    uint8_t count = 0; 
    Int_MPU6050_Get_Acc(&last_accel);

    while (count < 100)
    {
        Int_MPU6050_Get_Acc(&current_accel);
        if (abs(current_accel.accel_x - last_accel.accel_x) < 400 &&
            abs(current_accel.accel_y - last_accel.accel_y) < 400 &&
            abs(current_accel.accel_z - last_accel.accel_z) < 400 
        )
        {
            count++;
        }
        else {
            count = 0;
        }
        last_accel = current_accel; 
        vTaskDelay(6);
    }

    // Calculate zero offset
    Gyro_Accel_Struct gyro_accel_data = { 0 };

    int32_t acc_x_sum = { 0 };
    int32_t acc_y_sum = { 0 };
    int32_t acc_z_sum = { 0 };

    int32_t gyro_x_sum = { 0 };
    int32_t gyro_y_sum = { 0 };
    int32_t gyro_z_sum = { 0 };

    for (uint8_t i = 0; i < 100; i++)
    {
        Int_MPU6050_Get_Acc(&gyro_accel_data);
        acc_x_sum += (gyro_accel_data.accel.accel_x - 0);
        acc_y_sum += (gyro_accel_data.accel.accel_y - 0);
        acc_z_sum += (gyro_accel_data.accel.accel_z - 16384);

        gyro_x_sum += (gyro_accel_data.gyro.gyro_x - 0);
        gyro_y_sum += (gyro_accel_data.gyro.gyro_y - 0);
        gyro_z_sum += (gyro_accel_data.gyro.gyro_z - 0);

        vTaskDelay(6);
    }

    acc_x_offset = acc_x_sum / 100; 
    acc_y_offset = acc_y_sum / 100; 
    acc_z_offset = acc_z_sum / 100; 

    gyro_x_offset = gyro_x_sum / 100; 
    gyro_y_offset = gyro_y_sum / 100; 
    gyro_z_offset = gyro_z_sum / 100; 
}

/**
 * @brief Initialize MPU6050
 */
void Int_MPU6050_Init(void)
{
    // Reset device => DEVICE_RESET = 1
    Int_MPU6050_Write_Reg(MPU6050_ADDR, 0x80);

    // Wait till reset is completed => read power management register = 0x40 (Sleep)
    uint8_t data = { 0 };
    while (data != 0x40)
    {
        Int_MPU6050_Read_Reg(MPU_PWR_MGMT1_REG, &data);
    }

    // Wake up device 
    Int_MPU6050_Write_Reg(MPU_PWR_MGMT1_REG, 0x00);

    // Configure Scale range
    // Set gyro config (0x1B) => +-2000 dps
    Int_MPU6050_Write_Reg(MPU_GYRO_CFG_REG, 3 << 3);
    // Set acceleromter config (0x1C) => +-2g 
    Int_MPU6050_Write_Reg(MPU_ACCEL_CFG_REG, 0x00);

    // Disable Interupt (not used)
    Int_MPU6050_Write_Reg(MPU_INT_EN_REG, 0x00);

    // User control config 
    Int_MPU6050_Write_Reg(MPU_USER_CTRL_REG, 0x00);

    // Sample rate config => 1000 Hz
    // Divider = 2  
    Int_MPU6050_Write_Reg(MPU_SAMPLE_RATE_REG, 0x01);

    // Set DLPF config => 184 Hz 
    Int_MPU6050_Write_Reg(MPU_CFG_REG, 1);

    // Enable PLL 
    Int_MPU6050_Write_Reg(MPU_PWR_MGMT1_REG, 0x01);

    // Enable gyro and accelerometer 
    Int_MPU6050_Write_Reg(MPU_PWR_MGMT2_REG, 0x00);

    // Calculate zero offset
    Int_MPU6050_Calculate_Offset();
}


/**
 * @brief Read gyro data 
 * 
 * @param gyro
 */
void Int_MPU6050_Get_Gyro(Gyro_Struct *gyro)
{
    uint8_t high = 0;
    uint8_t low = 0;

    // X axis 
    Int_MPU6050_Read_Reg(MPU_GYRO_XOUTH_REG, &high);
    Int_MPU6050_Read_Reg(MPU_GYRO_XOUTL_REG, &low);
    gyro->gyro_x = (high << 8 | low) / 32768.2f * 2000;

    // Y axis
    Int_MPU6050_Read_Reg(MPU_GYRO_YOUTH_REG, &high);
    Int_MPU6050_Read_Reg(MPU_GYRO_YOUTL_REG, &low);
    gyro->gyro_y = (high << 8 | low) / 32768.2f * 2000;

    // Z axis
    Int_MPU6050_Read_Reg(MPU_GYRO_ZOUTH_REG, &high);
    Int_MPU6050_Read_Reg(MPU_GYRO_ZOUTL_REG, &low);
    gyro->gyro_z = (high << 8 | low) / 32768.2f * 2000;

    gyro->gyro_x -= gyro_x_offset;
    gyro->gyro_y -= gyro_y_offset;
    gyro->gyro_z -= gyro_z_offset;
}

/**
 * @brief Read accelerometer data 
 * 
 * @param acc
 */
void Int_MPU6050_Get_Acc(Accel_Struct *acc)
{
    uint8_t high = 0;
    uint8_t low = 0;

    // X axis 
    Int_MPU6050_Read_Reg(MPU_ACCEL_XOUTH_REG, &high);
    Int_MPU6050_Read_Reg(MPU_ACCEL_XOUTL_REG, &low);
    acc->accel_x = (high << 8 | low) / 32768.2f * 2;

    // Y axis
    Int_MPU6050_Read_Reg(MPU_ACCEL_YOUTH_REG, &high);
    Int_MPU6050_Read_Reg(MPU_ACCEL_YOUTL_REG, &low);
    acc->accel_y = (high << 8 | low) / 32768.2f * 2;

    // Z axis
    Int_MPU6050_Read_Reg(MPU_ACCEL_ZOUTH_REG, &high);
    Int_MPU6050_Read_Reg(MPU_ACCEL_ZOUTL_REG, &low);
    acc->accel_z = (high << 8 | low) / 32768.2f * 2;

    acc->accel_x -= acc_x_offset;
    acc->accel_y -= acc_y_offset;
    acc->accel_z -= acc_z_offset;
}

/**
 * @brief Read gyro + accelerometer data 
 * 
 * @param data
 */
void Int_MPU6050_Get_Data(Gyro_Accel_Struct *data)
{
    Int_MPU6050_Get_Gyro(&data->gyro);
    Int_MPU6050_Get_Acc(&data->accel);
}