#ifndef __nRF24L01P__
#define __nRF24L01P__

#include "spi.h"
#include "Com_debug.h"
#include "FreeRTOS.h"
#include "task.h"

// Pull-down CS
#define CS_LOW HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_RESET);
// Pull-up CS
#define CS_HIGH HAL_GPIO_WritePin(SPI1_NSS_GPIO_Port, SPI1_NSS_Pin, GPIO_PIN_SET);

// Pull-down CE
#define CE_LOW HAL_GPIO_WritePin(SI_EN_GPIO_Port, SI_EN_Pin, GPIO_PIN_RESET);
// Pull-up CE
#define CE_HIGH HAL_GPIO_WritePin(SI_EN_GPIO_Port, SI_EN_Pin, GPIO_PIN_SET);

// TX configuration
#define CHANNEL 40
#define TX_ADR_WIDTH 5    
#define TX_PLOAD_WIDTH 32 

//********************************************************************************************************************//
// SPI(SI24R1) commands
#define SI24R1_READ_REG 0x00  // Define read command to register
#define SI24R1_WRITE_REG 0x20 // Define write command to register
#define RD_RX_PLOAD 0x61      // Define RX payload register address
#define WR_TX_PLOAD 0xA0      // Define TX payload register address
#define FLUSH_TX 0xE1         // Define flush TX register command
#define FLUSH_RX 0xE2         // Define flush RX register command
#define REUSE_TX_PL 0xE3      // Define reuse TX payload register command
#define NOP 0xFF              // Define No Operation, might be used to read status register

//********************************************************************************************************************//
// SPI(SI24R1) registers(addresses)
#define CONFIG 0x00      // 'Config' register address
#define EN_AA 0x01       // 'Enable Auto Acknowledgment' register address
#define EN_RXADDR 0x02   // 'Enabled RX addresses' register address
#define SETUP_AW 0x03    // 'Setup address width' register address
#define SETUP_RETR 0x04  // 'Setup Auto. Retrans' register address
#define RF_CH 0x05       // 'RF channel' register address
#define RF_SETUP 0x06    // 'RF setup' register address
#define STATUS 0x07      // 'Status' register address
#define OBSERVE_TX 0x08  // 'Observe TX' register address
#define RSSI 0x09        // 'Received Signal Strength Indecator' register address
#define RX_ADDR_P0 0x0A  // 'RX address pipe0' register address
#define RX_ADDR_P1 0x0B  // 'RX address pipe1' register address
#define RX_ADDR_P2 0x0C  // 'RX address pipe2' register address
#define RX_ADDR_P3 0x0D  // 'RX address pipe3' register address
#define RX_ADDR_P4 0x0E  // 'RX address pipe4' register address
#define RX_ADDR_P5 0x0F  // 'RX address pipe5' register address
#define TX_ADDR 0x10     // 'TX address' register address
#define RX_PW_P0 0x11    // 'RX payload width, pipe0' register address
#define RX_PW_P1 0x12    // 'RX payload width, pipe1' register address
#define RX_PW_P2 0x13    // 'RX payload width, pipe2' register address
#define RX_PW_P3 0x14    // 'RX payload width, pipe3' register address
#define RX_PW_P4 0x15    // 'RX payload width, pipe4' register address
#define RX_PW_P5 0x16    // 'RX payload width, pipe5' register address
#define FIFO_STATUS 0x17 // 'FIFO Status Register' register address

//********************************************************************************************************************//
// STATUS Register
#define RX_DR 0x40 /**/
#define TX_DS 0x20
#define MAX_RT 0x10

//********************************************************************************************************************//
//                                        FUNCTION's PROTOTYPES                                                       //
//********************************************************************************************************************//
// SI24R1 API Functions

/**
 * @brief Write one byte to the SI24R1 module
 * @param reg: Register address (format: SI24R1_WRITE_REG | reg)
 * @param value: Value to write
 * @return Status register value
 */
uint8_t Int_SI24R1_Write_Reg(uint8_t reg, uint8_t value);

/**
 * @brief Write multiple bytes to the SI24R1 module
 * @param reg: Register address (format: SI24R1_WRITE_REG | reg)
 * @param pBuf: Pointer to the buffer containing the data to write
 * @param size: Number of bytes to write
 * @return Status register value
 */
uint8_t Int_SI24R1_Write_Buf(uint8_t reg, const uint8_t *pBuf, uint8_t size);

/**
 * @brief Read one byte from the SI24R1 module
 * @param reg: Register address (format: SI24R1_READ_REG | reg)
 * @return Value read from the register
 */
uint8_t Int_SI24R1_Read_Reg(uint8_t reg);

/**
 * @brief Read multiple bytes from the SI24R1 module
 * @param reg: Register address (format: SI24R1_READ_REG | reg)
 * @param pBuf: Pointer to the buffer to store the read data
 * @param size: Number of bytes to read
 * @return Status register value
 */
uint8_t Int_SI24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t size);

/**
 * @brief Set the SI24R1 module to RX mode
 */
void Int_SI24R1_RX_Mode(void);

/**
 * @brief Set the SI24R1 module to TX mode
 */
void Int_SI24R1_TX_Mode(void);

/**
 * @brief Read received data from the RX FIFO and check if data is received
 * @param rxbuf: Pointer to the buffer to store the received data
 * @return Status value (0: Data received, 1: No data received)
 */
uint8_t Int_SI24R1_RxPacket(uint8_t *rxbuf);

/**
 * @brief Write a packet to the SI24R1 module
 * @param txbuf: Pointer to the buffer containing the data to transmit
 * @return Status value (0: Transmission successful, 1: Transmission failed)
 */
uint8_t Int_SI24R1_TxPacket(uint8_t *txbuf);

/**
 * @brief Initialize the SI24R1 module
 *
 */
void Int_SI24R1_Init(void); // SI24R1 Pin Init
//********************************************************************************************************************//
#endif
