/*
 * PR_ADC.C
 *
 *  Created on: 25 abr. 2025
 *      Author: Abel
 */

#include"EjercicioADC.h"
#include <math.h>

uint32_t ConvertirValor(uint32_t valor){
	uint32_t Rref = 10000;

	float VCC = 3.3;

	float V0 = ( valor * VCC ) / 4096.0;

	uint32_t Rp = (( VCC / V0 ) * Rref ) - Rref;

	return Rp;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double ConvertirValorSensortemp(float valor){

	float R1 = 10000;
	float logR2, R2, T;
	float c1 = 0.001129148, c2 = 0.000234125, c3 = 0.0000000876741;
	R2 = R1 * (4095.0 / (float)valor - 1.0); //Calcular la resistencia
	logR2 = log(R2);
	T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); // Temperatura en ºK
	T = T - 273.15;
	return  T;

}

//

void AplicacionSimple(void){

	if(Cantidad_Muestras == 0){
		IniciarConversion();
	}

	if( flagfinconver == 1 && Cantidad_Muestras <= 4){

		Array_Muestras[Cantidad_Muestras] = dataADC;

		Cantidad_Muestras++;

		flagfinconver = 0;

		if(Cantidad_Muestras < 5){
			IniciarConversion();
		}

	}
}


float RealizarPromedio(void){

	float Promedio = 0;

	for( uint16_t i = 0; i <= 4; i++){

		Promedio = Promedio + Array_Muestras[i];

	}

	return Promedio / ( Cantidad_Muestras + 1);
}

