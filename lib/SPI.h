#ifndef SPI_H
#define SPI_H

#include "stm32f103xb.h"

void spi_init(SPI_TypeDef* spi_inst);
void spi_ss(SPI_TypeDef* spi_inst);
void spi_ds(SPI_TypeDef* spi_inst); //deseleccionar slave
void spi_8bit(SPI_TypeDef* spi_inst);
unsigned char spi_8bit_rw(SPI_TypeDef* spi_inst, unsigned char dato);
void spi_16bit(SPI_TypeDef* spi_inst);
uint16_t spi_16bit_rw(SPI_TypeDef* spi_inst, uint16_t dato);

#endif
