/*
 * XPT2046.h
 *
 *  Created on: Jan 11, 2023
 *      Author: Szymon Grzegorzewski
 */

#ifndef LIBRARIES_TOUCHSCREEN_XPT2046_H_
#define LIBRARIES_TOUCHSCREEN_XPT2046_H_

void Spi_XPT2046_Setup();
void Spi_XPT2046_Send(uint8_t *byte, uint32_t length);


#endif /* LIBRARIES_TOUCHSCREEN_XPT2046_H_ */
