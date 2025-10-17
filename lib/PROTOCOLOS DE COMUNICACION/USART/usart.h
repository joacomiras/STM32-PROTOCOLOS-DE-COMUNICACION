#ifndef USART_H
#define USART_H

#include "stm32f103xb.h"

void usart_init(USART_TypeDef* usart_inst, unsigned int baudrate);
void usart_send_char(USART_TypeDef* usart_inst, char c);
void usart_send_string(USART_TypeDef* usart_inst, const char* str);
char usart_recibir_char(USART_TypeDef* usart_inst);

#endif