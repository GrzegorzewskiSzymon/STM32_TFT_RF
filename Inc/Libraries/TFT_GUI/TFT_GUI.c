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
#include "../TouchScreen/XPT2046.h"
#include "SettingsImg.h"
#include "ReturnImg.h"
#include "LedRGB.h"

GUI_Data        guiInfo;
enum GUI_Layers guiSelectedLayer;
enum GUI_Layers guiAlreadyDisplayedLayer = 1; //Need to be different than LAYER_DESKTOP (0)
uint8_t isReadyToReadNewTouch = 1;            //If touch was released (to prevent multiple reads of one touch)
uint8_t needToRedrawPartsOfLayer = 0;         //Such as small texts
uint8_t tmpDisplayRotation = VERTICAL;

GUI_BUTTON guiButton_Settings;
GUI_BUTTON guiButton_Return;
GUI_BUTTON guiButton_LedRGB;
GUI_BUTTON guiButton_Rotation;

void GUI_SetButton(GUI_BUTTON *button, uint16_t posX,uint16_t posY, uint16_t width, uint16_t height, uint8_t *imgPointer)
{
	button->posX = posX;
	button->posY = posY;
	button->width = width;
	button->height = height;
	button->imgPointer = imgPointer;
}

void GUI_SetSlider(GUI_SLIDER *slider, uint16_t posX, uint16_t posY, uint16_t width, uint16_t height)
{
	slider->posX = posX;
	slider->posY = posY;
	slider->width = width;
	slider->height = height;
	slider->value = 0;
}

void GUI_SetDefaultSettings()
{
	guiInfo.backgroundColor = ILI9341_DARKGREY;
	guiInfo.displayRotation = VERTICAL;
}

uint16_t posY, posX;
uint8_t GUI_IsButtonTouched(GUI_BUTTON button)
{
	if(button.posX+button.width  > posX && button.posX < posX &&
	   button.posY+button.height > posY && button.posY < posY)
		return 1;
	return 0;
}

uint8_t GUI_UpdateSlider(GUI_SLIDER *slider)
{
	if(slider->posX+slider->width  > posX && slider->posX < posX &&
		slider->posY+slider->height > posY && slider->posY < posY)
	{
		slider->value = slider->posY - posY;
		return 1;
	}
	return 0;
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


void GUI_DisplayDesktopLayer()
{

	if(guiAlreadyDisplayedLayer != guiSelectedLayer) //If Layer was changed and need to be displayed
	{
		/*Exclude possibility of occurring interrupt witch another drawing function (TIM3 displaying battery voltage)
		 *  while displaying this layer*/
		__disable_irq();
		ILI9341_DrawPixel(0, 0, tftWidth, tftHeight, guiInfo.backgroundColor);

		if(guiInfo.displayRotation == VERTICAL)
		{
			GUI_SetButton(&guiButton_Settings,50,40,50,50, (uint8_t*)SettingsImg);
			GUI_SetButton(&guiButton_LedRGB, 140, 40, 50, 50, (uint8_t*)LedRGBImg);

			ILI9341_DrawRoundedRectangle(50, 135, 50, 50, 10, ILI9341_BLACK);
			ILI9341_DrawRoundedRectangle(140, 135, 50, 50, 10, ILI9341_BLACK);

			ILI9341_DrawRoundedRectangle(50, 230, 50, 50, 10, ILI9341_BLACK);
			ILI9341_DrawRoundedRectangle(140, 230, 50, 50, 10, ILI9341_BLACK);
			batteryDisplayData.posX = 170;
			batteryDisplayData.posY = 3;
		}
		else //Horizontal
		{
			GUI_SetButton(&guiButton_Settings,40,50,50,50, (uint8_t*)SettingsImg);
			GUI_SetButton(&guiButton_LedRGB, 40, 140, 50, 50, (uint8_t*)LedRGBImg);

			ILI9341_DrawRoundedRectangle(135, 50, 50, 50, 10, ILI9341_BLACK);
			ILI9341_DrawRoundedRectangle(135, 140, 50, 50, 10, ILI9341_BLACK);

			ILI9341_DrawRoundedRectangle(230, 50, 50, 50, 10, ILI9341_BLACK);
			ILI9341_DrawRoundedRectangle(230, 140, 50, 50, 10, ILI9341_BLACK);
			batteryDisplayData.posX = 250;
			batteryDisplayData.posY = 3;
		}
		ILI9341_DrawRoundedRectangleButton(guiButton_Settings);
		ILI9341_DrawRoundedRectangleButton(guiButton_LedRGB);

		__enable_irq(); //When everything is drawn enable interrupts
	}
}

void GUI_DisplaySettingsLayer()
{
	/*Exclude possibility of occurring interrupt witch another drawing function (TIM3 displaying battery voltage)
	 *  while displaying this layer*/
	__disable_irq();

	if(guiAlreadyDisplayedLayer != guiSelectedLayer) //If Layer was changed and need to be displayed
	{
		ILI9341_DrawPixel(0, 0, tftWidth, tftHeight, guiInfo.backgroundColor);

		if(guiInfo.displayRotation == VERTICAL)
		{
			GUI_SetButton(&guiButton_Return, 0, 0, 50, 50, (uint8_t *)returnImg);

			GUI_SetButton(&guiButton_Rotation, 70, 70, 80, 20, 0);
			ILI9341_DrawString(2, guiButton_Rotation.posY + (guiButton_Rotation.height/2)-4, "Rotation", ILI9341_BLACK, guiInfo.backgroundColor, 1);
		}
		else //Horizontal
		{
			GUI_SetButton(&guiButton_Return, 0, 0, 50, 50, (uint8_t *)returnImg);

			GUI_SetButton(&guiButton_Rotation, 70, 70, 80, 20, 0);
			ILI9341_DrawString(2, guiButton_Rotation.posY + (guiButton_Rotation.height/2)-4, "Rotation", ILI9341_BLACK, guiInfo.backgroundColor, 1);
		}
		ILI9341_DrawRoundedRectangleButton(guiButton_Return);
		ILI9341_DrawRoundedRectangle(guiButton_Rotation.posX, guiButton_Rotation.posY, guiButton_Rotation.width, guiButton_Rotation.height, 4, ILI9341_BLACK);
	}

	//Draw rotation button
	if(needToRedrawPartsOfLayer || guiAlreadyDisplayedLayer != guiSelectedLayer)
	{
		if(tmpDisplayRotation == VERTICAL)
			ILI9341_DrawString(guiButton_Rotation.posX + 5, guiButton_Rotation.posY + (guiButton_Rotation.height/2)-4, "VERTICAL", ILI9341_BLACK, guiInfo.backgroundColor, 1);
		else
			ILI9341_DrawString(guiButton_Rotation.posX + 5, guiButton_Rotation.posY + (guiButton_Rotation.height/2)-4, "HORIZONTAL", ILI9341_BLACK, guiInfo.backgroundColor, 1);
		needToRedrawPartsOfLayer = 0;
	}

	__enable_irq();//When everything is drawn enable interrupts
}

GUI_SLIDER redSlider;
GUI_SLIDER greenSlider;
GUI_SLIDER blueSlider;
void GUI_DisplayLedRGBLayer()
{
	/*Exclude possibility of occurring interrupt witch another drawing function (TIM3 displaying battery voltage)
	 *  while displaying this layer*/
	__disable_irq();

	if(guiAlreadyDisplayedLayer != guiSelectedLayer) //If Layer was changed and need to be displayed
	{
		ILI9341_DrawPixel(0, 0, tftWidth, tftHeight, guiInfo.backgroundColor);

		if(guiInfo.displayRotation == VERTICAL)
		{
			GUI_SetButton(&guiButton_Return, 0, 0, 50, 50, (uint8_t *)returnImg);

			GUI_SetSlider(&redSlider, 70, 50, 30, 256);
			GUI_SetSlider(&greenSlider, 110, 50, 30, 256);
			GUI_SetSlider(&blueSlider, 150, 50, 30, 256);

		}
		else //Horizontal
		{
			GUI_SetButton(&guiButton_Return, 0, 0, 50, 50, (uint8_t *)returnImg);
		}
		ILI9341_DrawRoundedRectangleButton(guiButton_Return);
		ILI9341_DrawRectangleSlider(redSlider);
		ILI9341_DrawRectangleSlider(greenSlider);
		ILI9341_DrawRectangleSlider(blueSlider);
	}

	if(needToRedrawPartsOfLayer || guiAlreadyDisplayedLayer != guiSelectedLayer)
	{
		ILI9341_DrawPixel(redSlider.posX+1, redSlider.posY+1,                                  redSlider.width - 2, redSlider.height -2, guiInfo.backgroundColor);
		ILI9341_DrawPixel(redSlider.posX+1, redSlider.posY+redSlider.height - redSlider.value, redSlider.width - 2, redSlider.value - 1, ILI9341_RED);

		ILI9341_DrawPixel(greenSlider.posX+1, greenSlider.posY+1,                                  greenSlider.width - 2, greenSlider.height -2, guiInfo.backgroundColor);
		ILI9341_DrawPixel(greenSlider.posX+1, greenSlider.posY+greenSlider.height - greenSlider.value, greenSlider.width - 2, greenSlider.value - 1, ILI9341_GREEN);

		ILI9341_DrawPixel(blueSlider.posX+1, blueSlider.posY+1,                                  blueSlider.width - 2, blueSlider.height -2, guiInfo.backgroundColor);
		ILI9341_DrawPixel(blueSlider.posX+1, blueSlider.posY+blueSlider.height - blueSlider.value, blueSlider.width - 2, blueSlider.value - 1, ILI9341_BLUE);

		needToRedrawPartsOfLayer = 0;
	}
	__enable_irq();//When everything is drawn enable interrupts
}

void GUI_Display()
{
	switch (guiSelectedLayer) {
		case LAYER_DESKTOP:
			if(guiInfo.displayRotation != tmpDisplayRotation)
			{
				guiInfo.displayRotation = tmpDisplayRotation;
				ILI9341_setRotation();
			}
			GUI_DisplayDesktopLayer();
			guiAlreadyDisplayedLayer = LAYER_DESKTOP;
			break;

		case LAYER_SETTINGS:
			GUI_DisplaySettingsLayer();
			guiAlreadyDisplayedLayer = LAYER_SETTINGS;
			break;

		case LAYER_LEDRGB:
			GUI_DisplayLedRGBLayer();
			guiAlreadyDisplayedLayer = LAYER_LEDRGB;
			break;
	}
}

void GUI_TouchCheck()
{
	switch (guiSelectedLayer) {
		case LAYER_DESKTOP:
			if(GUI_IsButtonTouched(guiButton_Settings))
			{
				guiSelectedLayer = LAYER_SETTINGS;
			}
			else if(GUI_IsButtonTouched(guiButton_LedRGB))
			{
				guiSelectedLayer = LAYER_LEDRGB;
			}
			break;

		case LAYER_SETTINGS:
			if(GUI_IsButtonTouched(guiButton_Return))
			{
				guiSelectedLayer = LAYER_DESKTOP;
			}
			if(GUI_IsButtonTouched(guiButton_Rotation))
			{
				if(tmpDisplayRotation == VERTICAL )
					tmpDisplayRotation = HORIZONTAL;
				else
					tmpDisplayRotation = VERTICAL;
				needToRedrawPartsOfLayer = 1;
			}
			break;

		case LAYER_LEDRGB:
			if(GUI_IsButtonTouched(guiButton_Return))
			{
				guiSelectedLayer = LAYER_DESKTOP;
			}
			if(GUI_UpdateSlider(&redSlider) || GUI_UpdateSlider(&greenSlider) || GUI_UpdateSlider(&blueSlider))
				needToRedrawPartsOfLayer = 1; //Update Sliders
			break;
	}
}

void GUI_Run()
{
	if(XPT2046_IsTouched())
	{
		if(guiInfo.displayRotation == VERTICAL)
			XPT2046_GetTouchPoint(&posX, &posY);
		else
		{
			XPT2046_GetTouchPoint(&posY, &posX);
			posX = tftWidth - posX;
		}

		if(isReadyToReadNewTouch)
		{
			GUI_TouchCheck();
			isReadyToReadNewTouch = 0; //Not ready
		}
	}
	else
		isReadyToReadNewTouch = 1;//Ready

	GUI_Display();
}
