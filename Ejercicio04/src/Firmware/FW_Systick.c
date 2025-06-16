/*
 * FW_Systick.c
 *
 *  Created on: 2 abr. 2025
 *      Author: Abel
 */


#include <Ejercicio04.h>

void Systick_Init(SystickTickConfig config){
    switch(config){
    case Tick_1ms:
        SysTick_Config(SystemCoreClock / 1000);  // 1ms
        break;
    case Tick_10ms:
        SysTick_Config(SystemCoreClock / 100);   // 10ms
        break;
    case Tick_50ms:
        SysTick_Config(SystemCoreClock / 20);    // 50ms
        break;
    default:
        SysTick_Config(SystemCoreClock / 1000);  // Por defecto 1ms
        break;
    }
}

void SysTick_Handler(void)
{
	static uint16_t CountSegs = 0;
	if(NumCont > 9 ){
		NumCont = 0;
	}
	else{
		NumCont++;
	}
	CountSegs++;
	return;
}
