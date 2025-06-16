/*
 * FW_GPIO.C
 *
 *  Created on: 30 mar. 2025
 *      Author: Abel
 */


#include"Ejercicio01.h"

void GPIO_Init(void){
	// Configura el pin como GPIO y con pulldown
    Chip_IOCON_PinMux(LPC_IOCON, LED_PIN, IOCON_MODE_PULLDOWN , IOCON_FUNC0);
    //	Usar el pin GPIO como salida
    Chip_GPIO_WriteDirBit(LPC_GPIO, LED_PIN, true);
}

void TogglePin(uint16_t port, uint16_t pin){
	// Hace un toggle de un pin
	Chip_GPIO_SetPinToggle(LPC_GPIO, port, pin);
}
