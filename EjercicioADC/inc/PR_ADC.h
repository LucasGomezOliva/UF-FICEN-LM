/*
 * PR_ADC.h
 *
 *  Created on: 25 abr. 2025
 *      Author: Abel
 */

#ifndef PR_ADC_H_
#define PR_ADC_H_

extern volatile uint16_t dataADC;

extern volatile uint16_t flagfinconver;

extern volatile uint16_t Cantidad_Muestras;


extern uint16_t Array_Muestras[5];


void AplicacionSimple(void);

uint32_t ConvertirValor(uint32_t valor);

long map(long x, long in_min, long in_max, long out_min, long out_max);

double ConvertirValorSensortemp(float valor);

float RealizarPromedio(void);

#endif /* PR_ADC_H_ */
