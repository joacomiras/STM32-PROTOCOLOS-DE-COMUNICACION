#include "spi.h"

void spi_init(SPI_TypeDef * SPIx)
{
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;

	if (SPIx == SPI1)
	{
		// PA4-SS, PA5-SCK, PA6-MISO, PA7-MOSI
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_SPI1EN;

		GPIOA->CRL &= ~((0xF << 4*4) | (0xF << 5*4) | (0xF << 6*4) | (0xF << 7*4));
		GPIOA->CRL |= (PIN_CONFIG_SPI_SS  << 4*4); // PA4 (SS)
		GPIOA->CRL |= (PIN_CONFIG_SPI_AF  << 5*4); // PA5 (SCK)
		GPIOA->CRL |= (PIN_CONFIG_SPI_IN  << 6*4); // PA6 (MISO)
		GPIOA->CRL |= (PIN_CONFIG_SPI_AF  << 7*4); // PA7 (MOSI)
        
        // Poner SS en ALTO (deseleccionado)
        GPIOA->BSRR = (1 << 4);
	}
	else if (SPIx == SPI2)
	{
		// PB12-SS, PB13-SCK, PB14-MISO, PB15-MOSI
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; 
		RCC->APB1ENR |= RCC_APB1ENR_SPI2EN; 

		GPIOB->CRH &= ~((0xF << (12%8)*4) | (0xF << (13%8)*4) | (0xF << (14%8)*4) | (0xF << (15%8)*4));
        // Configurar
		GPIOB->CRH |= (PIN_CONFIG_SPI_SS  << (12%8)*4); // PB12 (SS)
		GPIOB->CRH |= (PIN_CONFIG_SPI_AF  << (13%8)*4); // PB13 (SCK)
		GPIOB->CRH |= (PIN_CONFIG_SPI_IN  << (14%8)*4); // PB14 (MISO)
		GPIOB->CRH |= (PIN_CONFIG_SPI_AF  << (15%8)*4); // PB15 (MOSI)

        // Poner SS en ALTO (deseleccionado)
        GPIOB->BSRR = (1 << 12);
	}
    
	SPIx->CR1 = 0; 

    // PCLK2/4 = 18MHz (para SPI1)
    // PCLK1/2 = 18MHz (para SPI2)
    if(SPIx == SPI1) {
        SPIx->CR1 |= SPI_CR1_BR_0; // Divisor /4
    } else {
        SPIx->CR1 |= 0; // Divisor /2
    }

	SPIx->CR1 |= SPI_CR1_MSTR;  // Modo Maestro
   
	
}

uint8_t spi_transfer_byte(SPI_TypeDef * SPIx, uint8_t data)
{
	while (!(SPIx->SR & SPI_SR_TXE));
	SPIx->DR = data; 
	while (!(SPIx->SR & SPI_SR_RXNE));
	return SPIx->DR; 
}

void spi_send_char(SPI_TypeDef * SPIx, uint8_t data)
{
    // No se lee la respuesta
	spi_transfer_byte(SPIx, data);
}

uint8_t spi_receive_char(SPI_TypeDef * SPIx)
{
    // Transferir un byte dummy (0xFF es estándar) para recibir
	return spi_transfer_byte(SPIx, 0xFF);
}

void spi_ss_enable(SPI_TypeDef * SPIx)
{
	if(SPIx == SPI1) {
		GPIOA->BSRR = (1 << (4 + 16)); 
	}
	else if(SPIx == SPI2) {
		GPIOB->BSRR = (1 << (12 + 16)); 
	}
}

void spi_ss_disable(SPI_TypeDef * SPIx)
{
	if(SPIx == SPI1) {
		GPIOA->BSRR = (1 << 4); 
	}
	else if(SPIx == SPI2) {
		GPIOB->BSRR = (1 << 12); 
	}
}