/*
 * LEDS.c
 *
 *  Created on: 2 abr. 2025
 *      Author: Abel
 */

#include <Ejercicio05.h>


void Encender_Led_Rojo(void){
	SetPinOn(PORT_PIN_LED_ROJO);
}

void Apagar_Led_Rojo(void){
	SetPinOff(PORT_PIN_LED_ROJO);
}

void Encender_Led_Amarillo(void){
	SetPinOn(PORT_PIN_LED_AMARILLO);
}

void Apagar_Led_Amarillo(void){
	SetPinOff(PORT_PIN_LED_AMARILLO);
}

void Encender_Led_Verde(void){
	SetPinOn(PORT_PIN_LED_VERDE);
}

void Apagar_Led_Verde(void){
	SetPinOff(PORT_PIN_LED_VERDE);
}

void Toggle_Led_Amarillo(void){
	TogglePin(PORT_PIN_LED_AMARILLO);
}
