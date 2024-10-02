#include "dis.h"
#include "usart_ex.h"
#include "adc_ex.h"
#include "gpio_ex.h"
#include "tim_ex.h"
#include "crc_ex.h"
#include "stm32h7xx_ll_exti.h"
#include "cmplx.h"
#include "array.h"

#define cmd2uint(char1, char2, char3, char4) \
    ((char1 << 24) + (char2 << 16) + (char3 << 8) + char4)

#define SG_LEN (uint32_t) 32768
#define CMD_LEN 4
#define SG_ADC_CHUNK_LEN      (uint32_t)32768
#define SG_CHUNK_BYTE_LEN     (uint32_t)32768
#define SG_ADC_CHUNKS          SG_LEN / SG_ADC_CHUNK_LEN

#define SG_CHUNK_HALFWORD_LEN SG_CHUNK_BYTE_LEN / 2
#define SG_CHUNK_WORD_LEN     SG_CHUNK_BYTE_LEN / 4
#define SG_HALFWORD_CHUNKS    2 * SG_LEN / SG_CHUNK_BYTE_LEN
#define SG_WORD_CHUNKS        4 * SG_LEN / SG_CHUNK_BYTE_LEN
#define FFT_WORD_CHUNKS       SG_WORD_CHUNKS / 2
#define SG_HALF_LEN           SG_LEN / 2

enum cmd {
    CMD_NONE = cmd2uint('c', 'm', 'd', '0'),
    CMD_PWR_ON = cmd2uint('c', 'm', 'd', '1'),
    CMD_PWR_OFF = cmd2uint('c', 'm', 'd', '2'),
    CMD_EXTI_ON = cmd2uint('c', 'm', 'd', '3'),
    CMD_SEND_RES = cmd2uint('c', 'm', 'd', '6'),
    CMD_START_CONV = cmd2uint('c', 'm', 'd', '7'),
    CMD_SEND_FFT = cmd2uint('c', 'm', 'd', '8'),
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

int16_t adc_data[SG_LEN] = {0};
static cmplx64_t fft[SG_LEN] = {0};
static float32_t fft_mag_sq[SG_HALF_LEN] = {0};

static volatile enum {
    UART_STATE_RCV = 0,
    UART_STATE_SEND_RES = 1,
    UART_STATE_SEND_CRC = 2,
    UART_STATE_ECHO = 3,
    UART_STATE_ERROR = 4,
    // debug states
    UART_STATE_SEND_FFT,
    UART_STATE_SEND_SG,
} uart_state = UART_STATE_RCV;

static volatile enum {
    ADC_STATE_IDLE = 0,
    ADC_STATE_CONV = 1,
    ADC_STATE_RDY = 2,
    ADC_STATE_ERROR = 3,
} adc_state = ADC_STATE_IDLE;

static void
conv_sg()

{
}

static void crc_check()
{
    uint16_t crc = crc_calc(rx_buf.bytes, CMD_LEN);
    if (rx_buf.crc != crc) {
        uart_state = UART_STATE_ERROR;
    }
    LL_CRC_SetInitialData(CRC, 0xFFFF);
}

static void dis_echo()
{
    uart_dma_send(&rx_buf, CMD_LEN + 2);
    uart_state = UART_STATE_RCV;
}

void dis_init()
{
    uart_timeout_config();

    adc_dma_config(&adc_data, SG_CHUNK_BYTE_LEN);
    uart_dma_tx_config(&tx_buf, CMD_LEN + 2);
    uart_dma_rx_config(&rx_buf, CMD_LEN + 2);

    adc_calibration();
    adc_en();

    LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0);
}

void dis_work()
{
    switch (adc_state)
    {
    case ADC_STATE_RDY:
        array_i16_to_cmplxf32(adc_data, fft, SG_LEN);
        cmplx_fft_calc(fft);
        arm_cmplx_mag_f32((float32_t *)fft, fft_mag_sq, SG_HALF_LEN);
        adc_state = ADC_STATE_IDLE;
        uart_state = UART_STATE_SEND_FFT;
        uart_dma_send(&tx_buf.cmd, CMD_LEN);
        break;
    default:
        break;
    }
}

void cmd_work()
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
        adc_state = ADC_STATE_CONV;
        adc_dma_start(&adc_data, SG_CHUNK_BYTE_LEN);
        tim_on();
    } break;
    case CMD_SEND_RES: {
        // TODO
    } break;
    case CMD_SEND_FFT: {
        uart_state = UART_STATE_SEND_FFT;
        uart_dma_send(&tx_buf.cmd, CMD_LEN);
        crc_calc((uint8_t *)&cmd, CMD_LEN);
    } break;
    case CMD_SEND_SG: {
        uart_state = UART_STATE_SEND_SG;
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
    switch (uart_state) {
    case UART_STATE_ERROR: {
        uart_state = UART_STATE_RCV;
        break;
    }
    case UART_STATE_SEND_FFT: {
        tx_buf.crc = crc_calc((uint8_t *)&fft[SG_CHUNK_WORD_LEN * cnt], SG_CHUNK_BYTE_LEN);
        uart_dma_send(&fft[SG_CHUNK_WORD_LEN * (cnt++)], SG_CHUNK_BYTE_LEN);
        if (cnt == FFT_WORD_CHUNKS) {
            uart_state = UART_STATE_SEND_CRC;
        };
        break;
    }
    case UART_STATE_SEND_SG: {
        tx_buf.crc = crc_calc((uint8_t *)&adc_data[SG_CHUNK_HALFWORD_LEN * cnt], SG_CHUNK_BYTE_LEN);
        uart_dma_send(&adc_data[SG_CHUNK_HALFWORD_LEN * (cnt++)], SG_CHUNK_BYTE_LEN);
        if (cnt == SG_HALFWORD_CHUNKS) {
            uart_state = UART_STATE_SEND_CRC;
        };
        break;
    }
    case UART_STATE_SEND_CRC: {
        uart_dma_send(&tx_buf.crc, 2);
        LL_CRC_SetInitialData(CRC, 0xFFFF);
        cnt = 0;
        uart_state = UART_STATE_RCV;
        break;
    }
    case UART_STATE_ECHO: {
        uart_state = UART_STATE_RCV;
        break;
    }
    default: {
        uart_state = UART_STATE_RCV;
        break;
    }
    }
}

void uart_recv_dma_callback()
{
    switch (uart_state) {
    case UART_STATE_ERROR: {
        break;
    }
    case UART_STATE_RCV: {
        cmd_work();
        break;
    }
    default: {
        break;
    }
    }
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);
}

void EXTI0_IRQHandler(void)
{
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET && LL_EXTI_IsEnabledIT_0_31(LL_EXTI_LINE_0)) {
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
        tx_buf.cmd = CMD_SEND_SG;
        crc_calc((uint8_t *)&tx_buf.cmd, CMD_LEN);
        adc_dma_start(&adc_data, SG_CHUNK_BYTE_LEN);
        adc_state = ADC_STATE_CONV;
        tim_on();
        LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_0);
    }
}

void adc_dma_callback()
{
    static int cnt = 0;
    cnt++;
    if (cnt < SG_ADC_CHUNKS) {
        adc_dma_start(&adc_data[SG_CHUNK_BYTE_LEN * cnt], SG_CHUNK_BYTE_LEN);
    } else {
        tim_off();
        adc_state = ADC_STATE_RDY;
        // uart_state = UART_STATE_SEND_SG;
        // uart_dma_send(&tx_buf.cmd, CMD_LEN);
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
        adc_state = ADC_STATE_ERROR;
    }
}
