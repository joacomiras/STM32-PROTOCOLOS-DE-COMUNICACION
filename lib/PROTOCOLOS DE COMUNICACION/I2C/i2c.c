#include "i2c.h"
#include "stm32f103xb.h"

void i2c_init(I2C_TypeDef * I2Cx)
{
	// Habilitar Clocks de GPIO y AFIO
    // Ambos I2C1 y I2C2 están en el puerto B en el STM32F103
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; // Habilitar reloj para Puerto B
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // Habilitar funciones alternativas

	if (I2Cx == I2C1)
	{
		RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
		
		// Configurar Pines (PB6 - SCL, PB7 - SDA)
		// Limpiar bits de configuración para PB6 y PB7
		GPIOB->CRL &= ~((GPIO_CRL_CNF6 | GPIO_CRL_MODE6) | (GPIO_CRL_CNF7 | GPIO_CRL_MODE7)); 
		
		// Configurar PB6 y PB7 como Salida de Función Alternativa Open-Drain, 50MHz
		GPIOB->CRL |= (GPIO_CRL_CNF6_1 | GPIO_CRL_CNF6_0 | GPIO_CRL_MODE6_1 | GPIO_CRL_MODE6_0); // PB6 (SCL1)
		GPIOB->CRL |= (GPIO_CRL_CNF7_1 | GPIO_CRL_CNF7_0 | GPIO_CRL_MODE7_1 | GPIO_CRL_MODE7_0); // PB7 (SDA1)
	}
	else if (I2Cx == I2C2)
	{
		// Habilitar Reloj para I2C2
		RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;

		// Configurar Pines (PB10 - SCL, PB11 - SDA)
        // Estos pines están en el registro ALTO (CRH)
		
        // Limpiar bits de configuración para PB10 y PB11
		GPIOB->CRH &= ~((GPIO_CRH_CNF10 | GPIO_CRH_MODE10) | (GPIO_CRH_CNF11 | GPIO_CRH_MODE11));
		
		// Configurar PB10 y PB11 como Salida de Función Alternativa Open-Drain, 50MHz
		GPIOB->CRH |= (GPIO_CRH_CNF10_1 | GPIO_CRH_CNF10_0 | GPIO_CRH_MODE10_1 | GPIO_CRH_MODE10_0); // PB10 (SCL2)
		GPIOB->CRH |= (GPIO_CRH_CNF11_1 | GPIO_CRH_CNF11_0 | GPIO_CRH_MODE11_1 | GPIO_CRH_MODE11_0); // PB11 (SDA2)
	}
    

	// Configuración común de I2C (Reset, Tiempos y Habilitación)
	I2Cx->CR1 |= I2C_CR1_SWRST;  // Resetear el periférico
	I2Cx->CR1 &= ~I2C_CR1_SWRST; // Salir del reset

	// Configuración de Tiempos (para 100KHz SCL con PCLK1=36MHz)
	I2Cx->CR2 |= (36 << 0);     // 36MHz (Frecuencia del bus APB1)
	I2Cx->CCR |= (180 << 0);    // 100KHz SCL (CCR = (PCLK1 / (2 * SCL_Freq)))
	I2Cx->TRISE |= (37 << 0);   // (1000ns / T_PCLK1) + 1 
	
	I2Cx->CR1 |= I2C_CR1_PE;    // Habilitar el periférico I2C
}

void i2c_start(I2C_TypeDef * I2Cx)
{
	I2Cx->CR1 |= I2C_CR1_START;         
	while(!(I2Cx->SR1 & I2C_SR1_SB)); 
}


void i2c_send_direccion(I2C_TypeDef * I2Cx, char address)
{
	I2Cx->DR = address;                      
	while(!(I2Cx->SR1 & I2C_SR1_ADDR));  
	(void)I2Cx->SR2; 
}

void i2c_send_byte(I2C_TypeDef * I2Cx, char data)
{
	while(!(I2Cx->SR1 & I2C_SR1_TXE)); 
	I2Cx->DR = data; 
}

void i2c_stop(I2C_TypeDef * I2Cx)
{
	while(!(I2Cx->SR1 & I2C_SR1_TXE)); // Esperar a que DR esté vacío
	while(!(I2Cx->SR1 & I2C_SR1_BTF)); // Esperar a que el byte se haya transferido
	
	I2Cx->CR1 |= I2C_CR1_STOP; // Generar Stop
}
unsigned char i2c_recibir_byte_ack(I2C_TypeDef * I2Cx)
{
	// 1. Habilitar el ACK (para decirle al esclavo que queremos más datos)
	I2Cx->CR1 |= I2C_CR1_ACK;
    
	// 2. Esperar a que el buffer de recepción no esté vacío (RXNE = 1)
	while (!(I2Cx->SR1 & I2C_SR1_RXNE));
    
	// 3. Leer y devolver el dato (esto limpia el flag RXNE)
	return (uint8_t)I2Cx->DR;
}

/**
 * @brief Lee el último byte de datos y envía un NACK (Not Acknowledge).
 */
unsigned char i2c_recibir_byte_nack(I2C_TypeDef * I2Cx)
{
	// 1. Deshabilitar el ACK (para decirle al esclavo que este es el último byte)
	I2Cx->CR1 &= ~I2C_CR1_ACK;
	
	// 2. Generar la condición de STOP (esto debe hacerse ANTES de leer el último byte)
	I2Cx->CR1 |= I2C_CR1_STOP;
	
	// 3. Esperar a que el buffer de recepción no esté vacío (RXNE = 1)
	while (!(I2Cx->SR1 & I2C_SR1_RXNE));
	
	// 4. Leer y devolver el último dato
	return (uint8_t)I2Cx->DR;
}