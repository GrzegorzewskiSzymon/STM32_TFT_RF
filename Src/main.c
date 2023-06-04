#include <stdint.h>
#include "stm32g431xx.h"
#include "../Inc/Libraries/RegistersConfig/RegistersConfig.h"
#include "../Inc/Libraries/RF/NRF24L01.h"
#include "../Inc/Libraries/PowerManagement/BatteryManagement.h"
#include "../Inc/Libraries/TFT_ILI9341/TFT_ILI9341.h"
#include "../Inc/Libraries/TFT_GUI/TFT_GUI.h"
#include "../Inc/Libraries/TouchScreen/XPT2046.h"

int main(void)
{
	ClockFrequency_Setup();

	GPIOA_Setup();
	GPIOB_Setup();
	GPIOC_Setup();

	Spi_NRF24L01_Setup();

	Systick_Setup();

	SPI_NRF24L01_ENABLE;

	MOSFETS_ON;

	NRF24L01_Setup();
	NRF24L01_Mode_Tx(txAddress, 10);

	Spi_ILI9341_Setup();
	Spi_XPT2046_Setup();
	SPI_ILI9341_ENABLE;
	SPI_XPT2046_ENABLE;

	GUI_SetDefaultSettings();
	ILI9341_Init();

	ADC1_Setup();
	TIM3_Setup();

	XPT2046_Setup();
	XPT2046_IRQ_Setup();

	while(1)
	{
		GUI_Run();
		XPT2046_Task();

	}
}

