#include "usart.h"

// FRECUENCIAS TODO PARA SYSCLK = 72MHz
#define PCLK2_FREQ 72000000 // Bus para USART1
#define PCLK1_FREQ 36000000 // Bus para USART2 y USART3

void usart_init(USART_TypeDef * USARTx, uint32_t BaudRate) {
    
    uint32_t pclk_freq = 0;
    
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; 

    if (USARTx == USART1)
    {
        // PA9-TX, PA10-RX
        pclk_freq = PCLK2_FREQ;
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;
        
        GPIOA->CRH &= ~((0xF << (9%8)*4) | (0xF << (10%8)*4));
        // Configurar
        GPIOA->CRH |= (PIN_CONFIG_TX << (9%8)*4); // PA9 (TX)
        GPIOA->CRH |= (PIN_CONFIG_RX << (10%8)*4); // PA10 (RX)
    }
    else if (USARTx == USART2)
    {
        // PA2-TX, PA3-RX
        pclk_freq = PCLK1_FREQ;
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
        
        GPIOA->CRL &= ~((0xF << 2*4) | (0xF << 3*4));
        GPIOA->CRL |= (PIN_CONFIG_TX << 2*4); // PA2 (TX)
        GPIOA->CRL |= (PIN_CONFIG_RX << 3*4); // PA3 (RX)
    }
    else if (USARTx == USART3)
    {
        // PB10-TX, PB11-RX
        pclk_freq = PCLK1_FREQ;
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
        RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
        
        GPIOB->CRH &= ~((0xF << (10%8)*4) | (0xF << (11%8)*4));
        GPIOB->CRH |= (PIN_CONFIG_TX << (10%8)*4);  // PB10 (TX)
        GPIOB->CRH |= (PIN_CONFIG_RX << (11%8)*4);  // PB11 (RX)
    }

    // Calculo baudrate CAMBIEN EN SUS CODIGOS !!!!
    float divisor = (float)pclk_freq / (16.0f * (float)BaudRate);
    uint16_t mantissa = (uint16_t)divisor; // Parte entera
    uint16_t fraction = (uint16_t)((divisor - (float)mantissa) * 16.0f); // Parte con coma
    USARTx->BRR = (mantissa << 4) | (fraction & 0x0F);

    USARTx->CR1 = 0; 
    USARTx->CR1 |= (USART_CR1_UE | USART_CR1_TE | USART_CR1_RE);
}

void usart_send_char(USART_TypeDef * USARTx, uint8_t data) {
    while (!(USARTx->SR & USART_SR_TXE));
    USARTx->DR = data;
}

uint8_t usart_receive_char(USART_TypeDef * USARTx) {
    while (!(USARTx->SR & USART_SR_RXNE));
    return (uint8_t)USARTx->DR;
}