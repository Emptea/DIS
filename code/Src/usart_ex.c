#include "usart_ex.h"

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

void uart_dma_rx_config()
{
    LL_USART_EnableDMAReq_RX(USART1);
    LL_DMA_ConfigAddresses(
        DMA1,
        LL_DMA_STREAM_1,
        LL_USART_DMA_GetRegAddr(USART1, LL_USART_DMA_REG_DATA_RECEIVE),
        (uint32_t)&uart_rx_buf,
        LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_1));
    LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, 2*RE_SG_LEN);

    LL_DMA_ClearFlag_TE1(DMA1);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_1);
    LL_DMA_ClearFlag_TC1(DMA1);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_1);
}

void uart_dma_tx_config()
{
    LL_USART_EnableDMAReq_TX(USART1);
    LL_DMA_ConfigAddresses(
        DMA1,
        LL_DMA_STREAM_2,
        (uint32_t)&uart_tx_buf,
        LL_USART_DMA_GetRegAddr(USART1, LL_USART_DMA_REG_DATA_TRANSMIT),
        LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_2));
    LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, 2*RE_SG_LEN);

    LL_DMA_ClearFlag_TE2(DMA1);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_2);
    LL_DMA_ClearFlag_TC2(DMA1);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_2);
}

void uart_dma_transmit_sg()
{
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_2, (uint32_t)&sg);
    //LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_2, RE_SG_LEN);
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_2);
}

void uart_dma_tx_handler()
{
    if (LL_DMA_IsActiveFlag_TC2(DMA1)) {
        LL_DMA_ClearFlag_TC2(DMA1);
    }

    if (LL_DMA_IsActiveFlag_TE2(DMA1)) {
        LL_DMA_ClearFlag_TE2(DMA1);
    }
}

void uart_dma_rx_handler()
{
    if (LL_DMA_IsActiveFlag_TC1(DMA1)) {
        LL_DMA_ClearFlag_TC1(DMA1);
    }

    if (LL_DMA_IsActiveFlag_TE1(DMA1)) {
        LL_DMA_ClearFlag_TE1(DMA1);
    }
}
