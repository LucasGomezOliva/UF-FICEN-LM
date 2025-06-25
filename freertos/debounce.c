#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

// Simulación de lectura de GPIO (reemplazar con la real)
uint8_t LeerGPIO(uint32_t Puerto, uint32_t Pin); // 0 = presionado, 1 = libre

typedef struct {
    uint32_t Puerto;
    uint32_t Pin;
} DataButton_t;

typedef enum {
    ESPERANDO_PULSACION,
    CONFIRMANDO_PULSACION,
    MANTENIDO,
    CONFIRMANDO_LIBERACION
} EstadoBoton_t;

void vTareaAntirebote(void *pvParameters) {
    const DataButton_t *Boton = (DataButton_t *)pvParameters;
    EstadoBoton_t estado = ESPERANDO_PULSACION;
    uint8_t entradaBoton;
    uint8_t contador = 0;
    const int LIMITE_REBOTE = 10; // Cantidad de ciclos para confirmar estado

    while (1) {
        entradaBoton = LeerGPIO(Boton->Puerto, Boton->Pin);

        switch (estado) {
            case ESPERANDO_PULSACION:
                if (entradaBoton == 0) { // Botón parece presionado
                    contador = 0;
                    estado = CONFIRMANDO_PULSACION;
                }
                break;

            case CONFIRMANDO_PULSACION:
                if (entradaBoton == 0) {
                    contador++;
                    if (contador >= LIMITE_REBOTE) {
                        // Confirmado: botón presionado
                        // Aquí puedes ejecutar una acción
                        estado = MANTENIDO;
                    }
                } else {
                    // Falsa alarma
                    estado = ESPERANDO_PULSACION;
                }
                break;

            case MANTENIDO:
                if (entradaBoton == 1) { // Botón parece liberado
                    contador = 0;
                    estado = CONFIRMANDO_LIBERACION;
                }
                break;

            case CONFIRMANDO_LIBERACION:
                if (entradaBoton == 1) {
                    contador++;
                    if (contador >= LIMITE_REBOTE) {
                        // Confirmado: botón liberado
                        // Aquí puedes ejecutar una acción al soltar
                        estado = ESPERANDO_PULSACION;
                    }
                } else {
                    // Falsa alarma
                    estado = MANTENIDO;
                }
                break;
        }

        vTaskDelay(pdMS_TO_TICKS(10)); // Pequeña espera entre ciclos
    }

    vTaskDelete(NULL);
}
