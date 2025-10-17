#include "usart.h"

int main(void) {
    // Inicializar dos puertos serie a diferentes velocidades
    usart_iniciar(USART1, 9600);
    usart_iniciar(USART2, 115200);

    usart_enviar_cadena(USART1, "Hola desde USART1\r\n");
    usart_enviar_cadena(USART2, "Hola desde USART2\r\n");

    while(1) {
        // Eco en USART1
        char c1 = usart_recibir_caracter(USART1);
        usart_enviar_caracter(USART1, c1);
    }
}