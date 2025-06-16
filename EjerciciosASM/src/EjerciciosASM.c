#include"EjerciciosASM.h"

/*
 * Escribir la siguiente en función en assembler que convierte un carácter a mayúsculas. Si
 * c es un carácter en minúscula la función retorna su mayúscula correspondiente. Si c es
 * un carácter en mayúscula o un carácter no alfabético retorna ese mismo carácter.
 *
 * int32_t MyToupperASM(int32_t c)
 *
*/
int32_t MyToupperASM(int32_t c);

/*
 * Escribir la siguiente en función en assembler que calcule factorial de un número
 *
 * uint32_t calcularFactorialASM(uint32_t n)
 *
 */
uint32_t calcularFactorialASM(uint32_t n);

/*
 * Escribir una rutina en assembler que implemente el algoritmo de correlación
 * denominado input-side
 *
 */

void convASM(const int32_t *x, int32_t nx, const int32_t *h, int32_t nh, int32_t *y);

// ConvC: utilizada para comparar la velocidad de ejecucion con respecto a la version de asm
void convC(const int32_t *x, int32_t nx, const int32_t *h, int32_t nh, int32_t *y){
	int i, j;
	for( i = 0; i < ( nx + nh);i++){
		y[i] = 0;
	}
	for( i = 0; i < nx; i++){
		for( j = 0; j < nh; j++){
			y[i+j]+= h[j]*x[i];
		}
	}
}

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    // TODO: insert code here

    int32_t caracter = 104;
    MyToupperASM(caracter);

    uint32_t res =  calcularFactorialASM(6);

    int32_t caraacter = 104;

    const int32_t SeñalEntrada[5] = {1,2,3,4,46};
    int32_t LongitudEntrada = 5;
    const int32_t SeñalH[4] = {12,22,4,4};
    int32_t LongitudH = 4;
    int32_t yASM [8];
    int32_t yC [8];

    convASM(SeñalEntrada, LongitudEntrada, SeñalH, LongitudH, yASM);
    convC(SeñalEntrada, LongitudEntrada, SeñalH, LongitudH, yC);

    while(1) {

    }
    return 0 ;
}
