#ifndef __USART_EX_H__
#define __USART_EX_H__

#include "stm32h7xx_ll_usart.h"

void uart_send_array(USART_TypeDef *USARTx, uint8_t *debug_buf, uint32_t length);
#endif