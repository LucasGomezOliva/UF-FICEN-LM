/*
 * MDE.c
 *
 *  Created on: 2 abr. 2025
 *      Author: Abel
 */

#include <Ejercicio05.h>


eMaquina_de_Estado Maquina(eMaquina_de_Estado STATUS , eMaquina_de_Estado EstAnt){

    switch (STATUS){

    case ESTADO_ROJO:

    	Encender_Led_Rojo();
    	Apagar_Led_Amarillo();
    	Apagar_Led_Verde();

    	if( time_mde >= TIEMPO_ROJO ){
    		STATUS = ESTADO_AMARILLO;
    		estadoauxiliar = ESTADO_ROJO;
    		time_mde = 0;
    	}

        break;

    case ESTADO_AMARILLO:

    	Apagar_Led_Rojo();
    	Encender_Led_Amarillo();
    	Apagar_Led_Verde();

    	if( time_mde >= TIEMPO_AMARILLO && estadoauxiliar == ESTADO_ROJO){
    	    STATUS = ESTADO_VERDE;
    	    estadoauxiliar = ESTADO_AMARILLO;
    	    time_mde = 0;
    	 }

    	if( time_mde >= TIEMPO_AMARILLO && estadoauxiliar == ESTADO_VERDE){
    		STATUS = ESTADO_ROJO;
    		estadoauxiliar = ESTADO_AMARILLO;
    		time_mde = 0;
    	  }

        break;

    case ESTADO_VERDE:

    	Apagar_Led_Rojo();
    	Apagar_Led_Amarillo();
    	Encender_Led_Verde();

    	if( time_mde >= TIEMPO_VERDE ){
    		STATUS = ESTADO_AMARILLO;
    		estadoauxiliar = ESTADO_VERDE;
    	    time_mde = 0;
    	}

        break;

    case ESTADO_EMERGENCIA:

    	Apagar_Led_Rojo();
    	Apagar_Led_Verde();

    	if( time_mde >= TIEMPO_AMARILLO ){
    		Toggle_Led_Amarillo();
    		time_mde = 0;
    	}

    	break;

    default:
        STATUS = ESTADO_ROJO;
        estadoauxiliar = ESTADO_ROJO;
        break;
    }

    return STATUS;
}
