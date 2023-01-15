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
#include "../TFT_ILI9341/TFT_ILI9341.h"

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
uint8_t Spi_XPT2046_Receive(uint8_t *byte)
{
	return Spi2_Receive_8b(byte);
}

void Spi_XPT2046_Transreceive(uint8_t *dataTx, uint16_t lengthTx, uint8_t *dataRx, uint16_t lengthRx )
{
	Spi2_Transreceive_8b(dataTx, lengthTx, dataRx, lengthRx);
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

#if (ILI9341_ROTATION == 0)
CalibData_t CalibrationData = {-.0009337, -.0636839, 250.342, -.0889775, -.00118110, 356.538}; // default calibration data
#endif
#if (ILI9341_ROTATION == 1)
CalibData_t CalibrationData = {-.0885542, .0016532, 349.800, .0007309, .06543699, -15.290}; // default calibration data
#endif
#if (ILI9341_ROTATION == 2)
CalibData_t CalibrationData = {.0006100, .0647828, -13.634, .0890609, .0001381, -35.73}; // default calibration data
#endif
#if (ILI9341_ROTATION == 3)
CalibData_t CalibrationData = {.0902216, .0006510, -38.657, -.0010005, -.0667030, 258.08}; // default calibration data
#endif

void XPT2046_GetRawData()
{
	Spi2_Transreceive_8b(SendBuffer, 5, ReceiveBuffer, 5);
}

void XPT2046_ReadTouchPoint(uint16_t *x, uint16_t *y)
{
	XPT2046_GetRawData();
	uint16_t _x, _y;
	_x = (uint16_t)((ReceiveBuffer[0]<<8) | (ReceiveBuffer[1]));
	_y = (uint16_t)((ReceiveBuffer[2]<<8) | (ReceiveBuffer[3]));

	*x = CalibrationData.alpha_x * _x + CalibrationData.beta_x * _y + CalibrationData.delta_x;
	*y= CalibrationData.alpha_y * _x + CalibrationData.beta_y * _y + CalibrationData.delta_y;

}


//
// IRQ
//

void XPT2046_IRQ_Setup()
{
	EXTI3_Setup();
}
