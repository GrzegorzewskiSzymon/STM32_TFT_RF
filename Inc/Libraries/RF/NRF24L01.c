/*
 * NRF24L01.c
 *
 *  Created on: Jan 16, 2023
 *      Author: Szymon Grzegorzewski
 */

#include <stdint.h>
#include "stm32g431xx.h"
#include "../RegistersConfig/RegistersConfig.h"

void Spi_NRF24L01_Setup()
{
	Spi3_Setup();
}

void Spi_NRF24L01_Receive(uint8_t *data)
{
	Spi3_Receive_8b(data);
}

void Spi_NRF24L01_Send(uint8_t *byte, uint32_t length)
{
	Spi3_Send(byte, length);
}

void Spi_NRF24L01_Transreceive(uint8_t *dataTx, uint16_t lengthTx, uint8_t *dataRx, uint16_t lengthRx)
{
	Spi3_Transreceive_8b(dataTx, lengthTx, dataRx, lengthRx);
}

void NRF24L01_WriteReg(uint8_t reg, uint8_t data)
{
	uint8_t buf[2];
	buf[0] = (1<<5) | reg;
	buf[1] = data;

	Spi_NRF24L01_Send(buf, 2);

}

void NRF24L01_WriteReg_Multi(uint8_t reg, uint8_t *data, uint16_t size)
{
	uint8_t buf;
	buf = (1<<5) | reg;

	Spi_NRF24L01_Send(&buf, 2);
	Spi_NRF24L01_Send(data, size);

}

uint8_t NRF24L01_ReadReg(uint8_t reg)
{
	uint8_t dataRx;
	Spi_NRF24L01_Transreceive(&reg, 1, &dataRx, 1);
	return dataRx;

}

void NRF24L01_ReadReg_Multi(uint8_t reg, uint8_t *data, uint16_t size)
{
	Spi_NRF24L01_Transreceive(&reg, 1, data, size);
}

void NRF24L01_SendCommand(uint8_t cmd)
{
	Spi_NRF24L01_Send(&cmd, 1);

}
void NRF24L01_Setup()
{


}
