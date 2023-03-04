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

	}
	else //Horizontal
	{

	}


}
