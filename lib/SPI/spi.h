#ifndef SPI_H_
#define SPI_H_

#include "stm32f103xb.h"
#include <stdint.h>

#define PIN_CONFIG_SPI_AF   0xB // AF Output Push-pull, 50MHz (SCK, MOSI)
#define PIN_CONFIG_SPI_IN   0x4 // Input Floating (MISO)
#define PIN_CONFIG_SPI_SS   0x3 // GP Output Push-pull, 50MHz (SS)


void spi_init(SPI_TypeDef * SPIx, uint8_t mode);

uint8_t spi_transfer_byte(SPI_TypeDef * SPIx, uint8_t data);

void spi_send_char(SPI_TypeDef * SPIx, uint8_t data);

uint8_t spi_receive_char(SPI_TypeDef * SPIx);

void spi_ss_enable(SPI_TypeDef * SPIx);

void spi_ss_disable(SPI_TypeDef * SPIx);

#endif 