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
#define NRF24L01_CE_HIGH GPIOC->BSRR = 1<<2
#define NRF24L01_CE_LOW  GPIOC->BSRR = 1<<18

#define NRF24L01_CS_HIGH GPIOC->BSRR = 1<<0
#define NRF24L01_CS_LOW  GPIOC->BSRR = 1<<16

#define SPI_NRF24L01_ENABLE  SPI3->CR1 |=  SPI_CR1_SPE //SPI  enable
#define SPI_NRF24L01_DISABLE SPI3->CR1 &=~ SPI_CR1_SPE //SPI disable



//
// Functions
//
void GPIOA_Setup();
void GPIOC_Setup();

void ClockFrequency_Setup();
void Systick_Setup();

void ADC1_Setup();
uint16_t ADC1_Read();

void Spi3_Setup();
uint8_t Spi3_Read8();
void Spi3_Read8_Multi(uint8_t *dataRx, uint16_t cnt);
void Spi3_ClearRxBuff();
void Spi3_Send(uint8_t *byte, uint32_t length);
extern uint64_t ms;//reference time counted from the "beginning"

#endif /* LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_ */
