#include "usart.h"

void usart_init(USART_TypeDef* usart_inst, unsigned int baudrate) {
    // Habilitar reloj de funciones alternativas
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    unsigned int pclk; // Frecuencia del bus del periférico

    if (usart_inst == USART1) {
        // Habilitar clocks para USART1 y GPIOA
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN | RCC_APB2ENR_IOPAEN;
        pclk = 72000000; // PCLK2 para USART1

        // Configurar pines PA9 (TX) y PA10 (RX)
        GPIOA->CRH &= ~(GPIO_CRH_CNF9 | GPIO_CRH_MODE9 | GPIO_CRH_CNF10 | GPIO_CRH_MODE10);
        GPIOA->CRH |= (GPIO_CRH_MODE9_1 | GPIO_CRH_MODE9_0 | GPIO_CRH_CNF9_1); // PA9: AF Push-pull
        GPIOA->CRH |= GPIO_CRH_CNF10_0; // PA10: Input floating
    } else if (usart_inst == USART2) {
        // Habilitar clocks para USART2 y GPIOA
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
        pclk = 36000000; // PCLK1 para USART2

        // Configurar pines PA2 (TX) y PA3 (RX)
        GPIOA->CRL &= ~(GPIO_CRL_CNF2 | GPIO_CRL_MODE2 | GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
        GPIOA->CRL |= (GPIO_CRL_MODE2_1 | GPIO_CRL_MODE2_0 | GPIO_CRL_CNF2_1); // PA2: AF Push-pull
        GPIOA->CRL |= GPIO_CRL_CNF3_0; // PA3: Input floating
    } else if (usart_inst == USART3) {
        // Habilitar clocks para USART3 y GPIOB
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
        pclk = 36000000; // PCLK1 para USART3

        // Configurar pines PB10 (TX) y PB11 (RX)
        GPIOB->CRH &= ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10 | GPIO_CRH_CNF11 | GPIO_CRH_MODE11);
        GPIOB->CRH |= (GPIO_CRH_MODE10_1 | GPIO_CRH_MODE10_0 | GPIO_CRH_CNF10_1); // PB10: AF Push-pull
        GPIOB->CRH |= GPIO_CRH_CNF11_0; // PB11: Input floating
    }

    // Baudrate
    usart_inst->BRR = pclk / (16 * baudrate);

    // Habilitar USART, Transmisor y Receptor
    usart_inst->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void usart_send_char(USART_TypeDef* usart_inst, char c) {
    while (!(usart_inst->SR & USART_SR_TXE));
    usart_inst->DR = c;
}

void usart_send_string(USART_TypeDef* usart_inst, const char* str) {
    while (*str) {
        usart_enviar_caracter(usart_inst, *str++);
    }
}

char usart_recibir_char(USART_TypeDef* usart_inst) {
    while (!(usart_inst->SR & USART_SR_RXNE));
    return (char)usart_inst->DR;
}