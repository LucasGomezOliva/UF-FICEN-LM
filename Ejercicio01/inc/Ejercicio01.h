/*
 * Ejercicio01.h
 *
 *  Created on: 30 mar. 2025
 *      Author: Abel
 */

#ifndef EJERCICIO01_H_
#define EJERCICIO01_H_

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif
#include <cr_section_macros.h>

// Define de pines del LPC1769

#define LED_PIN 0,1

// Include de librerias

#include"FW_GPIO.h"
#include"FW_Systick.h"
#include"PR_Aplicacion.h"

// Variables globales

extern int ToggleLedFlag;

#endif /* EJERCICIO01_H_ */
