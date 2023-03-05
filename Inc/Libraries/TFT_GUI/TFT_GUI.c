/*
 * TFT_GUI.c
 *
 *  Created on: 4 Mar 2023
 *      Author: Szymon Grzegorzewski
 */


#include <stdint.h>
#include "stm32g431xx.h"
#include "TFT_GUI.h"
#include "../TFT_ILI9341/TFT_ILI9341.h"
#include "../PowerManagement/BatteryManagement.h"

GUI_Data        guiInfo;
enum GUI_Layers guiSelectedLayer;


void GUI_SetDefaultSettings()
{
	guiInfo.backgroundColor = ILI9341_DARKGREY;
	guiInfo.displayRotation = HORIZONTAL;
}


void GUI_DisplayDesktopLayer()
{

	if(guiSelectedLayer != DESKTOP)
	{
		return;
	}

	ILI9341_DrawPixel(0, 0, tftWidth, tftHeight, guiInfo.backgroundColor);

	if(guiInfo.displayRotation == VERTICAL)
	{
		ILI9341_DrawRoundedRectangle(50, 40, 50, 50, 10, ILI9341_BLACK);
		ILI9341_DrawRoundedRectangle(140, 40, 50, 50, 10, ILI9341_BLACK);

		ILI9341_DrawRoundedRectangle(50, 135, 50, 50, 10, ILI9341_BLACK);
		ILI9341_DrawRoundedRectangle(140, 135, 50, 50, 10, ILI9341_BLACK);

		ILI9341_DrawRoundedRectangle(50, 230, 50, 50, 10, ILI9341_BLACK);
		ILI9341_DrawRoundedRectangle(140, 230, 50, 50, 10, ILI9341_BLACK);
	}
	else //Horizontal
	{
		ILI9341_DrawRoundedRectangle(40, 50, 50, 50, 10, ILI9341_BLACK);
		ILI9341_DrawRoundedRectangle(40, 140, 50, 50, 10, ILI9341_BLACK);

		ILI9341_DrawRoundedRectangle(135, 50, 50, 50, 10, ILI9341_BLACK);
		ILI9341_DrawRoundedRectangle(135, 140, 50, 50, 10, ILI9341_BLACK);

		ILI9341_DrawRoundedRectangle(230, 50, 50, 50, 10, ILI9341_BLACK);
		ILI9341_DrawRoundedRectangle(230, 140, 50, 50, 10, ILI9341_BLACK);
	}
}
