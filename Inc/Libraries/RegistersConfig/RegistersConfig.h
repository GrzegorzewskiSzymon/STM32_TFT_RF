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

//
// Functions
//
void GPIOA_Setup();
void ClockFrequency_Setup();
void Systick_Setup();

extern uint64_t ms;//reference time counted from the "beginning"

#endif /* LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_ */
