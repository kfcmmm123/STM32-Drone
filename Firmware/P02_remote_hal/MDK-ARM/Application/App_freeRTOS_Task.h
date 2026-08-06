#ifndef __APP_FREERTOS_TASK_H__
#define __APP_FREERTOS_TASK_H__

#include "FreeRTOS.h"
#include "task.h"
#include "Com_debug.h"
#include "Int_TP4336.h"
#include "Int_SI24R1.h"
#include "App_Process_Data.h"
#include "App_transmit_data.h"
#include "App_display.h"

/**
 * @brief  Initialize FreeRTOS tasks
 */
void App_freeRTOS_Task_Init(void);

#endif /* __APP_FREERTOS_TASK_H__ */
