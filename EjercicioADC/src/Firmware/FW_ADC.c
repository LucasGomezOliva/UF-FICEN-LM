/*
 * FW_ADC.c
 *
 *  Created on: 4 abr. 2025
 *      Author: Abel
 */

#include"EjercicioADC.h"

static ADC_CLOCK_SETUP_T ADCSetup;

void InitADC(void){

	//	TODO: Configurar bien el PINMUX

	Chip_IOCON_PinMux(LPC_IOCON, ADC_CH0_PORT, ADC_CH0_PIN, IOCON_MODE_PULLUP, IOCON_FUNC1);

	Chip_ADC_Init(LPC_ADC, &ADCSetup); // Inicializar el ADC

	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH0, ENABLE); // Habilitar el canal ADC 0

	// Configurar la tasa de muestreo

	Chip_ADC_SetSampleRate(LPC_ADC, &ADCSetup , ADC_MAX_SAMPLE_RATE);

	//Habilitar interrupciones del ADC para el canal 0

	Chip_ADC_Int_SetChannelCmd(LPC_ADC, ADC_CH0, ENABLE);

	NVIC_EnableIRQ(ADC_IRQn);

	NVIC_ClearPendingIRQ(ADC_IRQn);

	// Iniciar una nueva conversión del ADC

	// Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);

	return;
}

void ADC_IRQHandler(void){
	Chip_ADC_ReadValue(LPC_ADC, ADC_CH0, &dataADC);
	flagfinconver = 1;
}

void IniciarConversion(void){
	Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
}

