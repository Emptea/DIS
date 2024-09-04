#include "protocol.h"
#include "usart_ex.h"
#include "adc_ex.h"
#include "gpio_ex.h"
#include "tim_ex.h"

#define cmd2uint(char1, char2, char3, char4) \
    ((char4 << 24) + (char3 << 16) + (char2 << 8) + char1)

const enum cmd {
    CND_NONE = cmd2uint('c', 'm', 'd', '0'),
    CMD_PWR_ON = cmd2uint('c', 'm', 'd', '1'),
    CMD_PWR_OFF = cmd2uint('c', 'm', 'd', '2'),
    CMD_START_CONV = cmd2uint('c', 'm', 'd', '3'),
    CMD_SEND_RES = cmd2uint('c', 'm', 'd', '4'),
    CMD_SEND_SG = cmd2uint('c', 'm', 'd', '5'),
} cmd;


void cmd_work(uint8_t *buf)
{
    enum cmd cmd = (enum cmd)(*(uint32_t *)buf);
    switch (cmd) {
    case CMD_PWR_ON: {
        pwr_on();
        LL_USART_TransmitData8(USART1, (uint8_t)cmd);
    } break;
    case CMD_PWR_OFF: {
        pwr_off();
        LL_USART_TransmitData8(USART1, (uint8_t)cmd);
    } break;
    case CMD_START_CONV: {
        adc_start();
        tim_on();
    } break;
    case CMD_SEND_RES:{
    } break;
    case CMD_SEND_SG: {
        uart_dma_transmit_sg();
    } break;
    default: {
    } break;
    }
}
