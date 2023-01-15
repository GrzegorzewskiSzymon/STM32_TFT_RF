/*
 * XPT2046.h
 *
 *  Created on: Jan 11, 2023
 *      Author: Szymon Grzegorzewski
 */

#ifndef LIBRARIES_TOUCHSCREEN_XPT2046_H_
#define LIBRARIES_TOUCHSCREEN_XPT2046_H_



typedef struct
{
	long double alpha_x;
	long double beta_x;
	long double delta_x;
	long double alpha_y;
	long double beta_y;
	long double delta_y;
} CalibData_t;





void Spi_XPT2046_Setup();
void Spi_XPT2046_Send(uint8_t *byte, uint32_t length);
void XPT2046_IRQ_Setup();
void XPT2046_Setup();
void XPT2046_ReadTouchPoint(uint16_t *x, uint16_t *y);
void XPT2046_GetRawData();
#endif /* LIBRARIES_TOUCHSCREEN_XPT2046_H_ */
