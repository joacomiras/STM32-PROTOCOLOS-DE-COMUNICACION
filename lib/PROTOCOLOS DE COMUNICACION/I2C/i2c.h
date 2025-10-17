#ifndef I2C_H
#define I2C_H

#include "stm32f103xb.h"

void i2c_init(I2C_TypeDef* i2c_inst);
void i2c_start(I2C_TypeDef* i2c_inst);
void i2c_stop(I2C_TypeDef* i2c_inst);
void i2c_send_direccion(I2C_TypeDef* i2c_inst, unsigned char direccion);
void i2c_send_byte(I2C_TypeDef* i2c_inst, unsigned char dato);
unsigned char i2c_recibir_byte_ack(I2C_TypeDef* i2c_inst);
unsigned char i2c_recibir_byte_nack(I2C_TypeDef* i2c_inst);

#endif