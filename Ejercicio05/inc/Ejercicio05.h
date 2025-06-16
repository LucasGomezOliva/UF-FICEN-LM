/*
 * Ejercicio04.h
 *
 *  Created on: 2 abr. 2025
 *      Author: Abel
 */

#ifndef EJERCICIO05_H_
#define EJERCICIO05_H_

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// Include de librerias

#include<stdint.h>

// Include de app

#include"FW_GPIO.h"
#include"FW_Systick.h"
#include"PR_GPIO.h"
#include "FW_EINT.h"

#include"MDE.h"

#define PORT_PIN_LED_ROJO 0,1
#define PORT_PIN_LED_VERDE 0,2
#define PORT_PIN_LED_AMARILLO 0,3

#endif /* EJERCICIO05_H_ */
