#ifndef __INT_KEY_H__
#define __INT_KEY_H__

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

typedef enum
{
    KEY_NONE = 0,
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_LEFT_X,
    KEY_RIGHT_X,
    KEY_LEFT_X_LONG,
    KEY_RIGHT_X_LONG
} Key_type;

/**
 * @brief  Get key value
 * 
 * @return Key_type Key_NONE: No key pressed
 */
Key_type Int_key_get(void);

#endif // __INT_KEY_H__
