/*
 * RegistersConfig.c
 *
 *  Created on: Dec 28, 2022
 *      Author: Szymon Grzegorzewski
 */

#include <stdint.h>
#include "stm32g431xx.h"
#include "RegistersConfig.h"

//
// GPIOx
//

void GPIOA_Setup()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// PA5
	GPIOA->MODER  &= ~(1<<11);// Output
}

//
// SysClk
//

void ClockFrequency_Setup()
{

	RCC->CR |= (1<<8); //HSION

	while(!(RCC->CR & (1<<10))); //HSIRDY

	RCC->CR = 0x00000063;//reset

	//Enable HSE and wait till its ready
	RCC->CR |= (1<<16); //HSEON
	while (!(RCC->CR & (1<<17)));//HSERDY


	//Select the Range 1 boost mode by clearing the R1MODE bit is in the PWR_CR5 register
	PWR->CR5 &= ~(1<<8);

	//Voltage scaling range selection
	PWR->CR1 |= (1<<9);//VOS[0;1] -> Range 1


	FLASH->ACR |= (1<<8);//Enable prefetch to mitigate the disparity in speed at higher clock rates
	FLASH->ACR |= (5<<0);//Period to the Flash access time


	                        //PLLR = 2 by default
	RCC->PLLCFGR |= (85<<8);//PLLN = 85
	RCC->PLLCFGR |= (5<<4); //PLLM = 6
	RCC->PLLCFGR |= (3<<0); //PLL SRC
	RCC->PLLCFGR |= (1<<24);//Main PLL clock selected

	//Enable the PLL again by setting PLLON
	RCC->CR |= (1<<24);

	//Wait until PLLRDY is set
	while(!(RCC->CR & 1<<25))
	{
	}

	//The system clock must be divided by 2 using the AHB prescaler before switching
	// to a higher system frequency
	RCC->CFGR |= (1<<7);

	// Configure and switch to PLL for a new system frequency
	RCC->CFGR |= (3<<0);

	//Wait for at least 1us and then reconfigure the AHB prescaler to get the needed
	// HCLK clock frequency.
	while(!(RCC->CFGR & (2<<2)) && !(RCC->CFGR & (1<<2)))
	{
	}

	RCC->CFGR &= ~(1<<7);
}


//
// Systick
//

void Systick_Setup()
{
	NVIC_EnableIRQ(SysTick_IRQn); //Enable interrupt from Systick

	SysTick->LOAD = (uint32_t)170000;              //The value which will be decrementing, 24bit value
	SysTick->VAL = 0;                              //(undefined on reset)
 	SysTick->CTRL  =  (SysTick_CTRL_CLKSOURCE_Msk) //Processor clock (AHB)
 				   |  (SysTick_CTRL_ENABLE_Msk)    //Enables the counter
 				   |  (SysTick_CTRL_TICKINT_Msk);  //Exception request
}


//
// SPI 2
//

void Spi3_Setup()
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI3EN;
										//MSB first (by default)
										//Clock polarity to 0 when idle (by default)
										//The first clock transition is the first data capture edge (by default)
										//Data size 8-bit (by default)
										//Motorola frame format (by default)
										//No NSS pulse (by default)
	SPI3->CR1 |= SPI_CR1_MSTR;			//Master configuration
	SPI3->CR1 |= (4<<SPI_CR1_BR_Pos);	//fPCLK/32 = 5,3125Mhz
	SPI3->CR1 |= (1<<8) | (1<<9);  		//Software Slave Management
	SPI3->CR2 = 0;
	SPI3->CR2 |= SPI_CR2_FRXTH;//RXNE event is generated if the FIFO level is greater than or equal to 1/4 (8-bit)
}

void Spi3_Send(uint8_t *byte, uint32_t length)
{

    while (length > 0U)
    {
    	//not sure if necessary
    	if (((SPI3->SR)&(1<<1)))//Wait for TXE bit to set -> This will indicate that the buffer is empty
    	{
    		*((volatile uint8_t *) &SPI3->DR) = (*byte);//Load the data into the Data Register
    		byte++;
    		length--;
    	}

    }

    //not sure if necessary
	//Wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
	while (((SPI3->SR)&(1<<7))) {};
}

uint8_t SPI3_Receive_8b(uint8_t *data)
{
	if(SPI3->SR & SPI_SR_RXNE) // if there is data
	{
		*data = SPI3->DR;

		return 1;
	}
	return 0 ;
}

void SPI3_Transreceive_8b(uint8_t *dataTx, uint16_t lengthTx, uint8_t *dataRx, uint16_t lengthRx )
{
    while (lengthTx > 0 || lengthRx > 0)
    {
    	if(lengthTx > 0)
    	{
    		if (((SPI3->SR)&(1<<1)))//Wait for TXE bit to set -> This will indicate that the buffer is empty
    		{
    			*((volatile uint8_t *) &SPI3->DR) = (*dataTx);//Load the data into the Data Register
    			dataTx++;
    			lengthTx--;
    		}
    	}
    	if(lengthRx > 0)
    	{
    		if(SPI3->SR & SPI_SR_RXNE) // if there is data
    		{
    			*dataRx = SPI3->DR;
    			dataRx++;
    			lengthRx--;
    		}
    	}
    }
}


//
// IRQ Handlers
//

//SYSTICK
uint64_t ms;
void SysTick_Handler()
{
	ms++;
}
