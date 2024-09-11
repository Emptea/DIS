#ifndef __USART_EX_H__
#define __USART_EX_H__

#include "stm32h7xx_ll_usart.h"
#include "stm32h7xx_ll_dma.h"
#include "protocol.h"

extern uint8_t uart_tx_buf[RE_SG_LEN];

void uart_send_array(USART_TypeDef *USARTx, uint8_t *debug_buf, uint32_t length);

void uart_dma_tx_config();
void uart_dma_rx_config();

void uart_dma_echo();
void uart_dma_transmit_sg();

void uart_dma_tx_handler();
void uart_dma_rx_handler();

void uart_timeout_config();
#endif