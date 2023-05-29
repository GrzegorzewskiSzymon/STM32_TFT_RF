/*
 * RegistersConfig.c
 *
 *  Created on: Dec 28, 2022
 *      Author: Szymon Grzegorzewski
 */
#include <stdint.h>
#include "stm32g431xx.h"
#include "RegistersConfig.h"
#include "../PowerManagement/BatteryManagement.h"
#include "../TFT_GUI/TFT_GUI.h"
#include "../TouchScreen/XPT2046.h"

//
// GPIOx
//

void GPIOA_Setup()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	//
	//SPI 1 - ILI9341
	//

	//PA5 as SCK
	GPIOA->MODER  &=   ~(1<<10); //Alternate function mode
	GPIOA->AFR[0] |= (5<<20);//AFSEL5[0101] -> AF5
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_0;//Medium Speed

	//PA6 as MISO
	GPIOA->MODER  &=   ~(GPIO_MODER_MODER6_0);
	GPIOA->AFR[0] |= (5<<GPIO_AFRL_AFSEL6_Pos);

	//PA7 as MOSI
	GPIOA->MODER  &=   ~(GPIO_MODER_MODER7_0);
	GPIOA->AFR[0] |= (5<<GPIO_AFRL_AFSEL7_Pos);

	//PA8 as TFT_RESET
	GPIOA->MODER &= ~(GPIO_MODER_MODER8_1);//General purpose output mode

	//PA9 as D/C
	GPIOA->MODER &= ~(GPIO_MODER_MODER9_1);//General purpose output mode

	//
	// PowerManagement
	//

	// PA0 connected to battery voltage divider output; PA0 - ADC12_IN1
//	GPIOA->MODER &= ~ (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE0_1 ); // Input

	// PA15 connected to gate of mosfet
	GPIOA->MODER |=  GPIO_MODER_MODE15_Msk;
	GPIOA->MODER &= ~GPIO_MODER_MODE15_1; // GPOM


	//
	//SPI 2 - XPT2046
	//

	//PB13 as SCK

	//PA10 as MISO
	GPIOA->MODER  &=   ~(GPIO_MODER_MODE10_0);  //Alternate function mode
	GPIOA->AFR[1] |= (5<<GPIO_AFRH_AFSEL10_Pos);//AFSEL10[0101] -> AF5

	//PA11 as MOSI
	GPIOA->MODER  &=   ~(GPIO_MODER_MODE11_0);
	GPIOA->AFR[1] |= (5<<GPIO_AFRH_AFSEL11_Pos);

	//PA1 as IRQ (XPT2046)
	GPIOA->MODER  &=   ~( (GPIO_MODER_MODE1_0) | (GPIO_MODER_MODE1_1) );

}

void GPIOB_Setup()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	//
	//SPI 2 - XPT2046
	//

	//PB13 as SCK
	GPIOB->MODER  &=   ~(GPIO_MODER_MODE13_0);  //Alternate function mode
	GPIOB->AFR[1] |= (5<<GPIO_AFRH_AFSEL13_Pos);//AFSEL10[0101] -> AF5
	//PA10 as MISO
	//PA11 as MOSI
	//PA1 as IRQ (XPT2046)

}

void GPIOC_Setup()
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;


	//
	// SPI 3 - NRF24L01
	//

	// PC0 as CS
	GPIOC->MODER &= ~ GPIO_MODER_MODE0_1; // GPOM

	// PC10 as SCK
	GPIOC->MODER &= ~ GPIO_MODER_MODE10_0; // Alternate function mode
	GPIOC->AFR[1] |= (6<<GPIO_AFRH_AFSEL10_Pos); //AF6

	// PC11 as MISO
	GPIOC->MODER &= ~ GPIO_MODER_MODE11_0; // Alternate function mode
	GPIOC->AFR[1] |= (6<<GPIO_AFRH_AFSEL11_Pos); //AF6

	// PC12 as MOSI
	GPIOC->MODER &= ~ GPIO_MODER_MODE12_0; // Alternate function mode
	GPIOC->AFR[1] |= (6<<GPIO_AFRH_AFSEL12_Pos); //AF6

	// PC3 as IRQ
	GPIOC->MODER &= ~ (GPIO_MODER_MODE3_0 |GPIO_MODER_MODE3_1 ); // Input

	//PC2 as Chip Enable (CE)
	GPIOC->MODER  &= ~GPIO_MODER_MODE2_1;// Output
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
// Battery Management
//

void ADC1_RegistersCalib()
{
	ADC1->CR &= ~ADC_CR_ADCALDIF;
	ADC1->CR |=  ADC_CR_ADCAL; //Start the calibration of the ADC
	while(ADC1->CR & ADC_CR_ADCAL) //Wait for the end of the calibration
	{

	}
}

void ADC1_Setup()
{
	//Clock signal
	RCC->AHB2ENR |= RCC_AHB2ENR_ADC12EN;

	ADC12_COMMON->CCR |= (3<<ADC_CCR_CKMODE_Pos); //Adc_hclk/4

	ADC1->CR = 0; //Disable deep power_down mode
	ADC1->CR |= ADC_CR_ADVREGEN; //Voltage regulator enable

	ADC1->SMPR1 |= (4<<ADC_SMPR1_SMP1_Pos);	//47.5 ADC clock cycles

	ADC1_RegistersCalib();

	ADC1->ISR |= ADC_ISR_ADRDY; //Clear the ADRDY bit
	ADC1->CR |= ADC_CR_ADEN;   //Enable the ADC

	while (!(ADC1->ISR & ADC_ISR_ADRDY)) //Wait for ADC to be ready to start conversion
	{
	}

	ADC1->IER |= ADC_IER_EOCIE; //End of regular conversion interrupt enable
//	ADC1->CFGR |= (3<<ADC_CFGR_EXTEN_Pos); //Trigger detection on both the rising and falling edges
//	ADC1->CFGR |= (4<<ADC_CFGR_EXTSEL_Pos); //TIM3_TRGO

	NVIC_EnableIRQ(ADC1_2_IRQn);

}

void ADC1_StartConversion()
{

	ADC1->SQR1 |= (1<<ADC_SQR1_SQ1_Pos); //Channel number assigned as the 1st in the regular conversion sequence
	ADC1->CR |= ADC_CR_ADSTART; //Start of regular conversion
//	while(!((ADC1->ISR)&ADC_ISR_EOC))// Waiting for end of the conversions
//	{
//	}
//	return ADC1->DR;
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
// SPI 1 - ILI9341
//

void Spi1_Setup()
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
										//MSB first (by default)
										//Clock polarity to 0 when idle (by default)
										//The first clock transition is the first data capture edge (by default)
										//Data size 8-bit (by default)
										//Motorola frame format (by default)
										//No NSS pulse (by default)
	SPI1->CR1 |= SPI_CR1_MSTR;			//Master configuration
	SPI1->CR1 |= (4<<SPI_CR1_BR_Pos);	//fPCLK/32 = ~5,3Mhz
	SPI1->CR1 |= (1<<8) | (1<<9);  		//Software Slave Management
	SPI1->CR2 = 0;
}

void Spi1_Send(uint8_t *byte, uint32_t length)
{

    while (length > 0U)
    {
    	//not sure if necessary
    	if (((SPI1->SR)&(1<<1)))//Wait for TXE bit to set -> This will indicate that the buffer is empty
    	{
    		*((volatile uint8_t *) &SPI1->DR) = (*byte);//Load the data into the Data Register
    		byte++;
    		length--;
    	}
    }

    //not sure if necessary
	//Wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
	while (((SPI1->SR)&(1<<7))) {};
}

//
// SPI 2 -XPT2046
//

void Spi2_Setup()
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_SPI2EN;
										//MSB first (by default)
										//Clock polarity to 0 when idle (by default)
										//The first clock transition is the first data capture edge (by default)
										//Data size 8-bit (by default)
										//Motorola frame format (by default)
										//No NSS pulse (by default)
	SPI2->CR1 |= SPI_CR1_MSTR;			//Master configuration
	SPI2->CR1 |= (5<<SPI_CR1_BR_Pos);	//fPCLK/64 = ~2,65Mhz
	SPI2->CR1 |= (1<<8) | (1<<9);  		//Software Slave Management
	SPI2->CR2 = 0;
	SPI2->CR2 |= SPI_CR2_FRXTH;//RXNE event is generated if the FIFO level is greater than or equal to 1/4 (8-bit)
}

void Spi2_Send(uint8_t *byte, uint32_t length)
{

    while (length > 0U)
    {
    	//not sure if necessary
    	if (((SPI2->SR)&(1<<1)))//Wait for TXE bit to set -> This will indicate that the buffer is empty
    	{
    		*((volatile uint8_t *) &SPI2->DR) = (*byte);//Load the data into the Data Register
    		byte++;
    		length--;
    	}

    }

    //not sure if necessary
	//Wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
	while (((SPI2->SR)&(1<<7))) {};
}

uint8_t Spi2_Receive_8b(uint8_t *data)
{
	if(SPI2->SR & SPI_SR_RXNE) // if there is data
	{
		*data = SPI2->DR;

		return 1;
	}
	return 0 ;
}

void Spi2_Transreceive_8b(uint8_t *dataTx, uint16_t lengthTx, uint8_t *dataRx, uint16_t lengthRx )
{
    while (lengthTx > 0 || lengthRx > 0)
    {
    	if(lengthTx > 0)
    	{
    		if (((SPI2->SR)&(1<<1)))//Wait for TXE bit to set -> This will indicate that the buffer is empty
    		{
    			*((volatile uint8_t *) &SPI2->DR) = (*dataTx);//Load the data into the Data Register
    			dataTx++;
    			lengthTx--;
    		}
    	}
    	if(lengthRx > 0)
    	{
    		if(SPI2->SR & SPI_SR_RXNE) // if there is data
    		{
    			*dataRx = SPI2->DR;
    			dataRx++;
    			lengthRx--;
    		}
    	}
    }
}

//
// SPI 3 - NRF24L01
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

	SPI_NRF24L01_ENABLE;
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
    SPI_NRF24L01_DISABLE;
}

uint8_t Spi3_Read8()
{
	SPI_NRF24L01_ENABLE;
    while(!(SPI3->SR & SPI_SR_TXE));
    *(volatile uint8_t *)&SPI3->DR = 0;
    while (!(SPI3->SR & SPI_SR_RXNE));
    return *(volatile uint8_t *)&SPI3->DR;
    SPI_NRF24L01_DISABLE;
}

void Spi3_Read8_Multi(uint8_t *dataRx, uint16_t cnt)
{
	SPI_NRF24L01_ENABLE;
	for(uint8_t i= 0; i < cnt; i++)
	{
    while(!(SPI3->SR & SPI_SR_TXE));//Wait while buffer not empty
    *(volatile uint8_t *)&SPI3->DR = 0;
    while (!(SPI3->SR & SPI_SR_RXNE));
    dataRx[i] = *(volatile uint8_t *)&SPI3->DR;
	}
	SPI_NRF24L01_DISABLE;
}

void Spi3_ClearRxBuff()
{
	SPI_NRF24L01_ENABLE;
	while(SPI3->SR & SPI_SR_RXNE) //while receive buffer not empty
	{
		*(volatile uint8_t *)&SPI3->DR; //read data
	}
	SPI_NRF24L01_DISABLE;
}

void TIM3_Setup()
{
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM3EN; //TIM3 clock enable

	/*Set channel 1 to active level on match. tim_oc1ref signal is forced high when the
	 *	counter TIMx_CNT matches the capture/compare register 1 (TIMx_CCR1).
	 * */
	TIM3->CCMR1 |= (1<<TIM_CCMR1_OC1M_Pos);

	/*Frq = [CLK/(PSC+1)]/[ARR]
	 * 1Hz
	 * */
	TIM3->PSC = 16999;
	TIM3->CNT =  0;
	TIM3->ARR =  9999;
//	TIM3->CCR1 = 9999;

	//Master mode selection
	TIM3->CR2  |= (4<<4); //Compare - tim_oc1refc signal is used as trigger output (tim_trgo)
	TIM3->DIER |=  TIM_DIER_UIE;//Update interrupt enable
	TIM3->EGR  |= TIM_EGR_UG; 	//Update generation

	NVIC_EnableIRQ(TIM3_IRQn);    //Enable interrupt from TIM3

	TIM3_START;
}

//
// EXTI 1
//

void EXTI1_Setup()
{
	NVIC_EnableIRQ(EXTI1_IRQn);

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PA; // Select PA1 as EXTI1
	EXTI->IMR1 |= EXTI_IMR1_IM1; // Interrupt request is not masked
	EXTI->FTSR1 |= EXTI_FTSR1_FT1; // Falling edge trigger enabled
}

//
// INTERRUPT HANDLERS
//

//Battery 1HZ
void TIM3_IRQHandler()
{
	CalculateBattVolatage(); //Calculate battery level from previous sample
	TurnOffIfBatteryIsFlat();

	ADC1_StartConversion(); //Take a new sample

	GUI_DisplayBatteryStatus(batteryDisplayData.posX, batteryDisplayData.posY);
	TIM3->CNT =  0;
	TIM3->SR = 0; //Reset flag
}

void ADC1_2_IRQHandler()
{
	battery.rawVoltageData = ADC1->DR;
	ADC1->ISR |= ADC_ISR_EOC; //Reset flag
}


//SYSTICK
uint64_t ms;
void SysTick_Handler()
{
	ms++;
}

//XPT2046 PENIRQ handler
void EXTI1_IRQHandler()
{
	if(TouchState == XPT2046_IDLE)
	{
		TouchState = XPT2046_PRESAMPLING;
	}

	EXTI->PR1 |= EXTI_PR1_PIF1;
}
