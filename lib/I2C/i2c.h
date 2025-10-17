#ifndef I2C_H_
#define I2C_H_

#include "stm32f103xb.h"
#include <stdint.h>

#define PIN_CONFIG_I2C 0xF // AF Output Open-Drain, 50MHz

void i2c_init(I2C_TypeDef * I2Cx);

void i2c_start(I2C_TypeDef * I2Cx);

void i2c_stop(I2C_TypeDef * I2Cx);

void i2c_address(I2C_TypeDef * I2Cx, uint8_t address);

void i2c_send_char(I2C_TypeDef * I2Cx, uint8_t data);

uint8_t i2c_receive_ack(I2C_TypeDef * I2Cx);

uint8_t i2c_receive_nack(I2C_TypeDef * I2Cx);

#endif 