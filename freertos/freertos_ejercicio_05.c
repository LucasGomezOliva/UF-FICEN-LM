
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

typedef struct ParamsButtonTask  {
	xTaskHandle BlinkyLedTaskOnekHandler;
	xTaskHandle BlinkyLedTaskTwokHandler;
}ParamsButtonTask;

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

uint8_t cantidad_pulsaciones = 0;

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

    Chip_IOCON_PinMux(LPC_IOCON, 0,3, IOCON_MODE_PULLDOWN , IOCON_FUNC0);	// Configura el pin como GPIO y con pullup
    Chip_GPIO_WriteDirBit(LPC_GPIO, 0,3, true); //	Usar el pin GPIO como salida
    Chip_GPIO_SetPinState(LPC_GPIO, 0, 3, false);	// Por defecto el led esta apagado

    Chip_IOCON_PinMux(LPC_IOCON, 0,2, IOCON_MODE_PULLDOWN , IOCON_FUNC0);	// Configura el pin como GPIO y con pullup
    Chip_GPIO_WriteDirBit(LPC_GPIO, 0,2, false); //	Usar el pin GPIO como entrada

}



static void vBlinkyLedTaskOne(void *pvParameters){

	while(1){
		xSemaphoreTake( MutexBoardButton, portMAX_DELAY );
		Chip_GPIO_SetPinState(LPC_GPIO, 0, 1, false);
		Chip_GPIO_SetPinState(LPC_GPIO, 0, 3, true);
		xSemaphoreGive(MutexBoardButton);
		vTaskDelay( 100 / portTICK_RATE_MS );
	}
}

static void vBlinkyLedTaskTwo(void *pvParameters){

	while(1){
		xSemaphoreTake( MutexBoardButton, portMAX_DELAY );
		Chip_GPIO_SetPinState(LPC_GPIO, 0, 1, true);
		Chip_GPIO_SetPinState(LPC_GPIO, 0, 3, false);
		xSemaphoreGive(MutexBoardButton);
		vTaskDelay( 100 / portTICK_RATE_MS );
	}
}

static void vButtonTask(void *pvParameters){


	const ParamsButtonTask Data_ParamsButtonTask = *(ParamsButtonTask*)pvParameters;
	EstadoBoton_t estado = ESPERANDO_PULSACION;
	    uint8_t lecturapin;
	    int contador = 0;
	    const int LIMITE_REBOTE = 10;
	    while (1)
	    {
	    	lecturapin = Chip_GPIO_GetPinState(LPC_GPIO, 0, 2);
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

	                    	cantidad_pulsaciones++;

	                    	xSemaphoreTake( MutexBoardButton, portMAX_DELAY );

	                    	if(cantidad_pulsaciones == 1){
	                    		vTaskPrioritySet( Data_ParamsButtonTask.BlinkyLedTaskOnekHandler, tskIDLE_PRIORITY + 2 );
	                    		vTaskPrioritySet( Data_ParamsButtonTask.BlinkyLedTaskTwokHandler, tskIDLE_PRIORITY + 1 );
	                    	}

	                    	if(cantidad_pulsaciones == 2){
	                    		vTaskPrioritySet( Data_ParamsButtonTask.BlinkyLedTaskOnekHandler, tskIDLE_PRIORITY + 1 );
	                    		vTaskPrioritySet( Data_ParamsButtonTask.BlinkyLedTaskTwokHandler, tskIDLE_PRIORITY + 2 );
	                    	}

	                    	if(cantidad_pulsaciones == 3){
	                    		vTaskPrioritySet( Data_ParamsButtonTask.BlinkyLedTaskOnekHandler, tskIDLE_PRIORITY + 1 );
	                    		vTaskPrioritySet( Data_ParamsButtonTask.BlinkyLedTaskTwokHandler, tskIDLE_PRIORITY + 1 );
	                    		cantidad_pulsaciones = 0;
	                    	}

	                    	xSemaphoreGive(MutexBoardButton);

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

/*****************************************************************************
 * Public functions
 ****************************************************************************/


int main(void)
{
	prvSetupHardware();

	//------------------------ INICIO : Resolucion de Ejercicio 05 ------------------------

	MutexBoardButton = xSemaphoreCreateMutex();	//	Semaforo de exclusion mutua

	if(MutexBoardButton != NULL){

		xTaskHandle BlinkyLedTaskOnekHandler;

		xTaskCreate(vBlinkyLedTaskOne,
				(signed char *) "BlinkyLedOne",
				configMINIMAL_STACK_SIZE,
				(void *) NULL,
				(tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) &BlinkyLedTaskOnekHandler);

		xTaskHandle BlinkyLedTaskTwokHandler;

		xTaskCreate(vBlinkyLedTaskTwo,
				(signed char *) "BlinkyLedTwo",
				configMINIMAL_STACK_SIZE,
				(void *) NULL,
				(tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) &BlinkyLedTaskTwokHandler);

		xTaskHandle ButtonTaskkHandler;

		ParamsButtonTask DATA = {
				BlinkyLedTaskOnekHandler,
				BlinkyLedTaskTwokHandler
		};

		xTaskCreate(vButtonTask,
					(signed char *) "Button",
					configMINIMAL_STACK_SIZE,
					(void *) &DATA,
					(tskIDLE_PRIORITY + 3UL),
					(xTaskHandle *) &ButtonTaskkHandler);

		//------------------------ FIN : Resolucion de Ejercicio 05 ------------------------

		/* Start the scheduler */
		vTaskStartScheduler();

	}


	/* Should never arrive here */
	return 1;
}
