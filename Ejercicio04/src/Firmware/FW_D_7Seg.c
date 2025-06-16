/*
 * FW_GPIO.c
 *
 *  Created on: 2 abr. 2025
 *      Author: Abel
 */

#include <Ejercicio04.h>

void initDisplay(void) {
    Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 0, IOCON_FUNC0);
    Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 1, IOCON_FUNC0);
    Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 2, IOCON_FUNC0);
    Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 3, IOCON_FUNC0);
    Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 4, IOCON_FUNC0);
    Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 5, IOCON_FUNC0);
    Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 6, IOCON_FUNC0);

    for (int i = 0; i <= 6; i++) {
        Chip_GPIO_WriteDirBit(LPC_GPIO, 1, i, true);
    }
}

void AcualizarSegmentos(uint8_t num) {
    for (int i = 0; i < 7; i++) {
        Chip_GPIO_SetPinState(LPC_GPIO, 1, i, segmentos[num][i]);
    }
}
