/*
 * FW_EINT.c
 *
 *  Created on: 4 abr. 2025
 *      Author: Abel
 */

#include"Ejercicio05.h"
#include"FW_EINT.h"


void InitEINT0(void){

	//Configurar P2.10 como EINT0

	Chip_IOCON_PinMux(LPC_IOCON, EINT0_PORT, EINT0_PIN, IOCON_MODE_PULLUP,IOCON_FUNC1);

	LPC_SYSCTL->EXTMODE |= (1 << 0); // Activarse por flanco

	//LPC_SYSCTL->EXTPOLAR &= ~(1 << 0); // Activarse por flanco descendente

	LPC_SYSCTL->EXTPOLAR |= (1 << 0); // Activarse por flanco ascendente

	NVIC_EnableIRQ(EINT0_IRQn); // Habilitar EINT0 en el NVIC
}


void EINT0_IRQHandler(void){

	LPC_SYSCTL->EXTINT |= (1 << 0); // Limpiar flag de interrupción

	// Código de manejo de la interrupción

	emergencia = !emergencia;

}

