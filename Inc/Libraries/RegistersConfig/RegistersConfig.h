/*
 * RegistersConfig.h
 *
 *  Created on: Dec 28, 2022
 *      Author: Szymon Grzegorzewski
 */

#ifndef LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_
#define LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_

//
// #define
//

#define PA5_LOW  GPIOA->BSRR = 1<<21
#define PA5_HIGH GPIOA->BSRR = 1<<5

// NRF24L01; PC2 as Chip Enable (CE)
#define NRF24L01_CE_HIGH GPIO->BSRR = 1<<2
#define NRF24L01_CE_LOW  GPIO->BSRR = 1<<18


//
// Functions
//
void GPIOA_Setup();
void GPIOC_Setup();

void ClockFrequency_Setup();
void Systick_Setup();

void Spi3_Setup();
uint8_t Spi3_Receive_8b(uint8_t *data);
void Spi3_Send(uint8_t *byte, uint32_t length);
void Spi3_Transreceive_8b(uint8_t *dataTx, uint16_t lengthTx, uint8_t *dataRx, uint16_t lengthRx);
extern uint64_t ms;//reference time counted from the "beginning"

#endif /* LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_ */
