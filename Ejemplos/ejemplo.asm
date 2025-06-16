.syntax unified
.thumb
.section .text
.align 2

.global FUNCION
type FUNCION %FUNCION
    ADD R0 , R0 , R1    // R0 = R0 + R1
SUMAR:
    ADD R2, R2 , #1     // R2 = R2 + 1
    MULLS R3, R0 , R2    // R3 = R0 * R2
    B VC FIN            // 
    B SUMAR             // SALTO A LA LINEA SUMAR ( BULCE INFINITO )