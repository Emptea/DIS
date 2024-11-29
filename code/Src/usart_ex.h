#ifndef __USART_EX_H__
#define __USART_EX_H__

#include "stm32h7xx_ll_usart.h"
#include "stm32h7xx_ll_dma.h"

#define USART_TO_VAL 8 * (5 * 4)

void uart_send_array(USART_TypeDef *USARTx, uint8_t *debug_buf, uint32_t length);

void uart_dma_tx_config(void *buf, uint32_t size);
void uart_dma_rx_config(void *buf, uint32_t size);
void uart_timeout_config();

void uart_dma_send(void *buf, uint32_t size);
void uart_dma_recv(void *buf, uint32_t size);

void uart_send_dma_callback(void);
void uart_recv_dma_callback(void);
void uart_recv_timeout_callback(void);
#endif