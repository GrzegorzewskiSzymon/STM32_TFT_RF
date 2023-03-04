/*
 * TFT_GUI.h
 *
 *  Created on: 4 Mar 2023
 *      Author: Szymon Grzegorzewski
 */

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

void GUI_SetDefaultSettings();
void GUI_DisplayDesktopLayer();
