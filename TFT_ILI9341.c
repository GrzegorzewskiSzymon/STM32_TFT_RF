/*
 * TFT_ILI9341.c
 *
 *  Created on: 29 Dec 2022
 *      Author: Szymon Grzegorzewski
 */

#include <stdint.h>
#include "stm32g431xx.h"
#include "../SPI/Spi.h"
#include "TFT_ILI9341.h"
#include "../RegistersConfig/RegistersConfig.h"


void ILI9341_Delay_ms(uint32_t delay)
{
	uint64_t tmp_ms = ms;
	while((ms - tmp_ms) < delay)
	{
	}

}


void ILI9341_sendCommandAndData(uint8_t command, uint8_t *data, uint32_t nrOfData)
{
	SPI1_DC_COMMAND
	Spi1_Send(&command, 1);
	SPI1_DC_DATA;
	Spi1_Send(data, nrOfData);
}



static const uint8_t ILI9341_InitializationData[] =
{
//	0xEF, 3, 0x03, 0x80, 0x02, 				//Not existing in datasheet
//	0xCF, 3, 0x00, 0xC1, 0x30,				//Not existing in datasheet
//	0xED, 4, 0x64, 0x03, 0x12, 0x81, 		//Not existing in datasheet
//	0xE8, 3, 0x85, 0x00, 0x78,             	//Not existing in datasheet
//	0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02, 	//Not existing in datasheet
//	0xF7, 1, 0x20,                         	//Not existing in datasheet
//	0xEA, 2, 0x00, 0x00, 				   	//Not existing in datasheet
	ILI9341_PWCTR1, 1, 0x23,// Power control VRH[5:0] -> 4.60 V
//	ILI9341_PWCTR2, 1, 0x10,        // Power control SAP[2:0];BT[3:0]
	ILI9341_VMCTR1, 2, 0x3e, 0x28,	//VCM control 1  VCOMH voltage = 4.250 V, VCOML voltage = -1.500
	ILI9341_VMCTR2, 1, 0x86, //Set the VCOM offset voltage
	ILI9341_MADCTL, 1, 0x48,        // Memory Access Control
//	ILI9341_VSCRSADD, 1, 0x00,      // Vertical scroll zero
	ILI9341_PIXFMT, 1, 0x55,        //16 bits / pixel
	ILI9341_FRMCTR1, 2, 0x00, 0x18, //Frame Rate Control
	ILI9341_DFUNCTR, 3, 0x08, 0x82, 0x27, // Display Function Control
//	0xF2, 1, 0x00,                         //Not existing in datasheet
//	ILI9341_GAMMASET, 1, 0x01,             // Gamma curve selected
//	ILI9341_GMCTRP1, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
//	0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
//	ILI9341_GMCTRN1, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
//	0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
	ILI9341_SLPOUT, 0x80,                // Exit Sleep
	ILI9341_DISPON, 0x80,                // Display on
	0x00                                 // End of list
};


void ILI9341_Init()
{
	ILI9341_RESET_LOW;
	ILI9341_Delay_ms(10);
	ILI9341_RESET_HIG;
	ILI9341_Delay_ms(10);

	uint8_t cmd, x, numArgs;
	const uint8_t *addr = ILI9341_InitializationData;

	while ((cmd = *(addr++)) > 0)//if there is still data
	{
		x = *(addr++);
		numArgs = x & 0x7F;
		ILI9341_sendCommandAndData(cmd, (uint8_t*) addr, numArgs);
		addr += numArgs;

		if (x & 0x80)//MSB bit of x indicate need of delay
		{
			ILI9341_Delay_ms(150);
		}
	}
}

uint8_t ILI9341_setFrame(uint16_t posX, uint16_t posY, uint16_t width, uint16_t height)
{
	if (posX+width > TFT_WIDTH || posY+height > TFT_HEIGHT)
		return 1;

	uint8_t posY_tab[4];
	uint8_t posX_tab[4];

	posX_tab[0] =   posX >> 8;
	posX_tab[1] =   posX & 0xFF;
	posX_tab[2] =  (posX + width - 1) >> 8;
	posX_tab[3] = ((posX + width - 1) & 0xFF);

	posY_tab[0] =   posY >> 8;
	posY_tab[1] =   posY & 0xFF;
	posY_tab[2] =  (posY + height - 1) >> 8;
	posY_tab[3] = ((posY + height - 1) & 0xFF);

	ILI9341_sendCommandAndData(ILI9341_CASET, posX_tab, 4);
	ILI9341_sendCommandAndData(ILI9341_PASET, posY_tab, 4);

	return 0;
}
//
// Drawing functions
//

void ILI9341_DrawPixel(uint16_t posX, uint16_t posY, uint16_t width, uint16_t height, uint16_t color)
{

	if(ILI9341_setFrame(posX, posY, width, height) )
		return;

	uint8_t color_tab[2];
	color_tab[0] =  color >> 8;
	color_tab[1] =  color & 0xff;

	uint8_t tmp_command = ILI9341_RAMWR;
	SPI1_DC_COMMAND
	Spi1_Send(&tmp_command, 1);
	SPI1_DC_DATA;
	for(uint32_t i = 0; i < width * height; i++)
		Spi1_Send(color_tab, 2);
}

