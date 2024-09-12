#include "usart_ex.h"

void uart_send_str(USART_TypeDef *USARTx, uint8_t *str)
{
    while (*str != 0) {
        while (!LL_USART_IsActiveFlag_TXE(USART3)) {
            ;
        }
        LL_USART_TransmitData8(USARTx, *str++);
    }
}

void uart_send_array(USART_TypeDef *USARTx, uint8_t *debug_buf, uint32_t length)
{
    do {
        while (!LL_USART_IsActiveFlag_TXE(USART3))
            ;
        LL_USART_TransmitData8(USARTx, *debug_buf++);
    } while (--length > 0);

    while (!LL_USART_IsActiveFlag_TC(USART3))
        ;
}

void uart_dma_rx_config(void *buf, uint32_t size)
{
    LL_USART_EnableDMAReq_RX(USART1);
    LL_DMA_ConfigAddresses(
        DMA1,
        LL_DMA_STREAM_1,
        LL_USART_DMA_GetRegAddr(USART1, LL_USART_DMA_REG_DATA_RECEIVE),
        (uint32_t)buf,
        LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_1));
    LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_1, size);

    LL_DMA_ClearFlag_TE1(DMA1);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_1);
    LL_DMA_ClearFlag_TC1(DMA1);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_1);

    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);
}

void uart_dma_tx_config(void *buf, uint32_t size)
{
    LL_USART_EnableDMAReq_TX(USART1);
    LL_DMA_ConfigAddresses(
        DMA1,
        LL_DMA_STREAM_2,
        (uint32_t)buf,
        LL_USART_DMA_GetRegAddr(USART1, LL_USART_DMA_REG_DATA_TRANSMIT),
        LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_2));
    LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, size);

    LL_DMA_ClearFlag_TE2(DMA1);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_2);
    LL_DMA_ClearFlag_TC2(DMA1);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_2);
}

void uart_timeout_config()
{
    LL_USART_SetRxTimeout(USART1, USART_TO_VAL);
    LL_USART_EnableRxTimeout(USART1);
    LL_USART_EnableIT_RTO(USART1);
}

void uart_dma_send(void *buf, uint32_t size)
{
    LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, size);
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_2, (uint32_t)buf);
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_2);
}

__WEAK void uart_send_dma_callback(void)
{
}

__WEAK void uart_recv_dma_callback(void)
{
}

void uart_dma_rx_handler()
{
    if (LL_DMA_IsActiveFlag_TC1(DMA1)) {
        LL_USART_DisableRxTimeout(USART1);
        LL_DMA_ClearFlag_TC1(DMA1);
        uart_recv_dma_callback();
    }

    if (LL_DMA_IsActiveFlag_TE1(DMA1)) {
        LL_DMA_ClearFlag_TE1(DMA1);
    }
}

void uart_dma_tx_handler()
{
    if (LL_DMA_IsActiveFlag_TC2(DMA1)) {
        LL_DMA_ClearFlag_TC2(DMA1);
        uart_send_dma_callback();
    }

    if (LL_DMA_IsActiveFlag_TE2(DMA1)) {
        LL_DMA_ClearFlag_TE2(DMA1);
    }
}


void DMA1_Stream1_IRQHandler(void)
{
    uart_dma_rx_handler();
}

void DMA1_Stream2_IRQHandler(void)
{
    uart_dma_tx_handler();
}

void USART1_IRQHandler(void)
{
    /* Check RXNE flag value in ISR register */
    if (LL_USART_IsActiveFlag_RTO(USART1) && LL_USART_IsEnabledIT_RTO(USART1)) {
        LL_USART_ClearFlag_RTO(USART1);
        LL_DMA_DisableStream(DMA1, LL_DMA_STREAM_1);
    }
}
