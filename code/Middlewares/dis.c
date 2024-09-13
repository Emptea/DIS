#include "dis.h"
#include "usart_ex.h"
#include "adc_ex.h"
#include "gpio_ex.h"
#include "tim_ex.h"
#include "crc_ex.h"
#include "stm32h7xx_ll_exti.h"

#define cmd2uint(char1, char2, char3, char4) \
    ((char1 << 24) + (char2 << 16) + (char3 << 8) + char4)

#define SG_CHUNK_LEN   64000
#define SG_CHUNKS      RE_SG_LEN / SG_CHUNK_LEN
#define SG_UART_CHUNKS 2 * RE_SG_LEN / SG_CHUNK_LEN

enum cmd {
    CMD_NONE = cmd2uint('c', 'm', 'd', '0'),
    CMD_PWR_ON = cmd2uint('c', 'm', 'd', '1'),
    CMD_PWR_OFF = cmd2uint('c', 'm', 'd', '2'),
    CMD_EXTI_ON = cmd2uint('c', 'm', 'd', '3'),
    CMD_START_CONV = cmd2uint('c', 'm', 'd', '7'),
    CMD_SEND_RES = cmd2uint('c', 'm', 'd', '8'),
    CMD_SEND_SG = cmd2uint('c', 'm', 'd', '9'),
    CMD_PING = cmd2uint('p', 'i', 'n', 'g'),
};
static enum cmd cmd = CMD_NONE;

union {
    struct {
        uint32_t cmd;
        uint16_t crc;
    };

    uint8_t bytes[CMD_LEN + 2];
} rx_buf;

struct {
    uint32_t cmd;
    uint16_t crc;
} tx_buf;

int16_t sg[RE_SG_LEN];

static volatile enum {
    STATE_RCV = 0,
    STATE_SEND_SG = 1,
    STATE_SEND_CRC = 2,
    STATE_ECHO = 3,
    STATE_ERROR = 4,
} status = STATE_RCV;

static void conv_sg()
{
}

static void crc_check()
{
    uint16_t crc = crc_calc(rx_buf.bytes, CMD_LEN);
    if (rx_buf.crc != crc) {
        status = STATE_ERROR;
    }
    LL_CRC_SetInitialData(CRC, 0xFFFF);
}

static void dis_echo()
{
    uart_dma_send(&rx_buf, CMD_LEN + 2);
}

void dis_init()
{
    uart_timeout_config();

    adc_dma_config(&sg, SG_CHUNK_LEN);
    uart_dma_tx_config(&tx_buf, CMD_LEN + 2);
    uart_dma_rx_config(&rx_buf, CMD_LEN + 2);

    adc_calibration();
    adc_en();

    LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0);
}

void dis_cmd_work()
{
    tx_buf.cmd = rx_buf.cmd;
    switch (rx_buf.cmd) {
    case CMD_PWR_ON: {
        pwr_on();
        dis_echo();
    } break;
    case CMD_PWR_OFF: {
        pwr_off();
        dis_echo();
    } break;
    case CMD_EXTI_ON: {
        LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);
    } break;
    case CMD_START_CONV: {
        adc_dma_start(&sg, SG_CHUNK_LEN);
        tim_on();
    } break;
    case CMD_SEND_RES: {
    } break;
    case CMD_SEND_SG: {
        uart_dma_send(&tx_buf.cmd, CMD_LEN);
        crc_calc((uint8_t *)&cmd, CMD_LEN);
    } break;
    case CMD_PING: {
        dis_echo();
    } break;
    default: {
    } break;
    }
}

void uart_send_dma_callback()
{
    static uint32_t cnt = 0;
    switch (status) {
    case STATE_ERROR: {
        status = STATE_RCV;
        break;
    }
    case STATE_SEND_SG: {
        tx_buf.crc = crc_calc((uint8_t *)&sg[SG_CHUNK_LEN/2 * cnt], SG_CHUNK_LEN);
        uart_dma_send(&sg[SG_CHUNK_LEN/2 * (cnt++)], SG_CHUNK_LEN);
        if (cnt == SG_UART_CHUNKS) {
            status = STATE_SEND_CRC;
        };
        break;
    }
    case STATE_SEND_CRC: {
        uart_dma_send(&tx_buf.crc, 2);
        LL_CRC_SetInitialData(CRC, 0xFFFF);
        cnt = 0;
        status = STATE_RCV;
        break;
    }
    case STATE_ECHO: {
        status = STATE_RCV;
        break;
    }
    default: {
        status = STATE_RCV;
        break;
    }
    }
}

void uart_recv_dma_callback()
{
    switch (status) {
    case STATE_ERROR: {
        break;
    }
    case STATE_RCV: {
        dis_cmd_work();
        break;
    }
    default: {
        break;
    }
    }
    status = STATE_RCV;
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);
}

void EXTI0_IRQHandler(void)
{
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET) {
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
        tx_buf.cmd = CMD_SEND_SG;
        crc_calc((uint8_t *)&tx_buf.cmd, CMD_LEN);
        adc_dma_start(&sg, SG_CHUNK_LEN);
        tim_on();
        LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_0);
    }
}

void adc_dma_callback()
{
    static int cnt = 0;
    cnt++;
    if (cnt < SG_CHUNKS) {
        adc_dma_start(&sg[SG_CHUNK_LEN * cnt], SG_CHUNK_LEN);
    } else {
        tim_off();
        status = STATE_SEND_SG;
        uart_dma_send(&tx_buf.cmd, CMD_LEN);
        cnt = 0;
    }
}

/**
 * @brief This function handles DMA1 stream0 global interrupt (ADC).
 */
void DMA1_Stream0_IRQHandler(void)
{
    if (LL_DMA_IsActiveFlag_TC0(DMA1)) {
        LL_DMA_ClearFlag_TC0(DMA1);
        adc_dma_callback();
    }

    if (LL_DMA_IsActiveFlag_TE0(DMA1)) {
        LL_DMA_ClearFlag_TE0(DMA1);
    }
}
