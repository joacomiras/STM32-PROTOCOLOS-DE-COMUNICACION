#include "spi.h"
/*
El prescaler divide la frecuencia máx del APB del que está por un valor de 2^(prescaler)
Es decir si pongo prescaler en 0, tengo para SPI1 72MHz/2=36MHz y para SPI2 36/2=18
*/
void spi_iniciar(SPI_TypeDef* spi_inst, uint32_t prescaler){
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

    if (spi_inst == SPI1) {
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN | RCC_APB2ENR_IOPAEN;
        // PA4(SS), PA5(SCK), PA6(MISO), PA7(MOSI)
        GPIOA->CRL &= 0x0000FFFF;
        GPIOA->CRL |= (GPIO_CRL_MODE4_1 | GPIO_CRL_MODE4_0); // PA4: Out PP
        GPIOA->CRL |= (GPIO_CRL_MODE5_1 | GPIO_CRL_MODE5_0 | GPIO_CRL_CNF5_1); // PA5: AF PP
        GPIOA->CRL |= GPIO_CRL_CNF6_0; // PA6: In Float
        GPIOA->CRL |= (GPIO_CRL_MODE7_1 | GPIO_CRL_MODE7_0 | GPIO_CRL_CNF7_1); // PA7: AF PP
        spi_ds(SPI1);
    } else if (spi_inst == SPI2) {
        RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
        // PB12(SS), PB13(SCK), PB14(MISO), PB15(MOSI)
        GPIOB->CRH &= 0x0000FFFF;
        GPIOB->CRH |= (GPIO_CRH_MODE12_1 | GPIO_CRH_MODE12_0); // PB12: Out PP
        GPIOB->CRH |= (GPIO_CRH_MODE13_1 | GPIO_CRH_MODE13_0 | GPIO_CRH_CNF13_1); // PB13: AF PP
        GPIOB->CRH |= GPIO_CRH_CNF14_1; // PB14: PU
        GPIOB->BSRR |= GPIO_BSRR_BS14;
        GPIOB->CRH |= (GPIO_CRH_MODE15_1 | GPIO_CRH_MODE15_0 | GPIO_CRH_CNF15_1); // PB15: AF PP
        spi_ds(SPI2);
    }
    
    // Configuración genérica del periférico
    spi_inst->CR1 = 0;
    spi_inst->CR1 |= prescaler;
    spi_inst->CR1 |= SPI_CR1_MSTR; // Modo Maestro
    spi_inst->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI; // SS por software
    spi_inst->CR1 |= SPI_CR1_SPE; // Habilitar SPI
}

void spi_ss(SPI_TypeDef* spi_inst) {
    if (spi_inst == SPI1) GPIOA->BSRR = GPIO_BSRR_BR4;
    else if (spi_inst == SPI2) GPIOB->BSRR = GPIO_BSRR_BR12;
}

void spi_ds(SPI_TypeDef* spi_inst) {
    if (spi_inst == SPI1) GPIOA->BSRR = GPIO_BSRR_BS4;
    else if (spi_inst == SPI2) GPIOB->BSRR = GPIO_BSRR_BS12;
}

void spi_8bit(SPI_TypeDef* spi_inst) {
    while (spi_inst->SR & SPI_SR_BSY);
    spi_inst->CR1 &= ~SPI_CR1_SPE;
    spi_inst->CR1 &= ~SPI_CR1_DFF;
    spi_inst->CR1 |= SPI_CR1_SPE;
}

unsigned char spi_8bit_rw(SPI_TypeDef* spi_inst, unsigned char dato) {
    while (!(spi_inst->SR & SPI_SR_TXE));
    spi_inst->DR = dato;
    while (!(spi_inst->SR & SPI_SR_RXNE));
    return spi_inst->DR;
}

void spi_16bit(SPI_TypeDef* spi_inst) {
    while (spi_inst->SR & SPI_SR_BSY);
    spi_inst->CR1 &= ~SPI_CR1_SPE;
    spi_inst->CR1 |= SPI_CR1_DFF;
    spi_inst->CR1 |= SPI_CR1_SPE;
}

uint16_t spi_16bit_rw(SPI_TypeDef* spi_inst, uint16_t dato) {
    while (!(spi_inst->SR & SPI_SR_TXE));
    spi_inst->DR = dato;
    while (!(spi_inst->SR & SPI_SR_RXNE));
    return spi_inst->DR;
}
