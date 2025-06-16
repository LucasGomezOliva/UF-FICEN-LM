/*	PROTOTIPO DE TAREA	*/
void MyTask(void *p)
{
	/*	INICIALIZACIONES DE TAREA	*/

	while(1)
	{
		/*	CODIGO A EJECUTAR	*/
		vTaskDelay(1/portTICK_RATE_MS);		// Delay de tarea, se busca ejecutar la misma cada 1ms
	}
}

//  ----------------------------------------------------------------------------------------------------------------------------
/*	HANDLERS PARA FREERTOS		*/
xTaskHandle MyTaskHandler;				                // Variable de tipo Tarea Handler para manejo de mi tarea
xSemaphoreHandle MutexBoardButton;			            // Variabe de tipo Sem Handler para manejo de semaforo de exclusion mutua
xSemaphoreHandle BinarySemaphore;			            // Variabe de tipo Sem Handler para manejo de semaforo binario
xQueueHandle DataQueueHandle;				            // Variable de tipo Queue Handler para manejo de cola de datos

//  ----------------------------------------------------------------------------------------------------------------------------
/*	FUNCIONES BASICAS DE FREERTOS		*/
xTaskCreate( 	MyTask,					                // Puntero a la funcion/tarea
		(signed char *)"MyTaskName",		            // Nombre dado a la tarea
		configMINIMAL_STACK_SIZE,		                // Stack de la tarea
		(void *) NULL,				                    // Puntero a los parametros de entrada de la tarea
		(tskIDLE_PRIORITY + 1UL),		                // Prioridad de la tarea tskIDLE_PRIORITY + 1
		(xTaskHandle *) &MyTaskHandler);	            // Puntero a la tarea

MutexBoardButton = xSemaphoreCreateMutex();		        // Crear Semaforo Mutex y asignarlo a un handler
BinarySemaphore = vSemaphoreCreateBinary();		        // Crear Semaforo Binario y asignarlo a un handler
DataQueueHandle = xQueueCreate(100,sizeof(uint32_t));	// Crear una cola de datos de 100 elementos de tipo uint32_t

xSemaphoreTake(MutexBoardButton,portMAX_DELAY);		    // Intenta tomar el semaforo hasta que pase portMAX_DELAY la tarea seguira bloqueada
xSemaphoreGive(MutexBoardButton);			            // Libera el semaforo	

//  ----------------------------------------------------------------------------------------------------------------------------
BaseType_t xQueueSend(QueueHandle_t xQueue,const void * pvItemToQueue,TickType_t xTicksToWait);
/*
Parameters:
	xQueue 	The handle to the queue on which the item is to be posted.
	pvItemToQueue 	A pointer to the item that is to be placed on the queue. The size of the items the queue will hold was defined when the queue was created, so this many bytes will be copied from pvItemToQueue into the queue storage area.
	xTicksToWait 	The maximum amount of time the task should block waiting for space to become available on the queue, should it already be full. The call will return immediately if the queue is full and xTicksToWait is set to 0. The time is defined in tick periods so the constant portTICK_PERIOD_MS should be used to convert to real time if this is required.
			If INCLUDE_vTaskSuspend is set to '1' then specifying the block time as portMAX_DELAY will cause the task to block indefinitely (without a timeout).
Returns:
	pdTRUE if the item was successfully posted, otherwise errQUEUE_FULL. 
*/

xQueueSend(DataQueueHandle,(uint32_t)dato,portMAX_DELAY);	// Envia el contendio de dato de la queue DataQueueHandle, si esta llena se bloquea hasta que haya lugar

-//  ---------------------------------------------------------------------------------------------------------------------------
BaseType_t xQueueReceive(QueueHandle_t xQueue,void *pvBuffer,TickType_t xTicksToWait);
/*
Parameters:
	xQueue 	The handle to the queue from which the item is to be received.
	pvBuffer 	Pointer to the buffer into which the received item will be copied.
	xTicksToWait 	The maximum amount of time the task should block waiting for an item to receive should the queue be empty at the time of the call. Setting 			xTicksToWait to 0 will cause the function to return immediately if the queue is empty. The time is defined in tick periods so the constant portTICK_PERIOD_MS should be used to convert to real time if this is required.
			If INCLUDE_vTaskSuspend is set to '1' then specifying the block time as portMAX_DELAY will cause the task to block indefinitely (without a timeout).
Returns:
	pdTRUE if an item was successfully received from the queue, otherwise pdFALSE.
*/

xQueueReceive(DataQueueHandle,(uint32_t)dato,portMAX_DELAY);	// Saca de la queue DataQueueHandle y lo guarda en dato, si no hay dato se bloquea hasta que halla

//  ----------------------------------------------------------------------------------------------------------------------------

/*	FUNCION PARA CONTAR TICKS DEL SISTEMA FREERTOS	*/
volatile TickType_t xTaskGetTickCount( void );

Returns:
	The count of ticks since vTaskStartScheduler was called.