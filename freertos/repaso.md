
#   C para sistemas embebidos

## Instrucciones de cabecera del punto H:

    --  IFNDEF - cabecera de funcion impide que se vuelva a delcarar el mismo .h
    --  volatile
    --  extern
    --  const
    --  tipos de datos uint32 ( rango de las variables )
    --  Operadores logicos ( como hacer desplazamientos , el tema de las mask, registros )
    --  Operadores bit a bt:    AND da un uno si la mask en 1
                                OR  da un cero al revez
# ARM:
    --  Caracteriticas
    --  Cuando se usa
    --  Especificaciones de cada uno

# LPC1769:
    --  Funcion de los pines:
                --  GPIO:
                        --  Como entrada
                        --  Como salida
                        --  Cada uno tiene una configuracion electrica pulldown pull up
                --  ADC:
                    --  Elegir CANAL
                    --  Frecuencia de conversion
                    --  Interrupcion ( crear handle, habilitar isr )
                    --  Uso ( configuracion de start )
                    -- Teoria:
                        --  TIpos de adc
                        --  Cual usa el lpc 
                        --  Ver en el ppt
    --  Timers:
                --  Timer de systick ( generar ticks del sistema )
                                        --  Determinar la frecuencia de disparo, utilizando a la frecuencia del sistema 96MHz
                                        --  Definir el isr handler de interrupcion
                --  Timer de proposito generlal:
                                    --  Elegir el timer que quiero usar ( habilitarlo )
                                    --  Elegir el clock del timer: (CREO QUE ERAN 3)
                                                        --  el del sistmea
                                                        --  el del sistema x 4
                                    -- Elegir el modo de uso:
                                            -- Contador 
                                                    -- como match ( cuando el tiemr llegue a la cuenta tal interrumpime ) hasta 4 match distitnos por timer 
                                                            Por ejemplo disparar una conversion de ADC utilizando el timer
                                                    -- o yo manualmente tengo que preguntar en x momento el valor del mismo
                                            -- Captura:
                                                    -- captura si hubo un cambio de flanco
                                                    -- si hubo un cambio de nivel
                --  Watchdog
                --  Timer rtc ( timer de tiempo real )
    -- Interrupciones externas:
                --  Hay 4 interrupciones externas
                --  Inicializar el pin
                --  Funcion:
                        --  flanco descendente
                        --  Flanco ascendente
                -- Habilitar las interrupciones
                -- Configurar el handel ( NO OLVIDAR DE BORRAR EL FLAG DE INTERRUPCION )


Ejemplo:

Si tengo una ADC de 8 bits y 0 a 5 Vref

--  RDE 5V
--  RDS 2^{8} = 256 niveles
--  LSB = RDE/RDS = 19mV
--  Hacer el grafico

En un dac de 10 bits con RDE 3V
RDS = 2^{10} = 1024 NIVELES
LSB = 2.92 mV

Nos puedo sacar el una tension mayor a mi 3V

Si tengo que sacar un a v = 1.75

hago V/LSB = con esto obtengo la cantidad de niveles

# ASSEMBLER:
    --  Formas de implementarlo:
            --  assembler inline ( volatile_asm "ADD R0 R1 #1" ,"R1"(C) , "R0"(x))
            --  funciones
            --  ASM.h tengo los prototipos en C
            --  fuente.c llamaba a la funcion en c como una funcion cualquiera
            --  en asm.s hago mi implimentacion
            --  Implimentacion:
                        --  Parametros de entrada
                        --  Parametros de salida: R0 pero puedo usar tmb R1 si tengo una varialbe de 64 bits
                        --  Registros 
                                --  De R0 a R3 la arquitectura los guarda
                                --  De R4 A R12 los tengo que guardar en la pila
                                --  Registros especiales:
                                            --  R15 ( PC - porgram counter : apunta a la instruccion a ejecutar)
                                            --  R14 ( LR - Link register - me guarda la direccion de memoria al program counter donde tengo que volver al salir de la funcion de assembler ) 
                                            --  R13 ( SP - Stack Pointer - puntero a la pila )

Branch condicion etiqueta ( cond es EQ NEQ y essas)
Branch and Link [ B COND etiqueta ] ( actualiza el valor del link register para saber a donde tengo que volver )

Si uso BL me tengo que guarar el LR en la pila para poder volver al lugar donde llame a la primer funcion

    PUSH{ LR }  -- en el mismo orden en el que los carque a la pila es el orden donde lo tengo que sacar
    BL set_zero
    POP{ LR }   - en el mismo orden en el que los carque a la pila es el orden donde lo tengo que sacar
    Bx LR

#  FreeRTOS

--  Sistema operativo a tiempo real
            --  Expropiatios: si necesito tomo el control
            --  Cooperativos: si no cedo el control nadie lo puede usar
--  Tareas:
        --  Estados Posibles: 
                --  Running ( de aca voy a redy o bloqueada)
                --  Ready   (de aca voy a running o a bloqueada)
                --  Block ( o suspendida ) (de aca voy unicamente a ready)
        --  Funciones: tienen que ser especificas
        --  Dos tipos principales:
                    --  Periodicas
                    --  A demanda de algun recurso (Ejemplo: porcesar adc del ejercicio 8 )
--  Schedule:  
        --  Periodico
        --  Realiza los cambios de contexto entre tarea ( Actualizacion de registros y datos claves ) esto se hace en funcion de las prioridaes y el estado de cada tarea
        --  Cada tarea en su tiempo de ejcucion tiene el control total del sistema
        --  Es el que maneja el sistema operativo
-- Semaforos:
        --  Binarios: Se pued eliberar desde otra tarea distinta a la que se llamo
        --  Mutex: Solo s epeude liberar en la mimsa tarea que lo tomo
        --  Contadores: Cuenta los accesos
-- Queue:
    --  Tipo de elemento especifico
    --  Tamaño finito
    --  Son de tipo FIFO pero se puede sacar de cualquiera de los dos lados
    --  Se utilizan para pasaje de datos


Ejemplo de clase:

int main(){

}

// recibe datos de una que y los suma y si suma mayor a 20 prendo un led
void task(void*pv){


}

adc_isr(9){

    sennd_queue_isr()-- tengo que usar una funcion espeical para mandar los datos de ina interrupcion a una que
}

.global func
.type func % function
// suma dos numeros y retorna su valor