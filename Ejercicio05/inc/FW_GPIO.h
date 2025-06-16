/*
 * FW_GPIO.h
 *
 *  Created on: 2 abr. 2025
 *      Author: Abel
 */

#ifndef FW_GPIO_H_
#define FW_GPIO_H_

extern volatile uint8_t emergencia;

void GPIO_Init(void);

void TogglePin(uint16_t pin, uint16_t port);

void SetPinOn(uint16_t port, uint16_t pin);

void SetPinOff(uint16_t port, uint16_t pin);

#endif /* FW_GPIO_H_ */
