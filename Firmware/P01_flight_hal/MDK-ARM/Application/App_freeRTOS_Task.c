#include "App_freeRTOS_task.h"

// Power management task
void power_task(void *args);
#define POWER_TASK_STACK_SIZE 128
#define POWER_TASK_PRIORITY 1
TaskHandle_t power_task_handle;
#define POWER_TASK_PERIOD 10000 // 10 seconds

// Flight control task
void flight_task(void *args);
#define FLIGHT_TASK_STACK_SIZE 128
#define FLIGHT_TASK_PRIORITY 2
TaskHandle_t flight_task_handle;
#define FLIGHT_TASK_PERIOD 6

Motor_Struct left_top_motor = { .tim = &htim3, .channel = TIM_CHANNEL_1, .speed = 200 };
Motor_Struct left_bottom_motor = { .tim = &htim4, .channel = TIM_CHANNEL_4, .speed = 200 };
Motor_Struct right_top_motor = { .tim = &htim2, .channel = TIM_CHANNEL_2, .speed = 200 };
Motor_Struct right_bottom_motor = { .tim = &htim1, .channel = TIM_CHANNEL_3, .speed = 200 };


// LED control task
void led_task(void *args);
#define LED_TASK_STACK_SIZE 128
#define LED_TASK_PRIORITY 1
TaskHandle_t led_task_handle;
#define LED_TASK_PERIOD 100

LED_Struct left_top_led = { .port = LED1_GPIO_Port, .pin = LED1_Pin };
LED_Struct right_top_led = { .port = LED2_GPIO_Port, .pin = LED2_Pin };
LED_Struct right_bottom_led = { .port = LED3_GPIO_Port, .pin = LED3_Pin };
LED_Struct left_bottom_led = { .port = LED4_GPIO_Port, .pin = LED4_Pin };

Remote_State remote_state = REMOTE_DISCONNECTED;
Flight_State flight_state = IDLE;

Remote_data remote_data = { 0 };

// Communication task
void com_task(void *args);
#define COM_TASK_STACK_SIZE 128
#define COM_TASK_PRIORITY 2
TaskHandle_t com_task_handle;
#define COM_TASK_PERIOD 6

/**
 * @brief  Initialize FreeRTOS tasks
 */
void App_freeRTOS_Task_Init(void) 
{
    xTaskCreate(power_task, "power_task", POWER_TASK_STACK_SIZE, NULL, POWER_TASK_PRIORITY, &power_task_handle);
    
    xTaskCreate(flight_task, "flight_task", FLIGHT_TASK_STACK_SIZE, NULL, FLIGHT_TASK_PRIORITY, &flight_task_handle);
    
    xTaskCreate(led_task, "led_task", LED_TASK_STACK_SIZE, NULL, LED_TASK_PRIORITY, &led_task_handle);

    xTaskCreate(com_task, "com_task", COM_TASK_STACK_SIZE, NULL, COM_TASK_PRIORITY, &com_task_handle);
    
    vTaskStartScheduler();
}

void power_task(void *args)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    while (1)
    {
        // Runs every 10 sec => prevent shutdown 
        
        // Wait until being notified to start => 0: timeout, 1: notified 
        uint32_t res = ulTaskNotifyTake(pdTRUE, POWER_TASK_PERIOD);

        if (res != 0)
        {
            // Timeout => shutdown
            Int_TP4336_shutdown();
        }
        else 
        {
            // Notified => start power 
            Int_TP4336_start();
        }
    }
}

void flight_task(void *args)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    Int_MPU6050_Init();
    while (1)
    {
        // left_top_motor.speed = 400;
        // Init_motor_start(&left_top_motor);

        App_flight_get_euler_angle();
        vTaskDelayUntil(&last_wake_time, FLIGHT_TASK_PERIOD);
    }
}

void led_task(void *args)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    uint8_t count = 0;
    while (1)
    {
        count++;

        // Top LEDs indicate remote connection status
        if (remote_state == REMOTE_CONNECTED)
        {
            Int_led_turn_on(&left_top_led);
            Int_led_turn_on(&right_top_led);
        }
        else
        {
            Int_led_turn_off(&left_top_led);
            Int_led_turn_off(&right_top_led);
        }


        // Bottom LEDs indicate flight state
        if (flight_state == IDLE)
        {
            // Blink every 500 ms 
            if (count % 5 == 0)
            {
                Int_led_toggle(&left_bottom_led);
                Int_led_toggle(&right_bottom_led);
            }
        }
        else if (flight_state == NORMAL)
        {
            // Blink every 200 ms 
            if (count % 2 == 0)
            {
                Int_led_toggle(&left_bottom_led);
                Int_led_toggle(&right_bottom_led);
            }
        }
        else if (flight_state == FIX_HEIGHT)
        {
            Int_led_turn_on(&left_bottom_led);
            Int_led_turn_on(&right_bottom_led);
        }
        else
        {
            Int_led_turn_off(&left_bottom_led);
            Int_led_turn_off(&right_bottom_led);
        }

        if (count == 10) 
        {
            count = 0; // Reset count every 1 second
        }

        vTaskDelayUntil(&last_wake_time, LED_TASK_PERIOD);
    }
}

void com_task(void *args)
{
    TickType_t last_wake_time = xTaskGetTickCount();
    while (1)
    {
        uint8_t res = App_receive_data();
        
        // Connection state
        App_process_connect_state(res);

        // Process shutdown 
        if (remote_data.shutdown == 1)
        {
            xTaskNotifyGive(power_task_handle);
        }

        App_process_flight_state();

        vTaskDelayUntil(&last_wake_time, COM_TASK_PERIOD);
    }
}
