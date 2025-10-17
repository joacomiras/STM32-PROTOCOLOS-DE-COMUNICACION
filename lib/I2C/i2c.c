#include "i2c.h"

// SYSCLK a 72MHz
#define PCLK1_FREQ 36 

void i2c_init(I2C_TypeDef * I2Cx)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;

	if (I2Cx == I2C1)
	{
		//PB6-SCL, PB7-SDA
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; 
        
		GPIOB->CRL &= ~((0xF << 6*4) | (0xF << 7*4));
		GPIOB->CRL |= (PIN_CONFIG_I2C << 6*4); // PB6 (SCL1)
		GPIOB->CRL |= (PIN_CONFIG_I2C << 7*4); // PB7 (SDA1)
	}
	else if (I2Cx == I2C2)
	{
		// PB10-SCL, PB11-SDA
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN; 
        
		GPIOB->CRH &= ~((0xF << (10%8)*4 ) | (0xF << (11%8)*4));
		GPIOB->CRH |= (PIN_CONFIG_I2C << (10%8)*4 );  // PB10 (SCL2)
		GPIOB->CRH |= (PIN_CONFIG_I2C << (11%8)*4); // PB11 (SDA2)
	}

	I2Cx->CR1 |= I2C_CR1_SWRST;  
	I2Cx->CR1 &= ~I2C_CR1_SWRST; 
	// Tiempos para 100KHz (con PCLK1=36MHz)
	I2Cx->CR2 |= (PCLK1_FREQ << 0); 
	I2Cx->CCR |= (180 << 0);    
	I2Cx->TRISE |= (PCLK1_FREQ + 1); 
	I2Cx->CR1 |= I2C_CR1_PE;    
}

void i2c_start(I2C_TypeDef * I2Cx) {
	I2Cx->CR1 |= I2C_CR1_START;         
	while(!(I2Cx->SR1 & I2C_SR1_SB)); 
}

void i2c_stop(I2C_TypeDef * I2Cx) {
    // CORRECCIÓN: Esperar a que el byte se termine de enviar (BTF)
	while(!(I2Cx->SR1 & I2C_SR1_TXE)); 
	while(!(I2Cx->SR1 & I2C_SR1_BTF)); 
	I2Cx->CR1 |= I2C_CR1_STOP; 
}

void i2c_address(I2C_TypeDef * I2Cx, uint8_t address) {
	I2Cx->DR = address;                      
	while(!(I2Cx->SR1 & I2C_SR1_ADDR));  
    // CORRECCIÓN: Limpiar flag ADDR leyendo SR1 y SR2
	I2Cx->SR2; 
}

void i2c_send_char(I2C_TypeDef * I2Cx, uint8_t data) {
	while(!(I2Cx->SR1 & I2C_SR1_TXE)); 
	I2Cx->DR = data; 
}


uint8_t i2c_receive_ack(I2C_TypeDef * I2Cx) {
	I2Cx->CR1 |= I2C_CR1_ACK;
	while (!(I2Cx->SR1 & I2C_SR1_RXNE));
	return I2Cx->DR;
}

uint8_t i2c_receive_nack(I2C_TypeDef * I2Cx) {
	I2Cx->CR1 &= ~I2C_CR1_ACK;
	I2Cx->CR1 |= I2C_CR1_STOP;
	while (!(I2Cx->SR1 & I2C_SR1_RXNE));
	return I2Cx->DR;
}