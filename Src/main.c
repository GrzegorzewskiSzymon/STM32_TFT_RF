#include <stdint.h>
#include "stm32g431xx.h"
#include "../Inc/Libraries/RegistersConfig/RegistersConfig.h"


int main(void)
{
	ClockFrequency_Setup();
	Systick_Setup();
	GPIOA_Setup();
	GPIOC_Setup();



	while(1)
	{
		PA5_HIGH;
	}
}
