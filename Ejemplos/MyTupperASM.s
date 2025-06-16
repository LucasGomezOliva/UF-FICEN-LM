.syntax unified
.thumb
.section .text
.align 2

.global MyTupperASM, %FUNCION

// R0 = caracter

    CMP R0, #96
    BLT fin_MyTupperASM     // si es menor a 97 no es minuscula
    CMP R0, #123
    BLT fin_MyTupperASM     // si es mayor a 122 no es minuscula

    SUB R0, R0 , #32        // Le resto 32 a la letra minuscula para que sea mayuscula

    fin_MyTupperASM:
    BX  LR


// LDR  R3, [R0,R2,LSL,2]