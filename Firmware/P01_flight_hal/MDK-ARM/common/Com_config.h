#ifndef __COM_CONFIG_H__
#define __COM_CONFIG_H__

#include "main.h"

typedef enum 
{
    REMOTE_CONNECTED = 0,
    REMOTE_DISCONNECTED,
}Remote_State;

typedef enum 
{
    IDLE = 0,
    NORMAL, 
    FIX_HEIGHT,
    FAIL,
}Flight_State;

#endif /* __COM_CONFIG_H__ */
