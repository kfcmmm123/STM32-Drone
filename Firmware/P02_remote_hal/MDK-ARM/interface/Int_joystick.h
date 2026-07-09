#ifndef __INT_JOYSTICK_H__
#define __INT_JOYSTICK_H__

#include "adc.h"

typedef struct
{
    int16_t thr;
    int16_t yaw;
    int16_t rol;
    int16_t pit;
} Joystick_Struct;

/**
 * @brief  Initialize the joystick interface and ADC peripheral
 */
void Int_joystick_init(void);

/**
 * @brief  Get the current joystick values 
 * 
 * @param joystick: Pointer to a Joystick_Struct to store the current values
 */
void Int_joystick_get(Joystick_Struct *joystick);

#endif // __INT_JOYSTICK_H__
