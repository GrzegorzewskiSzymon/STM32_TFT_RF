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

//TIM3
#define TIM3_START TIM3->CR1 |=  (1<<0);//Counter enabled
#define TIM3_STOP  TIM3->CR1 &= ~(1<<0);//Counter enabled

//
// SPI 1 - ILI9341
//

#define SPI_ILI9341_ENABLE  SPI1->CR1 |=  SPI_CR1_SPE //SPI  enable
#define SPI_ILI9341_DISABLE SPI1->CR1 &=~ SPI_CR1_SPE //SPI disable
//PA8 as ILI9341_RESET
#define ILI9341_RESET_LOW GPIOA->BSRR |= GPIO_BSRR_BR8;
#define ILI9341_RESET_HIG GPIOA->BSRR |= GPIO_BSRR_BS8;
//PA9 as D/C
#define ILI9341_DC_COMMAND GPIOA->BSRR |= GPIO_BSRR_BR9; //LOW  - Command
#define ILI9341_DC_DATA    GPIOA->BSRR |= GPIO_BSRR_BS9; //HIGH - Data

//
// Functions
//
void GPIOA_Setup();
void GPIOC_Setup();

void ClockFrequency_Setup();
void Systick_Setup();

void ADC1_Setup();
void ADC1_StartConversion();
void TIM3_Setup();

void Spi1_Setup();
void Spi1_Send(uint8_t *byte, uint32_t length);

void Spi3_Setup();
uint8_t Spi3_Read8();
void Spi3_Read8_Multi(uint8_t *dataRx, uint16_t cnt);
void Spi3_ClearRxBuff();
void Spi3_Send(uint8_t *byte, uint32_t length);

extern uint64_t ms;//reference time counted from the "beginning"
extern uint16_t battRawVoltageData;

#endif /* LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_ */
