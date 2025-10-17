#include "i2c.h"

void i2c_init(I2C_TypeDef* i2c_inst) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN; // Clock para GPIOB y AFIO

    if (i2c_inst == I2C1) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; // Clock para I2C1

        // Configurar PB6 y PB7 como AF Open-Drain
        GPIOB->CRL &= ~(GPIO_CRL_CNF6 | GPIO_CRL_MODE6 | GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
        GPIOB->CRL |= (GPIO_CRL_CNF6 | GPIO_CRL_MODE6);
        GPIOB->CRL |= (GPIO_CRL_CNF7 | GPIO_CRL_MODE7);
    } else if (i2c_inst == I2C2) {
        RCC->APB1ENR |= RCC_APB1ENR_I2C2EN; // Clock para I2C2

        // Configurar PB10 y PB11 como AF Open-Drain
        GPIOB->CRH &= ~(GPIO_CRH_CNF10 | GPIO_CRH_MODE10 | GPIO_CRH_CNF11 | GPIO_CRH_MODE11);
        GPIOB->CRH |= (GPIO_CRH_CNF10 | GPIO_CRH_MODE10);
        GPIOB->CRH |= (GPIO_CRH_CNF11 | GPIO_CRH_MODE11);
    }

    // Configuración genérica del periférico I2C
    i2c_inst->CR1 |= I2C_CR1_SWRST;
    i2c_inst->CR1 &= ~I2C_CR1_SWRST;

    i2c_inst->CR2 = 36;    // Frecuencia de PCLK1 en MHz
    i2c_inst->CCR = 180;   // 100kHz SCL
    i2c_inst->TRISE = 37;
    i2c_inst->CR1 |= I2C_CR1_PE; // Habilitar periférico
}

void i2c_start(I2C_TypeDef* i2c_inst) {
    i2c_inst->CR1 |= I2C_CR1_START;
    while (!(i2c_inst->SR1 & I2C_SR1_SB));
}

void i2c_stop(I2C_TypeDef* i2c_inst) {
    i2c_inst->CR1 |= I2C_CR1_STOP;
    while(i2c_inst->CR1 & I2C_CR1_STOP);
}

void i2c_send_direccion(I2C_TypeDef* i2c_inst, unsigned char direccion) {
    i2c_inst->DR = direccion;
    while (!(i2c_inst->SR1 & I2C_SR1_ADDR));
    i2c_inst->SR2; // Limpiar flag ADDR
}

void i2c_send_byte(I2C_TypeDef* i2c_inst, unsigned char dato) {
    while (!(i2c_inst->SR1 & I2C_SR1_TXE));
    i2c_inst->DR = dato;
    while (!(i2c_inst->SR1 & I2C_SR1_BTF));
}

unsigned char i2c_recibir_byte_ack(I2C_TypeDef* i2c_inst) {
    // Hablito ACK
    i2c_inst->CR1 |= I2C_CR1_ACK;
    
    // Esperar hasta que RX no esté vacío (RXNE = 1)
    while (!(i2c_inst->SR1 & I2C_SR1_RXNE));
    return i2c_inst->DR;
}

unsigned char i2c_recibir_byte_nack(I2C_TypeDef* i2c_inst) {
    // Deshabilitar ACK (para indicar que este es 
    i2c_inst->CR1 &= ~I2C_CR1_ACK;
    
    // Esperar hasta que RX no esté vacío (RXNE = 1)
    while (!(i2c_inst->SR1 & I2C_SR1_RXNE));
    
    return i2c_inst->DR;
}