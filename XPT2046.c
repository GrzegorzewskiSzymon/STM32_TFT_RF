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


static uint8_t ChannelSettingsX, ChannelSettingsY;
static uint8_t SendBuffer[5];
static uint8_t ReceiveBuffer[5];

void XPT2046_Setup()
{
	//
	// Prepare Send Buffer
	//

	//     (     X    )           (     Y 	 )
	// (000 10010)(000 00000) (000 11010)(000 00000) (00000000)
	//	SendBuffer
	// (    0    )(    1    ) (    2    )(    3    ) (    4   )
	ChannelSettingsX = 0b10010000;
	ChannelSettingsY = 0b11010000;

	SendBuffer[0] = 0x80; // Clear settings in IC
	Spi_XPT2046_Send(SendBuffer, 5);

	//wait for 2 ms
	uint64_t tmp_ms = ms;
	while((ms - tmp_ms) < 2)
	{
	}

	// Fulfill Send Buffer with Channel control bytes
	SendBuffer[0] = (ChannelSettingsX>>3);
	SendBuffer[1] = (ChannelSettingsX<<5);
	SendBuffer[2] = (ChannelSettingsY>>3);
	SendBuffer[3] = (ChannelSettingsY<<5);
	SendBuffer[4] = 0;

}











//
// IRQ
//

void XPT2046_IRQ_Setup()
{
	EXTI3_Setup();
}
