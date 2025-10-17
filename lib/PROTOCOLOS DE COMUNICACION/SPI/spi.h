#ifndef SPI_H
#define SPI_H

#include "stm32f103xb.h"

void spi_init(SPI_TypeDef* spi_inst);
void spi_ss(SPI_TypeDef* spi_inst);
void spi_ds(SPI_TypeDef* spi_inst); //deseleccionar slave
unsigned char spi_rw_byte(SPI_TypeDef* spi_inst, unsigned char dato);

#endif