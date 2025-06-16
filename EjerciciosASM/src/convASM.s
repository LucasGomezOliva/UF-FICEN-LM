.syntax unified
.thumb
.section .text
.align 2

.global convASM
.type convASM, %function
// R0 = puntero vector x
// R1 = longitud vector x ( nx )
// R2 = puntero vector h
// R3 = longitud vector h ( nh )
// R4 = puntero de salida (vector y)
convASM:
 	POP { R4 }							// Recupero el 5 parametro de la funcion
 	PUSH { R5 - R11 }
  	MOV R5, #0                          // Inicializo i = 0
  	MOV R6, #0                          // Inicializo j = 0
  	MOV R8, #0							// Usado para leer valor del vector x
  	MOV R11, #0							// Usado para leer valor del vector h
  	ADD R7, R1, R3                      // R7 = nx + nh
CicloInicializacion:
  	STR R8, [R4, R5, LSL #2]            // y[i] = 0
  	ADD R5, R5, #1                      // R5 = R5 + 1
  	CMP R5, R7                          // Comparo si i < ( nx + nh)
  	BLT CicloInicializacion             // Mientras se cumple sigo iterando
  	MOV R5, #0                          // Inicializo i = 0
  	MOV R6, #0                          // Inicializo j = 0
CicloMultiplicacionExterno:
  	MOV R6, #0                          // Inicializo j = 0 ( reseteo el valor de j para cada iteracion )
CicloMultiplicacionInterno:
  	LDR R8, [R0, R5, LSL #2]            // R8 = x[i]
  	LDR R9, [R2, R6, LSL #2]            // R9 = h[j]
  	ADD R11,R5,R6						// R11 = R5 + R6
  	LDR R10, [R4, R11, LSL #2]          // R10 = y[ i + j ]
  	MLA R10, R8, R9, R10                // R10 += R8 * R9
  	STR R10, [R4, R11, LSL #2]          // y[ i + j ] = x[ i ] * h[ j ] + y[ i + j ]
  	ADD R6, R6, #1                      // R6 = R6 + 1
  	CMP R6, R3                          // Comparo si j < nh
  	BLT CicloMultiplicacionInterno		// Mientras se cumple sigo iterando
  	ADD R5, R5, #1                      // R5 = R5 + 1
  	CMP R5, R1                          // Comparo si i < nx
  	BLT CicloMultiplicacionExterno      // Mientras se cumple sigo iterando
  	POP { R5 - R11 }
  	BX LR
.end

