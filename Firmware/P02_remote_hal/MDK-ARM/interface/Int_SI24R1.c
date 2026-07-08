#include "Int_SI24R1.h"

// Define a static TX address => TX and RX address must be the same
uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x0A, 0x01, 0x06, 0x1E, 0x01};

// SPI read/write one byte
static uint8_t SPI_RW(uint8_t byte)
{
	uint8_t rx_data = 0;
	HAL_SPI_TransmitReceive(&hspi1, &byte, &rx_data, 1, 1000);
	return rx_data;
}

/**
 * @brief Write one byte to the SI24R1 module
 * @param reg: Register address (format: SI24R1_WRITE_REG | reg)
 * @param value: Value to write
 * @return Status register value
 */
uint8_t Int_SI24R1_Write_Reg(uint8_t reg, uint8_t value)
{
	uint8_t status;

	CS_LOW;
	status = SPI_RW(reg);
	SPI_RW(value);
	CS_HIGH;

	return (status);
}

/**
 * @brief Write multiple bytes to the SI24R1 module
 * @param reg: Register address (format: SI24R1_WRITE_REG | reg)
 * @param pBuf: Pointer to the buffer containing the data to write
 * @param size: Number of bytes to write
 * @return Status register value
 */
uint8_t Int_SI24R1_Write_Buf(uint8_t reg, const uint8_t *pBuf, uint8_t size)
{
	uint8_t status, byte_ctr;

	CS_LOW;
	status = SPI_RW(reg);
	for (byte_ctr = 0; byte_ctr < size; byte_ctr++)
	{
		SPI_RW(*pBuf++);
	}

	CS_HIGH;

	return (status);
}

/**
 * @brief Read one byte from the SI24R1 module
 * @param reg: Register address (format: SI24R1_READ_REG | reg)
 * @return Value read from the register
 */
uint8_t Int_SI24R1_Read_Reg(uint8_t reg)
{
	uint8_t value;

	CS_LOW;
	SPI_RW(reg);
	value = SPI_RW(0);
	CS_HIGH;

	return (value);
}

/**
 * @brief Read multiple bytes from the SI24R1 module
 * @param reg: Register address (format: SI24R1_READ_REG | reg)
 * @param pBuf: Pointer to the buffer to store the read data
 * @param size: Number of bytes to read
 * @return Status register value
 */
uint8_t Int_SI24R1_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t size)
{
	uint8_t status, byte_ctr;

	CS_LOW;
	status = SPI_RW(reg);
	for (byte_ctr = 0; byte_ctr < size; byte_ctr++)
	{
		pBuf[byte_ctr] = SPI_RW(0); // Read data from the SI24R1 module
	}
	CS_HIGH;

	return (status);
}

/**
 * @brief Set the SI24R1 module to RX mode
 */
void Int_SI24R1_RX_Mode(void)
{
	CE_LOW;
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_AA, 0x01);
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_RXADDR, 0x01);
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_CH, CHANNEL);
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RX_PW_P0, TX_PLOAD_WIDTH);
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_SETUP, 0x06);
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + CONFIG, 0x0f);
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, 0xff);
	CE_HIGH;
}

/**
 * @brief Set the SI24R1 module to TX mode
 */
void Int_SI24R1_TX_Mode(void)
{
	CE_LOW;
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH);
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_AA, 0x01);
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + EN_RXADDR, 0x01);
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + SETUP_RETR, 0x0a);
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_CH, CHANNEL);
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + RF_SETUP, 0x06);
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + CONFIG, 0x0e);
	CE_HIGH;
}

/**
 * @brief Read received data from the RX FIFO and check if data is received
 * @param rxbuf: Pointer to the buffer to store the received data
 * @return Status value (0: Data received, 1: No data received)
 */
uint8_t Int_SI24R1_RxPacket(uint8_t *rxbuf)
{
	uint8_t state;
	// Write 1 to the STATUS register to clear the RX_DR flag
	state = Int_SI24R1_Read_Reg(STATUS);					
	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, state); 

	if (state & RX_DR) // Received data available
	{
		Int_SI24R1_Read_Buf(RD_RX_PLOAD, rxbuf, TX_PLOAD_WIDTH); // Read data from RX FIFO
		Int_SI24R1_Write_Reg(FLUSH_RX, 0xff);					 // Clear RX FIFO
		return 0;
	}
	return 1; // No data received
}

/**
 * @brief Write a packet to the SI24R1 module
 * @param txbuf: Pointer to the buffer containing the data to transmit
 * @return Status value (0: Transmission successful, 1: Transmission failed)
 */
uint8_t Int_SI24R1_TxPacket(uint8_t *txbuf)
{
	uint8_t state;
	CE_LOW;													  // CE low to enable configuration
	Int_SI24R1_Write_Buf(WR_TX_PLOAD, txbuf, TX_PLOAD_WIDTH); // Write data to TX FIFO, 32 bytes
	CE_HIGH;												  // CE high to enable transmission

	state = Int_SI24R1_Read_Reg(STATUS); // Read the STATUS register 
	while (((state & TX_DS) == 0) && ((state & MAX_RT) == 0))
	{
		state = Int_SI24R1_Read_Reg(STATUS);
		vTaskDelay(1); // Delay to avoid busy waiting
	}

	Int_SI24R1_Write_Reg(SI24R1_WRITE_REG + STATUS, state); // Clear TX_DS and MAX_RT flags
	if (state & MAX_RT)										// Maximum retries exceeded
	{
		Int_SI24R1_Write_Reg(FLUSH_TX, 0xff); // Clear TX FIFO
		return 1;
	}
	if (state & TX_DS) // Transmission successful
	{
		return 0;
	}
	return 1; // Transmission failed
}

uint8_t si24r1_rx_buff[5] = {0};

/**
 * @brief Check if the SI24R1 module is initialized successfully
 *
 * @return Status value (0: Initialization successful, 1: Initialization failed)
 */
uint8_t Int_SI24R1_Check(void)
{

	// 1. Check if SPI communication is working properly by reading and writing the TX address.
	// 1.0 Read data first to ensure the SPI communication is functioning correctly
	Int_SI24R1_Read_Buf(SI24R1_READ_REG + TX_ADDR, si24r1_rx_buff, TX_ADR_WIDTH);

	// 1.1 Write the TX address
	Int_SI24R1_Write_Buf(SI24R1_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);

	// 1.2 Read back the TX address to verify if it matches the written value
	Int_SI24R1_Read_Buf(SI24R1_READ_REG + TX_ADDR, si24r1_rx_buff, TX_ADR_WIDTH);

	for (uint8_t i = 0; i < TX_ADR_WIDTH; i++)
	{
		if (si24r1_rx_buff[i] != TX_ADDRESS[i])
		{
			return 1;
		}
	}
	return 0;
}

/**
 * @brief Initialize the SI24R1 module
 *
 */
void Int_SI24R1_Init(void)
{
	HAL_Delay(200);
	// Check if the SI24R1 module is initialized successfully
	while (Int_SI24R1_Check() == 1)
	{
		// Check every 10ms
		HAL_Delay(10);
	}

	// Set default state to receive mode => Every time data is received, switch to receive mode
	Int_SI24R1_RX_Mode();
	debug_printf("SI24R1 Init Success!\r\n");
}
