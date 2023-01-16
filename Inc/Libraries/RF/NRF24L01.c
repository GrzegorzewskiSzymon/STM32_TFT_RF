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
	SPI3_Receive_8b(data);
}

void Spi_NRF24L01_Send(uint8_t *byte, uint32_t length)
{
	Spi3_Send(byte, length);
}

void Spi_NRF24L01_Transreceive(uint8_t *dataTx, uint16_t lengthTx, uint8_t *dataRx, uint16_t lengthRx)
{
	SPI3_Transreceive_8b(dataTx, lengthTx, dataRx, lengthRx);
}
