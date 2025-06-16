/*
 * Copyright 2022 NXP
 * NXP confidential.
 * This software is owned or controlled by NXP and may only be used strictly
 * in accordance with the applicable license terms.  By expressly accepting
 * such terms or by downloading, installing, activating and/or otherwise using
 * the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to
 * be bound by the applicable license terms, then you may not retain, install,
 * activate or otherwise use the software.
 */


#include"EjercicioADC.h"

volatile uint16_t dataADC;
volatile uint16_t flagfinconver = 0;
volatile uint16_t Cantidad_Muestras = 0;

uint16_t Array_Muestras[5] = {0,0,0,0,0};

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    InitADC();

    eMaquina_de_Estado status_MDE = ESTADO_ESPERANDO_PROMEDIO;

    while(1) {

    	status_MDE = Maquina(status_MDE);

    }

    return 0 ;
}
