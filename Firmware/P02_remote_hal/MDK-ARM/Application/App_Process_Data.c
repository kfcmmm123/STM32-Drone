#include "App_Process_Data.h"

Joystick_Struct joystick = { 0 };

Remote_data remote_data = { 0 };

int16_t key_pit_offset = 0;
int16_t key_roll_offset = 0;

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
        key_roll_offset -= 10;
    }
    else if (key == KEY_RIGHT)
    {
        // Adjust roll offset => roll to the right
        key_roll_offset += 10;
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
        // Adjust joystick offset logic 
    }
}

/**
 * @brief Process joystick data 
 */
void App_process_joystick_data(void)
{
    Int_joystick_get(&joystick);

    App_process_joystick_data();
}
