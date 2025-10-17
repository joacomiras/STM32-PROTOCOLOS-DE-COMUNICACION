#ifndef USART_H_
#define USART_H_

#include "stm32f103xb.h"
#include <stdint.h>

#define PIN_CONFIG_TX 0xB // AF Output Push-pull, 50MHz (1011b)
#define PIN_CONFIG_RX 0x4 // Input Floating (0100b)


void usart_init(USART_TypeDef * USARTx, uint32_t BaudRate);

void usart_send_char(USART_TypeDef * USARTx, uint8_t data);

uint8_t usart_receive_char(USART_TypeDef * USARTx);

#endif 