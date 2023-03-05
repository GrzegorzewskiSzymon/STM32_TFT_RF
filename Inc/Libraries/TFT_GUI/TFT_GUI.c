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

void GUI_SetButton(GUI_BUTTON *button, uint16_t posX,uint16_t posY, uint16_t width, uint16_t height)
{
	button->posX = posX;
	button->posY = posY;
	button->width = width;
	button->height = height;
}

void GUI_SetDefaultSettings()
{
	guiInfo.backgroundColor = ILI9341_DARKGREY;
	guiInfo.displayRotation = HORIZONTAL;
}

GUI_BUTTON guiButton_Settings;
void GUI_DisplayDesktopLayer()
{

	if(guiSelectedLayer != DESKTOP)
	{
		return;
	}

	ILI9341_DrawPixel(0, 0, tftWidth, tftHeight, guiInfo.backgroundColor);

	if(guiInfo.displayRotation == VERTICAL)
	{
		GUI_SetButton(&guiButton_Settings,50,40,50,50);
		ILI9341_DrawRoundedRectangle(140, 40, 50, 50, 10, ILI9341_BLACK);

		ILI9341_DrawRoundedRectangle(50, 135, 50, 50, 10, ILI9341_BLACK);
		ILI9341_DrawRoundedRectangle(140, 135, 50, 50, 10, ILI9341_BLACK);

		ILI9341_DrawRoundedRectangle(50, 230, 50, 50, 10, ILI9341_BLACK);
		ILI9341_DrawRoundedRectangle(140, 230, 50, 50, 10, ILI9341_BLACK);
	}
	else //Horizontal
	{
		GUI_SetButton(&guiButton_Settings,40,50,50,50);
		ILI9341_DrawRoundedRectangle(40, 140, 50, 50, 10, ILI9341_BLACK);

		ILI9341_DrawRoundedRectangle(135, 50, 50, 50, 10, ILI9341_BLACK);
		ILI9341_DrawRoundedRectangle(135, 140, 50, 50, 10, ILI9341_BLACK);

		ILI9341_DrawRoundedRectangle(230, 50, 50, 50, 10, ILI9341_BLACK);
		ILI9341_DrawRoundedRectangle(230, 140, 50, 50, 10, ILI9341_BLACK);
	}

	ILI9341_DrawRoundedRectangle(guiButton_Settings.posX, guiButton_Settings.posY, guiButton_Settings.width, guiButton_Settings.height, 10, ILI9341_BLACK);
}
