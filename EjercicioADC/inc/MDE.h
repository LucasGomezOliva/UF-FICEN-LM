/*
 * MDE.h
 *
 *  Created on: 25 abr. 2025
 *      Author: Abel
 */

#ifndef MDE_H_
#define MDE_H_

#define CANTIDAD_MUESTRAS_PROMEDIO 5


typedef enum
{
    ESTADO_ESPERANDO_PROMEDIO,
    ESTADO_MOSTRANDO_VALOR,
} eMaquina_de_Estado;

extern volatile uint16_t Cantidad_Muestras;

eMaquina_de_Estado Maquina(eMaquina_de_Estado STATUS);

#endif /* MDE_H_ */
