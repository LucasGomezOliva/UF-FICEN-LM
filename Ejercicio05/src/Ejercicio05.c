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

#include <Ejercicio05.h>

volatile uint8_t time_mde = 0;

volatile uint8_t emergencia = 0;

volatile eMaquina_de_Estado estadoauxiliar = ESTADO_ROJO;

volatile eMaquina_de_Estado estadoemergenciaaux;

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

    //	Variables para la Maquina de Estado

    eMaquina_de_Estado status_MDE = ESTADO_ROJO;
    eMaquina_de_Estado EstAnt = ESTADO_ROJO;

    //	Inicializacion de los GPIO

    GPIO_Init();

    //	Inicializacion del Systick

    Systick_Init(Tick_1ms);

    // Inicializacion de interrupcion externa

    InitEINT0();

    emergencia = 0;

    while(1) {


    	if( emergencia == 1){
    		status_MDE = ESTADO_EMERGENCIA;
    		estadoauxiliar = ESTADO_EMERGENCIA;
    	}
    	if(emergencia == 0 && status_MDE == ESTADO_EMERGENCIA){
    		status_MDE = ESTADO_ROJO;
    		estadoauxiliar = ESTADO_ROJO;
    	}

    	status_MDE = Maquina(status_MDE , EstAnt);

    }
    return 0 ;
}
