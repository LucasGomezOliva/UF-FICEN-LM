/*
 * FW_Systick.h
 *
 *  Created on: 2 abr. 2025
 *      Author: Abel
 */

#ifndef FW_SYSTICK_H_
#define FW_SYSTICK_H_

typedef enum {
    Tick_1ms,
	Tick_10ms,
	Tick_50ms,
} SystickTickConfig;

extern volatile uint8_t NumCont;

void Systick_Init(SystickTickConfig config);

void SysTick_Handler(void);


#endif /* FW_SYSTICK_H_ */
