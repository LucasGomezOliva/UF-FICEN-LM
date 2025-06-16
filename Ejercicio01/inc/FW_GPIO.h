/*
 * FW_GPIO.h
 *
 *  Created on: 30 mar. 2025
 *      Author: Abel
 */

#ifndef FW_GPIO_H_
#define FW_GPIO_H_

void GPIO_Init(void);

void TogglePin(uint16_t pin, uint16_t port);

#endif /* FW_GPIO_H_ */
