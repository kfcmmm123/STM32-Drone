#include "App_process_data.h"

Joystick_Struct joystick = { 0 };

Remote_data remote_data = { 0 };

int16_t key_pit_offset = 0;
int16_t key_rol_offset = 0;

int8_t thr_offset = 0;
int8_t yaw_offset = 0;
int8_t rol_offset = 0;
int8_t pit_offset = 0;

/**
 * 
 */
void App_calibrate_joystick(void)
{
    key_pit_offset = 0;
    key_rol_offset = 0;

    int8_t thr_sum = 0;
    int8_t yaw_sum = 0;
    int8_t rol_sum = 0;
    int8_t pit_sum = 0;

    for (uint8_t i = 0; i < 10; i++)
    {
        App_process_joystick_data();
        thr_sum += joystick.thr - 0;
        yaw_sum += joystick.yaw - 500;
        rol_sum += joystick.rol - 500;
        pit_sum += joystick.pit - 500;
        vTaskDelay(10);
    }

    thr_offset += thr_sum / 10;
    yaw_offset += yaw_sum / 10;
    rol_offset += rol_sum / 10;
    pit_offset += pit_sum / 10;
}

/**
 * @brief Process key data => record any pressed keys
 */
void App_process_key_data(void)
{
    Key_type key = Int_key_get();
    if (key == KEY_UP)
    {
        // Adjust pitch offset => pitch forward 
        key_pit_offset += 10;
    }
    else if (key == KEY_DOWN)
    {
        // Adjust pitch offset => pitch backward 
        key_pit_offset -= 10;
    }
    else if (key == KEY_LEFT)
    {
        // Adjust roll offset => roll to the left
        key_rol_offset -= 10;
    }
    else if (key == KEY_RIGHT)
    {
        // Adjust roll offset => roll to the right
        key_rol_offset += 10;
    }

    else if (key == KEY_LEFT_X)
    {
        remote_data.shutdown = 1;
    }
    else if (key == KEY_RIGHT_X)
    {
        remote_data.fix_height = 1;
    }
    else if (key == KEY_RIGHT_X_LONG)
    {
        // Adjust joystick offset  
        App_calibrate_joystick();
    }
}

/**
 * @brief Process joystick data 
 */
void App_process_joystick_data(void)
{
    // Prevent task switch during processing 
    taskENTER_CRITICAL();

    Int_joystick_get(&joystick);

    // Convert data polarity and range to 0-1000 => ADC range = 0-4095
    joystick.thr = 1000 - joystick.thr * 1000 / 4095;
    joystick.yaw = 1000 - joystick.yaw * 1000 / 4095;
    joystick.rol = 1000 - joystick.rol * 1000 / 4095;
    joystick.pit = 1000 - joystick.pit * 1000 / 4095;

    // Bias Calibration
    joystick.thr -= thr_offset;
    joystick.yaw -= yaw_offset;
    joystick.rol -= rol_offset;
    joystick.pit -= pit_offset;

    // Apply pit/roll offset 
    joystick.rol += key_rol_offset;
    joystick.pit += key_pit_offset;

    // Apply max/min limits 
    joystick.thr = Com_limit(joystick.thr, 0, 1000);
    joystick.yaw = Com_limit(joystick.yaw, 0, 1000);
    joystick.rol = Com_limit(joystick.rol, 0, 1000);
    joystick.pit = Com_limit(joystick.pit, 0, 1000);

    taskEXIT_CRITICAL();

}
