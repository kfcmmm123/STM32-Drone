#ifndef __COM_CONFIG_H__
#define __COM_CONFIG_H__

#include "main.h"

typedef enum 
{
    REMOTE_CONNECTED = 0,
    REMOTE_DISCONNECTED,
} Remote_State;

typedef enum 
{
    IDLE = 0,
    NORMAL, 
    FIX_HEIGHT,
    FAIL,
} Flight_State;

typedef struct
{
    int16_t thr;
    int16_t yaw;
    int16_t rol;
    int16_t pit;
    uint8_t shutdown;   // 0: no shutdown, 1: shutdown
    uint8_t fix_height; // 0: no fix height, 1: fix height
} Remote_data;

#endif /* __COM_CONFIG_H__ */
