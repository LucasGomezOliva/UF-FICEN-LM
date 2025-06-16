/*
 * FW_GPIO.c
 *
 *  Created on: 2 abr. 2025
 *      Author: Abel
 */

#include <Ejercicio05.h>


void GPIO_Init(void){

	// Inicializacion de los pines de salida para los distintos LEDS

    Chip_IOCON_PinMux(LPC_IOCON, PORT_PIN_LED_ROJO, IOCON_MODE_PULLDOWN , IOCON_FUNC0);
    Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_PIN_LED_ROJO, true);

    Chip_IOCON_PinMux(LPC_IOCON, PORT_PIN_LED_AMARILLO, IOCON_MODE_PULLDOWN , IOCON_FUNC0);
    Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_PIN_LED_AMARILLO, true);

    Chip_IOCON_PinMux(LPC_IOCON, PORT_PIN_LED_VERDE, IOCON_MODE_PULLDOWN , IOCON_FUNC0);
    Chip_GPIO_WriteDirBit(LPC_GPIO, PORT_PIN_LED_VERDE, true);

}


void TogglePin(uint16_t port, uint16_t pin){
	Chip_GPIO_SetPinToggle(LPC_GPIO, port, pin);
}

void SetPinOn(uint16_t port, uint16_t pin){
	Chip_GPIO_SetPinOutHigh(LPC_GPIO, port, pin);
}

void SetPinOff(uint16_t port, uint16_t pin){
	Chip_GPIO_SetPinOutLow(LPC_GPIO, port, pin);
}
