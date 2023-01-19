#include <stdint.h>
#include "stm32g431xx.h"
#include "../Inc/Libraries/RegistersConfig/RegistersConfig.h"
#include "../Inc/Libraries/RF/NRF24L01.h"

uint8_t txAddress[] = {0xEE, 0xDD, 0xCC, 0xBB, 0xAA};
uint8_t txData[] = "Hi there";



int main(void)
{
	ClockFrequency_Setup();
	Systick_Setup();

	GPIOA_Setup();
	GPIOC_Setup();


	Spi_NRF24L01_Setup();
	SPI_NRF24L01_ENABLE;

	uint8_t tmpBuff[2] = {16, 255};

	Spi3_Send(tmpBuff, 2);




	NRF24L01_Setup();
	NRF24L01_Mode_Tx(txAddress, 10);


	while(1)
	{
		if(NRF24L01_Transmit(txData))
		{

		}
		else
		{
			PA5_HIGH;
		}

		uint64_t tmp_ms = ms;
		while((ms - tmp_ms) < 2000)
		{
		}

	}
}
