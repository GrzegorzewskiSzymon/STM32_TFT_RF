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
	guiInfo.displayRotation = VERTICAL;
}

GUI_DISPLAY_BATTERY_DATA batteryDisplayData;
void GUI_DisplayBatteryStatus(uint16_t posX,uint16_t posY)
{
	char str[11];

	str[0] = battery.voltageInteger   + '0';
	str[1] = '.';
	str[2] = battery.voltageFract/100 + '0';
	str[3] = (battery.voltageFract%100)/10  + '0';
	str[4] = battery.voltageFract%10  + '0';
	str[5] = 'v';
	str[6] = ' ';
	str[7] = battery.voltagePercentage/100 + '0';
	str[8] = (battery.voltagePercentage%100)/10 + '0';
	str[9] = battery.voltagePercentage%10  + '0';
	str[10] = '%';

	if(str[7] == '0')
		str[7] = ' ';

	ILI9341_DrawString(posX, posY, str, ILI9341_BLACK, guiInfo.backgroundColor, 1);
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
		batteryDisplayData.posX = 170;
		batteryDisplayData.posY = 3;
	}
	else //Horizontal
	{
		GUI_SetButton(&guiButton_Settings,40,50,50,50);
		ILI9341_DrawRoundedRectangle(40, 140, 50, 50, 10, ILI9341_BLACK);

		ILI9341_DrawRoundedRectangle(135, 50, 50, 50, 10, ILI9341_BLACK);
		ILI9341_DrawRoundedRectangle(135, 140, 50, 50, 10, ILI9341_BLACK);

		ILI9341_DrawRoundedRectangle(230, 50, 50, 50, 10, ILI9341_BLACK);
		ILI9341_DrawRoundedRectangle(230, 140, 50, 50, 10, ILI9341_BLACK);
		batteryDisplayData.posX = 250;
		batteryDisplayData.posY = 3;
	}
	ILI9341_DrawRoundedRectangleButton(guiButton_Settings);
}


