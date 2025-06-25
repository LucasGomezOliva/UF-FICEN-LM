
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdint.h"

#include"queue.h"
#include "semphr.h"

/*****************************************************************************
 * types/enumerations
 ****************************************************************************/

#define ADC_CH0_PORT 0
#define ADC_CH0_PIN 23

/*****************************************************************************
 * Variables
 ****************************************************************************/

xSemaphoreHandle MutexBoardButton;

xQueueHandle xQueueADC;

xQueueHandle xQueueTEMP;

static ADC_CLOCK_SETUP_T ADCSetup;

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

	//////////////	Inicializacion del ADC

	Chip_IOCON_PinMux(LPC_IOCON, ADC_CH0_PORT, ADC_CH0_PIN, IOCON_MODE_PULLUP, IOCON_FUNC1);

	Chip_ADC_Init(LPC_ADC, &ADCSetup); // Inicializar el ADC

	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH0, ENABLE); // Habilitar el canal ADC 0

	// Configurar la tasa de muestreo

	// Chip_ADC_SetSampleRate(LPC_ADC, &ADCSetup , ADC_MAX_SAMPLE_RATE);

	//Habilitar interrupciones del ADC para el canal 0

	// Chip_ADC_Int_SetChannelCmd(LPC_ADC, ADC_CH0, ENABLE);

	// NVIC_EnableIRQ(ADC_IRQn);

	// NVIC_ClearPendingIRQ(ADC_IRQn);

	//////////////	Inicializacion del ADC
}

static void vADCTask(void *pvParameters){

	uint16_t dataADC;
	while(1){
		Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);	//	Inicia una lectura

		if(SUCCESS == Chip_ADC_ReadValue(LPC_ADC, ADC_CH0, &dataADC)){
			xQueueSendToBack(xQueueADC,&dataADC,portMAX_DELAY);	// envio dataADC a xQueueADC
		}

		vTaskDelay( 5 / portTICK_RATE_MS );
	}
}

static void vProcesarADCTask(void *pvParameters){

	uint16_t cant_muestras = 0;
	uint16_t acumulador = 0;
	uint16_t recibo_dato = 0;

	while(1){

		if (xQueueReceive(xQueueADC, &recibo_dato, portMAX_DELAY) == pdPASS) {
			cant_muestras++;
			acumulador = acumulador +  recibo_dato;
			if(cant_muestras == 10){
				acumulador = acumulador / 10;
				xQueueSendToBack(xQueueTEMP,&acumulador,portMAX_DELAY);
				acumulador = 0;
			}
		}
	}
}

static void vLedTask(void *pvParameters){

	uint16_t dato = 0;

	const portTickType xTicksToWait = 100 / portTICK_RATE_MS;

	while(1){

		if (xQueueReceive(xQueueTEMP, &dato, portMAX_DELAY) == pdPASS) {

			if(dato > 512){
				Chip_GPIO_SetPinState(LPC_GPIO, 0, 1, true);
			}
			else{
				Chip_GPIO_SetPinState(LPC_GPIO, 0, 1, false);
			}
		}
		vTaskDelay( 50 / portTICK_RATE_MS );
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/


int main(void)
{
	prvSetupHardware();

	//------------------------ INICIO : Resolucion de Ejercicio 08 ------------------------

	xQueueADC = xQueueCreate(10,sizeof(uint16_t));

	xQueueTEMP = xQueueCreate(10,sizeof(uint32_t));

	if(xQueueTEMP != NULL && xQueueADC != NULL ){

		xTaskHandle ADCTaskHandler;

		xTaskCreate(vADCTask,
				(signed char *) "ADCTask",
				configMINIMAL_STACK_SIZE,
				(void *) NULL,
				(tskIDLE_PRIORITY + 3UL),
				(xTaskHandle *) &ADCTaskHandler);

		xTaskHandle ProcesarADCTaskHandler;

		xTaskCreate(vProcesarADCTask,
				(signed char *) "ProcesarADCTask",
				configMINIMAL_STACK_SIZE,
				(void *) NULL,
				(tskIDLE_PRIORITY + 2UL),
				(xTaskHandle *) &ProcesarADCTaskHandler);

		xTaskHandle LedTaskHandler;

		xTaskCreate(vLedTask,
				(signed char *) "LedTask",
				configMINIMAL_STACK_SIZE,
				(void *) NULL,
				(tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) &LedTaskHandler);


		//------------------------ FIN : Resolucion de Ejercicio 08 ------------------------

		/* Start the scheduler */
		vTaskStartScheduler();

	}


	/* Should never arrive here */
	return 1;
}