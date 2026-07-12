#ifndef __APP_TRANSMIT_DATA_H__
#define __APP_TRANSMIT_DATA_H__

#include "Int_SI24R1.h"
#include "App_process_data.h"
#include "task.h"

#define FRAME_HEAD_CHECK_1 's'
#define FRAME_HEAD_CHECK_2 'g'
#define FRAME_HEAD_CHECK_3 'g'

/**
 * @brief Transmit data to flight controller 
 */
void App_transmit_data(void);

#endif // __APP_TRANSMIT_DATA_H__
