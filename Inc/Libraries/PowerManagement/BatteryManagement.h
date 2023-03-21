/*
 * BatteryManagement.h
 *
 *  Created on: 17 Feb 2023
 *      Author: Szymon Grzegorzewski
 */

#ifndef LIBRARIES_POWERMANAGEMENT_BATTERYMANAGEMENT_H_
#define LIBRARIES_POWERMANAGEMENT_BATTERYMANAGEMENT_H_


// PA15
#define MOSFETS_OFF  GPIOA->BSRR = 1<<31
#define MOSFETS_ON   GPIOA->BSRR = 1<<15

#define MAX_BATT_VALUE 2340  //4.2V

typedef struct
{
	uint16_t rawVoltageData;  //Raw data collected from ADC

	uint8_t voltagePercentage; //0%-100%
	uint8_t voltageInteger;    //0-4
	uint16_t voltageFract;     //0-999

}BATTERY_STATS;

extern BATTERY_STATS battery;

void CalculateBattVolatage();
void TurnOffIfBatteryIsFlat();

#endif /* LIBRARIES_POWERMANAGEMENT_BATTERYMANAGEMENT_H_ */
