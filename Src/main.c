#include <stdint.h>
#include "stm32g431xx.h"
#include "../Inc/Libraries/RegistersConfig/RegistersConfig.h"
#include "../Inc/Libraries/RF/NRF24L01.h"
#include "../Inc/Libraries/PowerManagement/BatteryManagement.h"

uint8_t rxAddress[] = {0xEE, 0xDD, 0xCC, 0xBB, 0xAA};
uint8_t rxBuff[32];

uint16_t posY, posX;
int main(void)
{
	ClockFrequency_Setup();

	GPIOA_Setup();
	GPIOC_Setup();

	Spi_NRF24L01_Setup();

	Systick_Setup();

	SPI_NRF24L01_ENABLE;

	MOSFETS_ON;

	NRF24L01_Setup();
	NRF24L01_Mode_Rx(rxAddress, 10);
	ADC1_Setup();
	TIM3_Setup();
	while(1)
	{

		if (NRF24L01_isDataAvailable(1))
		{
			NRF24L01_Receive(rxBuff);
			for (uint8_t i = 0; i < 32; i++)
			{
				rxBuff[i] = 0;
			}

		}
	}
}

