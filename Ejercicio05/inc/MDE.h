/*
 * MDE.h
 *
 *  Created on: 2 abr. 2025
 *      Author: Abel
 */

#ifndef MDE_H_
#define MDE_H_

#define TIEMPO_ROJO 5
#define TIEMPO_AMARILLO 1
#define TIEMPO_VERDE 4

typedef enum
{
    ESTADO_ROJO,
    ESTADO_AMARILLO,
    ESTADO_VERDE,
	ESTADO_EMERGENCIA

} eMaquina_de_Estado;

extern volatile uint8_t time_mde;

extern volatile eMaquina_de_Estado estadoauxiliar;

eMaquina_de_Estado Maquina(eMaquina_de_Estado STATUS , eMaquina_de_Estado EstAnt);

#endif /* MDE_H_ */
