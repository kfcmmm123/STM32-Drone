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

void key_task(void *args);
#define KEY_TASK_STACK_SIZE 128
#define KEY_TASK_PRIORITY 2
TaskHandle_t key_task_handle;
#define KEY_TASK_PERIOD 200

void joystick_task(void *args);
#define JOYSTICK_TASK_STACK_SIZE 128
#define JOYSTICK_TASK_PRIORITY 2
TaskHandle_t joystick_task_handle;
#define JOYSTICK_TASK_PERIOD 20

void oled_task(void *args);
#define OLED_TASK_STACK_SIZE 128
#define OLED_TASK_PRIORITY 1
TaskHandle_t oled_task_handle;
#define OLED_TASK_PERIOD 100

/**
 * @brief  Initialize FreeRTOS tasks
 */
void App_freeRTOS_Task_Init(void) 
{
    xTaskCreate(power_task, "power_task", POWER_TASK_STACK_SIZE, NULL, POWER_TASK_PRIORITY, &power_task_handle);

    xTaskCreate(com_task, "com_task", COM_TASK_STACK_SIZE, NULL, COM_TASK_PRIORITY, &com_task_handle);
    
    xTaskCreate(key_task, "key_task", KEY_TASK_STACK_SIZE, NULL, KEY_TASK_PRIORITY, &key_task_handle);

    xTaskCreate(joystick_task, "joystick_task", JOYSTICK_TASK_STACK_SIZE, NULL, JOYSTICK_TASK_PRIORITY, &joystick_task_handle);

    xTaskCreate(oled_task, "oled_task", OLED_TASK_STACK_SIZE, NULL, OLED_TASK_PRIORITY, &oled_task_handle);

    vTaskStartScheduler();
}

void power_task(void *args)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    while (1)
    {
        vTaskDelayUntil(&last_wake_time, POWER_TASK_PERIOD);

        Int_TP4336();
    }
}

void key_task(void *args)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    while (1)
    {
        App_process_key_data();

        vTaskDelayUntil(&last_wake_time, KEY_TASK_PERIOD);
    }
}

void com_task(void *args)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    while (1)
    {
        App_transmit_data();

        debug_printf("Hello remote\n");
    }
}

void joystick_task(void *args)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    Int_joystick_init(); // Initialize the joystick interface
    while (1)
    {
        App_process_joystick_data();

        vTaskDelayUntil(&last_wake_time, JOYSTICK_TASK_PERIOD);
    }
}

void oled_task(void *args)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    App_display_init();

    while (1)
    {
        App_display_show();
        vTaskDelayUntil(&last_wake_time, OLED_TASK_PERIOD);
    }
}
