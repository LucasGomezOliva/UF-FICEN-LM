/*
 * MDE.c
 *
 *  Created on: 2 abr. 2025
 *      Author: Abel
 */

#include "EjercicioADC.h"


eMaquina_de_Estado Maquina(eMaquina_de_Estado STATUS){

    switch (STATUS){

    case ESTADO_ESPERANDO_PROMEDIO:

    	AplicacionSimple();

    	if( Cantidad_Muestras >= CANTIDAD_MUESTRAS_PROMEDIO){
    		STATUS = ESTADO_MOSTRANDO_VALOR;
    	}

        break;

    case ESTADO_MOSTRANDO_VALOR:

    	float resultado_promedio = RealizarPromedio();

    	// float res = ConvertirValor(resultado_promedio);

    	double res = ConvertirValorSensortemp(resultado_promedio);

		Cantidad_Muestras = 0;

		STATUS = ESTADO_ESPERANDO_PROMEDIO;

    	break;

    default:
        STATUS = ESTADO_ESPERANDO_PROMEDIO;
        break;
    }

    return STATUS;
}
