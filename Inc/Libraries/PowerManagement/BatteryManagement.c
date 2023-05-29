/*
 * BatteryManagement.c
 *
 *  Created on: 17 Feb 2023
 *      Author: Szymon Grzegorzewski
 */

#include <stdint.h>
#include "stm32g431xx.h"
#include "BatteryManagement.h"

BATTERY_STATS battery;

long Map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void CalculateBattVolatage()
{
	long data;

	data = Map(battery.rawVoltageData, 0, MAX_BATT_VALUE, 0, 4200);

	battery.voltageInteger = data/1000;
	battery.voltageFract = data%1000;

	battery.voltagePercentage = Map(battery.rawVoltageData, MAX_BATT_VALUE*0.88, MAX_BATT_VALUE, 0, 100);//MAX_BATT_VALUE*0.88 = 3,7V
}

void TurnOffIfBatteryIsFlat()
{
	if(battery.voltageInteger > 2) //Temporary fix to do not consider first sample (0v)
		if(battery.voltageInteger < 4 && battery.voltageFract <= 700)
			MOSFETS_OFF;
}
