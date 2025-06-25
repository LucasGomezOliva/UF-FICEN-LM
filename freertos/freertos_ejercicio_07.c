
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdint.h"

#include"queue.h"
#include "semphr.h"

/*****************************************************************************
 * types/enumerations
 ****************************************************************************/

typedef enum {
    BOTON_PRESIONADO,
    BOTON_LIBERADO
} EstadoBotonGlobal;

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

xQueueHandle xQueueLed;

uint32_t cantidad_tiempo = 0;

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

static void vLedTask(void *pvParameters){

	uint32_t recibo_tiempo = 0;
	uint32_t tiempo_por_defecto = 200;

	const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

	while(1){

		if (xQueueReceive(xQueueLed, &recibo_tiempo, xTicksToWait) == pdPASS) {
			tiempo_por_defecto = recibo_tiempo;
		}

		Chip_GPIO_SetPinToggle(LPC_GPIO, 0, 1);

		vTaskDelay( tiempo_por_defecto / portTICK_RATE_MS );
	}
}

static void vButtonTask(void *pvParameters){

		// const ParamsButtonTask Data_ParamsButtonTask = *(ParamsButtonTask*)pvParameters;

		EstadoBoton_t estado = ESPERANDO_PULSACION;
	    uint8_t lecturapin;
	    int contador = 0;
	    const int LIMITE_REBOTE = 10;
	    while (1)
	    {
	    	lecturapin = Chip_GPIO_GetPinState(LPC_GPIO, 0, 2);
	        switch (estado) {
	            case ESPERANDO_PULSACION:
	            	cantidad_tiempo = 0;
	                if (lecturapin == 0) {
	                    contador = 0;
	                    estado = CONFIRMANDO_PULSACION;
	                }
	                break;

	            case CONFIRMANDO_PULSACION:
	                if (lecturapin == 0) {
	                    contador++;
	                    if (contador >= LIMITE_REBOTE) {

	                        estado = MANTENIDO;
	                    }
	                } else {
	                    estado = ESPERANDO_PULSACION;
	                }
	                break;

	            case MANTENIDO:

	            	cantidad_tiempo++;

	                if (lecturapin == 1) {

	                	xQueueSendToBack(xQueueLed,&cantidad_tiempo,0);
	                	// taskYIELD();

	                    contador = 0;
	                    estado = CONFIRMANDO_LIBERACION;
	                }
	                break;

	            case CONFIRMANDO_LIBERACION:
	                if (lecturapin == 1) {
	                    contador++;
	                    if (contador >= LIMITE_REBOTE) {

	                         	boton_status_global = BOTON_LIBERADO;

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

/*****************************************************************************
 * Public functions
 ****************************************************************************/


int main(void)
{
	prvSetupHardware();

	//------------------------ INICIO : Resolucion de Ejercicio 07 ------------------------

	MutexBoardButton = xSemaphoreCreateMutex();	//	Semaforo de exclusion mutua

	xQueueLed = xQueueCreate(5,sizeof(uint32_t)); // Creo la queue para variables de 32


	if(MutexBoardButton != NULL && xQueueLed != NULL ){

		xTaskHandle LedTaskHandler;

		xTaskCreate(vLedTask,
				(signed char *) "LedTask",
				configMINIMAL_STACK_SIZE,
				(void *) NULL,
				(tskIDLE_PRIORITY + 2UL),
				(xTaskHandle *) &LedTaskHandler);

		xTaskHandle ButtonTaskkHandler;

		xTaskCreate(vButtonTask,
					(signed char *) "Button",
					configMINIMAL_STACK_SIZE,
					(void *) NULL,
					(tskIDLE_PRIORITY + 1UL),
					(xTaskHandle *) &ButtonTaskkHandler);

		//------------------------ FIN : Resolucion de Ejercicio 07 ------------------------

		/* Start the scheduler */
		vTaskStartScheduler();

	}


	/* Should never arrive here */
	return 1;
}
