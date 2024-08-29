#include "usart_ex.h"

void uart_send_array(USART_TypeDef *USARTx, uint8_t *debug_buf, uint32_t length)
{
	do
	{
		while(!LL_USART_IsActiveFlag_TXE(USART3));
		LL_USART_TransmitData8(USARTx, *debug_buf++);
	} while (--length >0);
	
	while (!LL_USART_IsActiveFlag_TC(USART3));
}