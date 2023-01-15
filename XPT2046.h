/*
 * XPT2046.h
 *
 *  Created on: Jan 11, 2023
 *      Author: Szymon Grzegorzewski
 */

#ifndef LIBRARIES_TOUCHSCREEN_XPT2046_H_
#define LIBRARIES_TOUCHSCREEN_XPT2046_H_

#define XPT2046_MAX_SAMPLES 10
#define XPT2046_SAMPLE_INTERVAL 5

typedef struct
{
	long double alpha_x;
	long double beta_x;
	long double delta_x;
	long double alpha_y;
	long double beta_y;
	long double delta_y;
} CalibData_t;


typedef enum
{
	XPT2046_IDLE,		 // 0
	XPT2046_PRESAMPLING, // 1
	XPT2046_TOUCHED,	 // 2
	XPT2046_RELEASED	 // 3
} XPT2046_State;

extern volatile XPT2046_State TouchState;

void Spi_XPT2046_Setup();
void Spi_XPT2046_Send(uint8_t *byte, uint32_t length);
void XPT2046_IRQ_Setup();
void XPT2046_Setup();
void XPT2046_Task();
#endif /* LIBRARIES_TOUCHSCREEN_XPT2046_H_ */
