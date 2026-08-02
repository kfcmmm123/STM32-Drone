#ifndef __INIT_TP4336_H__
#define __INIT_TP4336_H__

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"

/**
 * @brief Start TP4336 
 */
void Int_TP4336_start(void);

/**
 * @brief Shutdown TP4336
 */
void Int_TP4336_shutdown(void);

#endif
