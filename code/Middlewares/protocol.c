#include "protocol.h"
#include "usart_ex.h"
#include "adc_ex.h"
#include "gpio_ex.h"
#include "tim_ex.h"
#include "stm32h7xx_ll_exti.h"

#define cmd2uint(char1, char2, char3, char4) \
    ((char1 << 24) + (char2 << 16) + (char3 << 8) + char4)

const enum cmd {
    CND_NONE = cmd2uint('c', 'm', 'd', '0'),
    CMD_PWR_ON = cmd2uint('c', 'm', 'd', '1'),
    CMD_PWR_OFF = cmd2uint('c', 'm', 'd', '2'),
    CMD_EXTI_ON = cmd2uint('c', 'm', 'd', '3'),
    CMD_START_CONV = cmd2uint('c', 'm', 'd', '7'),
    CMD_SEND_RES = cmd2uint('c', 'm', 'd', '8'),
    CMD_SEND_SG = cmd2uint('c', 'm', 'd', '9'),
    CMD_PING = cmd2uint('p', 'i', 'n', 'g'),
} cmd;

void conv_sg()
{
}

void cmd_work(uint8_t *buf)
{
    enum cmd cmd = (enum cmd)(*(uint32_t *)buf);
    switch (cmd) {
    case CMD_PWR_ON: {
        pwr_on();
        uart_dma_echo();
    } break;
    case CMD_PWR_OFF: {
        pwr_off();
        uart_dma_echo();
    } break;
    case CMD_EXTI_ON: {
        LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_0);
        uart_dma_echo();
    } break;
    case CMD_START_CONV: {
        uart_dma_echo();
        adc_start();
        tim_on();
    } break;
    case CMD_SEND_RES: {
    } break;
    case CMD_SEND_SG: {
        uart_dma_transmit_sg();
    } break;
    case CMD_PING: {
        uart_dma_echo();
    } break;
    default: {
    } break;
    }
}

void EXTI0_IRQHandler(void)
{
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET) {
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
        adc_start();
        tim_on();
    }
}
