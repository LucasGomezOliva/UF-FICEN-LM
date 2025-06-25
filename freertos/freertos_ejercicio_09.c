
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdint.h"

#include"queue.h"
#include "semphr.h"

/*****************************************************************************
 * types/enumerations
 ****************************************************************************/
/
//	Enum para la maquina de estado del antirrebote
 typedef enum {
	ESPERANDO_PULSACION,
    CONFIRMANDO_PULSACION,
    MANTENIDO,
    CONFIRMANDO_LIBERACION
} EstadoBoton_t;

//	Enum para la variable global de estado del boton
typedef enum {
    BOTON_PRESIONADO,
    BOTON_LIBERADO
} EstadoBotonGlobal;

 /*****************************************************************************
 * DATA
 ****************************************************************************/

#define SINE_SAMPLES 32

#define TONES 7

const uint16_t sine_table[SINE_SAMPLES] = {
    511, 611, 707, 794, 867, 925, 965, 986,
    990, 976, 938, 885, 819, 739, 650, 555,
    459, 366, 280, 207, 152, 117, 103, 111,
    139, 185, 247, 322, 408, 500, 594, 685
};

const uint16_t note_frequencies[TONES] = {
    262, // Do
    294, // Re
    330, // Mi
    349, // Fa
    392, // Sol
    440, // La
    494 // Si
};


/*****************************************************************************
 * Variables
 ****************************************************************************/

//	Variable global del boton
EstadoBotonGlobal boton_status_global = BOTON_LIBERADO;

//	Semaforo mutex
xSemaphoreHandle MutexBoardButton;

/*****************************************************************************
 * TASK
 ****************************************************************************/

/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();

}

void vToneControlTask(void *pvParameters){

	while(1){


		vTaskDelay( 100 / portTICK_RATE_MS );	// Delay de tarea, se busca ejecutar la misma cada 100ms
	}
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

/*****************************************************************************
 * Public functions
 ****************************************************************************/

 /*****************************************************************************
 * Interrupciones
 ****************************************************************************/
void InitDAC(void){
	// Configurar el pin P0.26 para la función DAC
	Chip_IOCON_PinMuxSet(LPC_IOCON,DAC_OUT_PIN, IOCON_FUNC2);
	// Inicializar el DAC
	Chip_DAC_Init(LPC_DAC);
	// Configurar el DAC para la velocidad de actualización máxima (por ejemplo, 1MHz)
	Chip_DAC_SetBias(LPC_DAC, DAC_MAX_UPDATE_RATE_1MHz);
	// Actualiza el valor de salida del DAC
	Chip_DAC_UpdateValue(LPC_DAC, 0);
}

void InitTimer0Match(void){
	Chip_TIMER_Init(LPC_TIMER0); // Inicializa Timer0
 	Chip_TIMER_Reset(LPC_TIMER0); // Reseteo valores de timer por defecto
 	Chip_TIMER_PrescaleSet(LPC_TIMER0, 96); // Configura prescaler a 96 para contar en 1 us
 	Chip_TIMER_SetMatch(LPC_TIMER0, 0, 1000000); // Configura Match0 para 1s (1,000,000 us))
 	Chip_TIMER_MatchEnableInt(LPC_TIMER0, 0); // Habilita interrupción en Match0
 	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER0, 0); // Reinicia Timer en Match0
 	Chip_TIMER_Enable(LPC_TIMER0); // Habilita Timer0
 	NVIC_ClearPendingIRQ(TIMER0_IRQn); // Limpia las interrupciones pendientes
 	NVIC_EnableIRQ(TIMER0_IRQn); // Habilita las interrupciones del Timer
}


 void TIMER0_IRQHandler(void){
 	if (Chip_TIMER_MatchPending(LPC_TIMER0, 0)){
 		Chip_TIMER_ClearMatch(LPC_TIMER0, 0); // Limpia flagde interrupción Match0
 	}
}


int main(void)
{
	prvSetupHardware();

	//------------------------ INICIO : Resolucion de Ejercicio 07 ------------------------

	MutexBoardButton = xSemaphoreCreateMutex();	//	Semaforo de exclusion mutua

	if(MutexBoardButton != NULL ){

		xTaskHandle ToneControlTaskHandler;

		xTaskCreate(vToneControlTask,
				(signed char *) "ToneControlTask",
				configMINIMAL_STACK_SIZE,
				(void *) NULL,
				(tskIDLE_PRIORITY + 2UL),
				(xTaskHandle *) &ToneControlTaskHandler);
        
        xTaskHandle vDebounceButtonTaskHandler;

		xTaskCreate(vDebounceButtonTask,
				(signed char *) "DebounceButtonTask",
				configMINIMAL_STACK_SIZE,
				(void *) NULL,
				(tskIDLE_PRIORITY + 2UL),
				(xTaskHandle *) &vDebounceButtonTaskHandler);


		//------------------------ FIN : Resolucion de Ejercicio 07 ------------------------

		/* Start the scheduler */
		vTaskStartScheduler();

	}


	/* Should never arrive here */
	return 1;
}
