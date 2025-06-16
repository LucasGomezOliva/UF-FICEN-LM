/*
 * PR_D_7Seg.c
 *
 *  Created on: 4 abr. 2025
 *      Author: Abel
 */

#include"Ejercicio04.h"

void mostrarNumero(uint8_t num) {
    if (num > 9) return;
    AcualizarSegmentos(num);
}
