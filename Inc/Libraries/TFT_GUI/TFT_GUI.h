/*
 * TFT_GUI.h
 *
 *  Created on: 4 Mar 2023
 *      Author: Szymon Grzegorzewski
 */
#ifndef LIBRARIES_TFT_GUI_TFT_GUI_H_
#define LIBRARIES_TFT_GUI_TFT_GUI_H_

enum GUI_Layers
{
	DESKTOP = 0,
	SETTINGS

};

#define VERTICAL 1
#define HORIZONTAL 0

typedef struct
{
	uint16_t backgroundColor;
	uint8_t  displayRotation; // 1-Vertical, 0-Horizontal

}GUI_Data;

extern GUI_Data guiInfo;

typedef struct
{
	uint16_t posX;
	uint16_t posY;

	uint16_t width;
	uint16_t height;

	uint8_t *imgPointer;

}GUI_BUTTON;

typedef struct
{
	uint16_t posX;
	uint16_t posY;

}GUI_DISPLAY_BATTERY_DATA;
extern GUI_DISPLAY_BATTERY_DATA batteryDisplayData;

void GUI_SetDefaultSettings();
void GUI_DisplayDesktopLayer();

#endif /* LIBRARIES_TFT_GUI_TFT_GUI_H_ */
