#include "App_receive_data.h"

extern Remote_data remote_data;
extern Remote_State remote_state; 

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
