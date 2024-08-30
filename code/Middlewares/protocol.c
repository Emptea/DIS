#include "protocol.h"
#include "usart_ex.h"
#include "adc_ex.h"
#include "gpio_ex.h"
#include "tim_ex.h"

enum cmd {
    CND_NONE = 0x00,
    CMD_PWR_ON = 0x01,
    CMD_PWR_OFF = 0x02,
    CMD_START_CONV = 0x03,
    CMD_SEND_SG = 0x04,
};


void cmd_work(uint8_t cmd)
{
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
    case CMD_SEND_SG: {
        uart_dma_transmit_sg();
    } break;
    default: {
    } break;
    }
}
