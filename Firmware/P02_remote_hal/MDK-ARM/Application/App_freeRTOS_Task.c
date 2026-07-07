#include "App_freeRTOS_Task.h"

void power_task(void *args);
#define POWER_TASK_STACK_SIZE 128
#define POWER_TASK_PRIORITY 1
TaskHandle_t power_task_handle;
#define POWER_TASK_PERIOD 10000

void com_task(void *args);
#define COM_TASK_STACK_SIZE 128
#define COM_TASK_PRIORITY 3
TaskHandle_t com_task_handle;
#define COM_TASK_PERIOD 6

/**
 * @brief  Initialize FreeRTOS tasks
 */
void App_freeRTOS_Task_Init(void) 
{
    xTaskCreate(power_task, "power_task", POWER_TASK_STACK_SIZE, NULL, POWER_TASK_PRIORITY, &power_task_handle);

    xTaskCreate(com_task, "com_task", COM_TASK_STACK_SIZE, NULL, COM_TASK_PRIORITY, &com_task_handle);

    vTaskStartScheduler();
}

void power_task(void *args)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    while (1)
    {
        vTaskDelayUntil(&last_wake_time, POWER_TASK_PERIOD);

        Init_TP4336();
    }
}

uint8_t com_buff[TX_PLOAD_WIDTH] = {0};

void com_task(void *args)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    while (1)
    {
        Int_SI24R1_TX_Mode();

        Int_SI24R1_TxPacket(com_buff);

        Int_SI24R1_RX_Mode();
        
        vTaskDelayUntil(&last_wake_time, COM_TASK_PERIOD);

        debug_printf("Hello remote\n");
    }
}
