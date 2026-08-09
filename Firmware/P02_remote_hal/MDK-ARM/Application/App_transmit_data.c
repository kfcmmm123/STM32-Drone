#include "App_transmit_data.h"

extern Remote_Data remote_data;

uint8_t tx_buff[TX_PLOAD_WIDTH] = {0};

uint8_t post_buff[TX_PLOAD_WIDTH] = {0}; 

/**
 * @brief Transmit data to flight controller 
 */
void App_transmit_data(void)
{
    uint32_t sum = 0;

    // Packet is 17 bytes => header check 3 bytes + data 10 bytes + CRC 4 bytes 
    
    // Header check
    tx_buff[0] = FRAME_HEAD_CHECK_1;
    tx_buff[1] = FRAME_HEAD_CHECK_2;
    tx_buff[2] = FRAME_HEAD_CHECK_3;

    // Remote data
    tx_buff[3] = (remote_data.thr >> 8) & 0xFF; 
    tx_buff[4] = remote_data.thr & 0xFF; 

    tx_buff[5] = (remote_data.yaw >> 8) & 0xFF; 
    tx_buff[6] = remote_data.yaw & 0xFF; 

    tx_buff[7] = (remote_data.pit >> 8) & 0xFF; 
    tx_buff[8] = remote_data.pit & 0xFF; 

    tx_buff[9] = (remote_data.rol >> 8) & 0xFF; 
    tx_buff[10] = remote_data.rol & 0xFF; 

    taskENTER_CRITICAL();
    tx_buff[11] = (remote_data.shutdown >> 8); 
    remote_data.shutdown = 0;
    tx_buff[12] = remote_data.fix_height; 
    remote_data.fix_height = 0;
    taskEXIT_CRITICAL();

    for (uint8_t i = 0; i < 13; i++)
    {
        sum += tx_buff[i];
    }

    tx_buff[13] = (sum >> 24) & 0xFF;
    tx_buff[14] = (sum >> 16) & 0xFF;
    tx_buff[15] = (sum >> 8) & 0xFF;
    tx_buff[16] = sum & 0xFF;

    // Start transmit mode
    Int_SI24R1_TX_Mode();

    // Transmit data
    uint8_t res = Int_SI24R1_TxPacket(tx_buff);

    // Switch back to receive mode
    Int_SI24R1_RX_Mode();

    if (res == 0)
    {
        while (Int_SI24R1_RxPacket(post_buff) == 1)
        {
            // Wait for acknowledgment
        }
    }
}
