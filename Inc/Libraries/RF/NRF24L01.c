/*
 * NRF24L01.c
 *
 *  Created on: Jan 16, 2023
 *      Author: Szymon Grzegorzewski
 */

#include <stdint.h>
#include "stm32g431xx.h"
#include "../RegistersConfig/RegistersConfig.h"
#include "NRF24L01.h"

uint8_t rxAddress[] = {0xEE, 0xDD, 0xCC, 0xBB, 0xAA};
uint8_t txAddress[] = {0xEE, 0xDD, 0xCC, 0xBB, 0xAA};
uint8_t rxBuff[32];
uint8_t txBuff[32];

void Spi_NRF24L01_Setup()
{
	Spi3_Setup();
}

void Spi_NRF24L01_Receive(uint8_t *data)
{
	*data = Spi3_Read8();
}

void Spi_NRF24L01_Send(uint8_t *byte, uint32_t length)
{
	Spi3_Send(byte, length);
}

void NRF24L01_WriteReg(uint8_t reg, uint8_t data)
{
	uint8_t buf[2];
	buf[0] = reg|1<<5;
	buf[1] = data;
	NRF24L01_CS_LOW;
	Spi_NRF24L01_Send(buf, 2);
	NRF24L01_CS_HIGH;

}

void NRF24L01_WriteReg_Multi(uint8_t reg, uint8_t *data, uint16_t size)
{
	uint8_t buf;
	buf = (1<<5) | reg;
	NRF24L01_CS_LOW;
	Spi_NRF24L01_Send(&buf, 2);
	Spi_NRF24L01_Send(data, size);
	NRF24L01_CS_HIGH;
}

uint8_t NRF24L01_ReadReg(uint8_t reg)
{
	uint8_t dataRx;
	NRF24L01_CS_LOW;

	Spi_NRF24L01_Send(&reg, 1);
	Spi3_ClearRxBuff();		//clear the reading STATUS register - not important
	dataRx = Spi3_Read8();

	NRF24L01_CS_HIGH;
	return dataRx;

}

void NRF24L01_SendCommand(uint8_t cmd)
{
	NRF24L01_CS_LOW;
	Spi_NRF24L01_Send(&cmd, 1);
	NRF24L01_CS_HIGH;
}

void NRF24L01_Reset(uint8_t REG)
{
	if (REG == STATUS)
	{
		NRF24L01_WriteReg(STATUS, 0x00);
	}
	else if (REG == FIFO_STATUS)
	{
		NRF24L01_WriteReg(FIFO_STATUS, 0x11);
	}
	else
	{
	NRF24L01_WriteReg(CONFIG, 0x08);
	NRF24L01_WriteReg(EN_AA, 0x3F);
	NRF24L01_WriteReg(EN_RXADDR, 0x03);
	NRF24L01_WriteReg(SETUP_AW, 0x03);
	NRF24L01_WriteReg(SETUP_RETR, 0x03);
	NRF24L01_WriteReg(RF_CH, 0x02);
	NRF24L01_WriteReg(RF_SETUP, 0x0E);
	NRF24L01_WriteReg(STATUS, 0x00);
	NRF24L01_WriteReg(OBSERVE_TX, 0x00);
	NRF24L01_WriteReg(CD, 0x00);
	uint8_t rx_addr_p0_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	NRF24L01_WriteReg_Multi(RX_ADDR_P0, rx_addr_p0_def, 5);
	uint8_t rx_addr_p1_def[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
	NRF24L01_WriteReg_Multi(RX_ADDR_P1, rx_addr_p1_def, 5);
	NRF24L01_WriteReg(RX_ADDR_P2, 0xC3);
	NRF24L01_WriteReg(RX_ADDR_P3, 0xC4);
	NRF24L01_WriteReg(RX_ADDR_P4, 0xC5);
	NRF24L01_WriteReg(RX_ADDR_P5, 0xC6);
	uint8_t tx_addr_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
	NRF24L01_WriteReg_Multi(TX_ADDR, tx_addr_def, 5);
	NRF24L01_WriteReg(RX_PW_P0, 0);
	NRF24L01_WriteReg(RX_PW_P1, 0);
	NRF24L01_WriteReg(RX_PW_P2, 0);
	NRF24L01_WriteReg(RX_PW_P3, 0);
	NRF24L01_WriteReg(RX_PW_P4, 0);
	NRF24L01_WriteReg(RX_PW_P5, 0);
	NRF24L01_WriteReg(FIFO_STATUS, 0x11);
	NRF24L01_WriteReg(DYNPD, 0);
	NRF24L01_WriteReg(FEATURE, 0);
	}
}

void NRF24L01_Setup()
{
	//Disable the hip;
	NRF24L01_CE_LOW;

	NRF24L01_Reset(0);

	NRF24L01_WriteReg(CONFIG, 0);
	NRF24L01_WriteReg(EN_AA, 0);       // No auto ACK
	NRF24L01_WriteReg(EN_RXADDR, 0);   // Not enabling any data pipe now
	NRF24L01_WriteReg(SETUP_AW, 0x03); // 5 Bytes
	NRF24L01_WriteReg(SETUP_RETR, 0);
	NRF24L01_WriteReg(RF_CH, 0);
	NRF24L01_WriteReg(RF_SETUP, 0x0E); // Power = 0db, data rate = 2Mbps

	//Enable the hip;
	NRF24L01_CE_HIGH;
}

void NRF24L01_Mode_Tx(uint8_t *address, uint8_t channel)
{
	//Disable the hip;
	NRF24L01_CE_LOW;

	NRF24L01_WriteReg(RF_CH, channel);
	NRF24L01_WriteReg_Multi(TX_ADDR, address, 5);

	// Power up the device
	uint8_t config = NRF24L01_ReadReg(CONFIG);
	config = config | (1<<1);
	NRF24L01_WriteReg(CONFIG, config);

	//Enable the hip;
	NRF24L01_CE_HIGH;
}

uint8_t NRF24L01_Transmit(uint8_t *data)
{
	uint8_t cmdToSend = W_TX_PAYLOAD;

	NRF24L01_CS_LOW;

	Spi_NRF24L01_Send(&cmdToSend, 1);

	Spi_NRF24L01_Send(data, 32);

	NRF24L01_CS_HIGH;
	uint64_t tmp_ms = ms;
	while((ms - tmp_ms) < 2)
	{
	}

	uint8_t fifoStatus = NRF24L01_ReadReg(FIFO_STATUS);

	if( (fifoStatus&(1<<4)) && (!(fifoStatus&(1<<3))) )
	{
		cmdToSend = FLUSH_TX;
		NRF24L01_SendCommand(cmdToSend);
		NRF24L01_Reset(FIFO_STATUS);
		return 0;
	}
	return 1;
}

void NRF24L01_Mode_Rx(uint8_t *Address, uint8_t channel)
{
	//Disable the hip;
	NRF24L01_CE_LOW;

	NRF24L01_Reset(STATUS);

	NRF24L01_WriteReg(RF_CH, channel);  // select the channel

	// select data pipe 1
	uint8_t en_rxaddr = NRF24L01_ReadReg(EN_RXADDR);
	en_rxaddr = en_rxaddr | (1<<1);
	NRF24L01_WriteReg(EN_RXADDR, en_rxaddr);

	NRF24L01_WriteReg_Multi(RX_ADDR_P1, Address, 5);  // Write the Pipe1 address
//	NRF24L01_WriteReg(RX_ADDR_P2, 0xEE);  // Write the Pipe2 LSB address

	NRF24L01_WriteReg(RX_PW_P1, 32);   // 32 bit payload size for pipe 2


	// power up the device in Rx mode
	uint8_t config = NRF24L01_ReadReg(CONFIG);
	config = config | (1<<1) | (1<<0);
	NRF24L01_WriteReg(CONFIG, config);

	//Enable the hip;
	NRF24L01_CE_HIGH;
}

uint8_t NRF24L01_isDataAvailable(int pipenum)
{
	uint8_t status = NRF24L01_ReadReg(STATUS);

	if ((status&(1<<6))&&(status&(pipenum<<1)))
	{

		NRF24L01_WriteReg(STATUS, (1<<6));

		return 1;
	}

	return 0;
}

uint8_t blankData[32];
void NRF24L01_Receive(uint8_t *data)
{
	uint8_t cmdtosend = 0;

	// select the device
	NRF24L01_CS_LOW;;

	// payload command
	cmdtosend = R_RX_PAYLOAD;
	Spi_NRF24L01_Send(&cmdtosend, 1);

	SPI_NRF24L01_ENABLE;
	Spi3_ClearRxBuff();

	// Receive the payload
	Spi3_Read8_Multi(data, 32);
	SPI_NRF24L01_DISABLE;

	// Unselect the device
	NRF24L01_CS_HIGH;

	uint64_t tmp_ms = ms;
	while((ms - tmp_ms) < 2)
	{
	}

	cmdtosend = FLUSH_RX;
	NRF24L01_SendCommand(cmdtosend);
}


//
//
//

void RF_SendData(uint8_t *address, uint8_t channel, uint8_t nrOfFunction, uint8_t* data, uint8_t length )
{
	//Put NRF is in Transmitting mode and set address and channel
	NRF24L01_Mode_Tx(address, channel);

	uint8_t tab[32];
	tab[0] = nrOfFunction;
	for(int i = 1; i <32; i++)
	{
		if(length >= i)
			tab[i] = data[i-1];
		else
			tab[i] = 0;
	}

	//combine nrOfFunction and data and send
	NRF24L01_Transmit(tab);
}

