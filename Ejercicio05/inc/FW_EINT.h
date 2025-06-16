/*
 * FW_EINT.h
 *
 *  Created on: 4 abr. 2025
 *      Author: Abel
 */

#ifndef FW_EINT_H_
#define FW_EINT_H_

#define EINT0_PORT 2
#define EINT0_PIN 10

extern volatile uint8_t emergencia;

void InitEINT0(void);

#endif /* FW_EINT_H_ */
