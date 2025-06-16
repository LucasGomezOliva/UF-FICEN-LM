/*
 * FW_ADC.h
 *
 *  Created on: 4 abr. 2025
 *      Author: Abel
 */

#ifndef FW_ADC_H_
#define FW_ADC_H_

#define ADC_CH0_PORT 0
#define ADC_CH0_PIN 23


extern volatile uint16_t dataADC;

extern volatile uint16_t flagfinconver;

void InitADC(void);
void IniciarConversion(void);

#endif /* FW_ADC_H_ */
