/*
 * FW_GPIO.h
 *
 *  Created on: 2 abr. 2025
 *      Author: Abel
 */

#ifndef FW_D_7SEG_H_
#define FW_D_7SEG_H_

#define SEG_A_PORT

#define SEG_A_PORTPIN SEG_A_PORT,0
#define SEG_B_PORTPIN SEG_A_PORT,1
#define SEG_C_PORTPIN SEG_A_PORT,2
#define SEG_D_PORTPIN SEG_A_PORT,3
#define SEG_E_PORTPIN SEG_A_PORT,4
#define SEG_F_PORTPIN SEG_A_PORT,5
#define SEG_G_PORTPIN SEG_A_PORT,6

extern const uint8_t segmentos[10][7];

void initDisplay(void);

void AcualizarSegmentos(uint8_t num);

#endif /* FW_D_7SEG_H_ */
