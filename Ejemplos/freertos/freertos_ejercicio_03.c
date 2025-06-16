
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdint.h"

#include "semphr.h"

/*****************************************************************************
 * types/enumerations
 ****************************************************************************/

typedef enum {
    BOTON_PRESIONADO,
    BOTON_LIBERADO
} EstadoBotonGlobal;

typedef struct DataPin {
	uint8_t Port;
	uint8_t Pin;
}DataPin;

typedef enum {
	ESPERANDO_PULSACION,
    CONFIRMANDO_PULSACION,
    MANTENIDO,
    CONFIRMANDO_LIBERACION
} EstadoBoton_t;

/*****************************************************************************
 * Variables
 ****************************************************************************/

EstadoBotonGlobal boton_status_global = BOTON_LIBERADO;

xSemaphoreHandle MutexBoardButton;

/*****************************************************************************
 * TASK
 ****************************************************************************/

/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();

    Chip_IOCON_PinMux(LPC_IOCON, 0,1, IOCON_MODE_PULLDOWN , IOCON_FUNC0);	// Configura el pin como GPIO y con pullup
    Chip_GPIO_WriteDirBit(LPC_GPIO, 0,1, true); //	Usar el pin GPIO como salida
    Chip_GPIO_SetPinState(LPC_GPIO, 0, 1, false);	// Por defecto el led esta apagado

    Chip_IOCON_PinMux(LPC_IOCON, 0,2, IOCON_MODE_PULLDOWN , IOCON_FUNC0);	// Configura el pin como GPIO y con pullup
    Chip_GPIO_WriteDirBit(LPC_GPIO, 0,2, false); //	Usar el pin GPIO como entrada

}

static void vDebounceButtonTask(void *pvParameters){

	const DataPin Button = *(DataPin*)pvParameters;
    EstadoBoton_t estado = ESPERANDO_PULSACION;
    uint8_t lecturapin;
    int contador = 0;
    const int LIMITE_REBOTE = 10;
    while (1)
    {
    	lecturapin = Chip_GPIO_GetPinState(LPC_GPIO, Button.Port, Button.Pin);
    	// lecturapin = Chip_GPIO_GetPinState(LPC_GPIO, 0, 2);
        switch (estado) {
            case ESPERANDO_PULSACION:
                if (lecturapin == 0) {
                    contador = 0;
                    estado = CONFIRMANDO_PULSACION;
                }
                break;

            case CONFIRMANDO_PULSACION:
                if (lecturapin == 0) {
                    contador++;
                    if (contador >= LIMITE_REBOTE) {
                    	//take sem
                    	 if( xSemaphoreTake( MutexBoardButton, portMAX_DELAY ) == pdTRUE ){
                         	// escribo buffer global
                         	boton_status_global = BOTON_PRESIONADO;
                         	// estado = MANTENIDO;
                         	// give sem
                         	xSemaphoreGive(MutexBoardButton);

                    	 }
                        estado = MANTENIDO;
                    }
                } else {
                    estado = ESPERANDO_PULSACION;
                }
                break;

            case MANTENIDO:
                if (lecturapin == 1) {
                    contador = 0;
                    estado = CONFIRMANDO_LIBERACION;
                }
                break;

            case CONFIRMANDO_LIBERACION:
                if (lecturapin == 1) {
                    contador++;
                    if (contador >= LIMITE_REBOTE) {

                    	//take sem
                    	 if( xSemaphoreTake( MutexBoardButton, portMAX_DELAY ) == pdTRUE ){
                         	// escribo buffer global
                         	boton_status_global = BOTON_LIBERADO;
                         	// estado = ESPERANDO_PULSACION;
                         	// give sem
                         	xSemaphoreGive(MutexBoardButton);
                    	 }

                        estado = ESPERANDO_PULSACION;
                    }
                } else {

                    estado = MANTENIDO;
                }
                break;
        }
        vTaskDelay( 1 / portTICK_RATE_MS );	// Delay de tarea, se busca ejecutar la misma cada 1ms
    }

	vTaskDelete( NULL );
}

void BlinkyLedConditionalTask(void *pvParameters){

	while(1){

		 if( xSemaphoreTake( MutexBoardButton, portMAX_DELAY ) == pdTRUE ){
			 if(boton_status_global == BOTON_PRESIONADO){
			 			Chip_GPIO_SetPinState(LPC_GPIO, 0, 1, false);	//	Apago el led
			 }
			 else{
				 Chip_GPIO_SetPinToggle(LPC_GPIO, 0, 1);	//	Toggle del led
				 // Chip_GPIO_SetPinState(LPC_GPIO, 0, 1, true);
				 }
			 xSemaphoreGive(MutexBoardButton);
		 }

		vTaskDelay( 100 / portTICK_RATE_MS );	// Delay de tarea, se busca ejecutar la misma cada 100ms
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/


int main(void)
{
	prvSetupHardware();

	//------------------------ INICIO : Resolucion de Ejercicios ------------------------

	MutexBoardButton = xSemaphoreCreateMutex();	//	Semaforo de exclusion mutua

	if(MutexBoardButton != NULL){

		//	--------Ejercicio 03 --------

		xTaskHandle vDebounceButtonTaskHandler;

		DataPin DataButton01 = {
				(uint8_t)0,
				(uint8_t)2
		};

		xTaskCreate(vDebounceButtonTask,
				(signed char *) "DebounceButton",
				configMINIMAL_STACK_SIZE,
				(void *) &DataButton01,
				(tskIDLE_PRIORITY + 2UL), 	//	Le asigno mas priridad a esta tarea
				(xTaskHandle *) &vDebounceButtonTaskHandler);

		xTaskHandle vBlinkyLedConditionalTaskHandler;

		xTaskCreate(BlinkyLedConditionalTask,
				(signed char *) "BlinkyLedConditional",
				configMINIMAL_STACK_SIZE,
				(void *) &DataButton01,
				(tskIDLE_PRIORITY + 1UL),	//	Le asigno menos prioridad a esta tarea
				(xTaskHandle *) &vBlinkyLedConditionalTaskHandler);


		//------------------------ FIN : Resolucion de Ejercicios ------------------------

		/* Start the scheduler */
		vTaskStartScheduler();

	}


	/* Should never arrive here */
	return 1;
}
