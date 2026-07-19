#ifndef __APP_RECEIVE_DATA_H__
#define __APP_RECEIVE_DATA_H__

#include "Int_SI24R1.h"
#include "Com_config.h"

#define FRAME_HEAD_CHECK_1 's'
#define FRAME_HEAD_CHECK_2 'g'
#define FRAME_HEAD_CHECK_3 'g'

#define MAX_RETRY_TIMES 10

/**
 * @brief Receive data from remote controller
 * 
 * @return uint8_t 0: successful receive, 1: failed receive (no data/check failed)
 */
uint8_t App_receive_data(void); 

/**
 * @brief Process connection state 
 */
void App_process_connect_state(uint8_t res);

/**
 * @brief Process flight state 
 */
void App_process_flight_state(void);

#endif // __APP_RECEIVE_DATA_H__
