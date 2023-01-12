/*
 * XPT2046.c
 *
 *  Created on: Jan 11, 2023
 *      Author: Szymon Grzegorzewski
 */
#include <stdint.h>
#include "stm32g431xx.h"
#include "XPT2046.h"
#include "../RegistersConfig/RegistersConfig.h"

//
// SPI
//

void Spi_XPT2046_Setup()
{
	Spi2_Setup();
}

void Spi_XPT2046_Send(uint8_t *byte, uint32_t length)
{
	Spi2_Send(byte, length);
}

