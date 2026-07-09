#ifndef __APP_PROCESS_DATA_H__
#define __APP_PROCESS_DATA_H__

#include "Int_key.h"
#include "Int_joystick.h"

typedef struct 
{
    int16_t thr;
    int16_t yaw;
    int16_t rol;
    int16_t pit;
    uint8_t shutdown; // 0: no shutdown, 1: shutdown
    uint8_t fix_height;
} Remote_data;

/**
 * @brief Process key data => record any pressed keys
 */
void App_process_key_data(void);

/**
 * @brief Process joystick data 
 */
void App_process_joystick_data(void);

#endif // __APP_PROCESS_DATA_H__
