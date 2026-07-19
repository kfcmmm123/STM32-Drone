#include "App_receive_data.h"

extern Remote_data remote_data;
extern Remote_State remote_state; 
extern Flight_State flight_state;

Thr_State thr_state = FREE;

uint32_t max_enter_time = 0;
uint32_t min_enter_time = 0;

uint8_t rx_buff[TX_PLOAD_WIDTH] = { 0 };

uint8_t retry_count = 0;

/**
 * @brief Receive data from remote controller
 * 
 * @return uint8_t 0: successful receive, 1: failed receive (no data/check failed)
 */
uint8_t App_receive_data(void)
{
    memset(rx_buff, 0, TX_PLOAD_WIDTH);
    Int_SI24R1_RxPacket(rx_buff);
    if (strlen(rx_buff) == 0)
    {
        return 1; 
    }

    // Header check 
    if (rx_buff[0] != FRAME_HEAD_CHECK_1 || rx_buff[1] != FRAME_HEAD_CHECK_2 || rx_buff[2] != FRAME_HEAD_CHECK_3)
    {
        return 1;
    }

    // Trailer check
    uint32_t sum_receive = 0;
    sum_receive = rx_buff[13] << 24 | rx_buff[14] << 16 | rx_buff[15] << 8 | rx_buff[16];

    uint32_t sum = 0;
    for (uint8_t i = 0; i < 13; i++)
    {
        sum += rx_buff[i];
    }

    if (sum != sum_receive)
    {
        return 1; 
    }

    // Store data
    remote_data.thr = rx_buff[3] << 8 | rx_buff[4];
    remote_data.yaw = rx_buff[5] << 8 | rx_buff[6];
    remote_data.pit = rx_buff[7] << 8 | rx_buff[8];
    remote_data.rol = rx_buff[9] << 8 | rx_buff[10];
    remote_data.shutdown = rx_buff[11];
    remote_data.fix_height = rx_buff[12];

    return 0;
}

/**
 * @brief Process connection state 
 */
void App_process_connect_state(uint8_t res)
{
    if (res == 0)
    {
        remote_state = REMOTE_CONNECTED; 
        retry_count = 0;
    }
    else if (res == 1)
    {
        retry_count++;
        if (retry_count == MAX_RETRY_TIMES)
        {
            remote_state = REMOTE_DISCONNECTED;
            retry_count = 0;
        }
    }
}

/**
 * @brief Process unlock logic 
 * 
 * @return uint8_t 0: unlock successful, 1: unlock failed 
 */
static uint8_t App_process_unlock(void)
{
    switch(thr_state)
    {
        case FREE:
            if (remote_data.thr >= 900)
            {
                thr_state = MAX;
                max_enter_time = xTaskGetTickCount();
            }
            break;
        case MAX:
            if (remote_data.thr < 900)
            {
                if (xTaskGetTickCount() - max_enter_time >= 1000)
                {
                    thr_state = LEAVE_MAX; 
                }
                else 
                {
                    thr_state = FREE;
                }
            }
            break;
        case LEAVE_MAX:
            if (remote_data.thr <= 100)
            {
                thr_state = MIN;
                min_enter_time = xTaskGetTickCount();
            }
            break;
        case MIN:
            if (xTaskGetTickCount() - min_enter_time <= 1000)
            {
                if (remote_data.thr > 100)
                {
                    thr_state = FREE;
                }
            }
            else 
            {
                thr_state = UNLOCK;
            }
        case UNLOCK:
            break;
        default:
            break;
    }

    if (thr_state == UNLOCK)
    {
        return 0;
    }
    return 1;
}

/**
 * @brief Process flight state 
 */
void App_process_flight_state(void)
{
    switch(flight_state)
    {
        case IDLE:
            if (App_process_unlock() == 0)
            {
                flight_state = NORMAL; 
                thr_state = FREE;
            }
            break;
        case NORMAL:
            // Fix height state  
            if (remote_data.fix_height == 1)
            {
                flight_state = FIX_HEIGHT;
                remote_data.fix_height = 0;
            }

            // Remote disconnecteed state
            if (remote_state == REMOTE_DISCONNECTED)
            {
                flight_state = FAIL;
            }
            break;
        case FIX_HEIGHT:
            // Normal state 
            if (remote_data.fix_height == 1)
            {
                flight_state = NORMAL;
                remote_data.fix_height = 0;
            }

            // Remote disconnecteed state
            if (remote_state == REMOTE_DISCONNECTED)
            {
                flight_state = FAIL;
            }
            
            break;
        case FAIL:
            vTaskDelay(1);
            flight_state = IDLE;
            break;
        default: 
            break;
    }
}
