.syntax unified
.thumb
.section .text
.align 2

.global MyToupperASM
.type MyToupperASM ,%function
// R0 = caracter
 MyToupperASM:
    CMP R0, #96
    BLT fin_MyTupperASM     // si es menor a 97 no es minuscula
    CMP R0, #123
    BGT fin_MyTupperASM     // si es mayor a 122 no es minuscula
    SUB R0, R0 , #32        // Le resto 32 a la letra minuscula para que sea mayuscula
   fin_MyTupperASM:
    BX  LR					// Retorno de la funcion

.global calcularFactorialASM
.type calcularFactorialASM ,%function
// R0 = numero para factorial
 calcularFactorialASM:
 	MOV	R1,#1
 	MOV	R2,#1
 ciclo_multiplicacion:
 	MUL R1,R2,R1				// R1 = R1 * R2
 	ADD R2,R2,#1				// R2 = R2 + 1
	CMP R2,R0					// comparo si ya llegue a multiplican n veces
	BNE ciclo_multiplicacion	// si no llegue sigo multiplicando
	MOV R0,R1
	BX	LR

.end
