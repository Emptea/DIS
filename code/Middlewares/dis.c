#include "dis.h"
#include "usart_ex.h"
#include "adc_ex.h"
#include "gpio_ex.h"
#include "tim_ex.h"
#include "crc_ex.h"
#include "stm32h7xx_ll_exti.h"
#include "cmplx.h"
#include "arm_math.h"
#include "array.h"
#include "burg.h"

// #define BURG

#define ALIGN_32BYTES __attribute__((aligned(32)))

#define cmd2uint(char1, char2, char3, char4) \
    ((char1 << 24) + (char2 << 16) + (char3 << 8) + char4)

#define TIM_DLY_MAX           1000

#define SG_LEN_MIN            1024
#define SG_LEN_MAX            32768

#define FFT_LEN_MIN           1024
#define FFT_LEN_MAX           32768

#define CMD_LEN               4
#define ARG_LEN               4
#define RES_LEN               4

#define SG_CHUNK_BYTE_LEN_MAX UINT16_MAX

#define FS                    2000000.0f
#define F_STEP                (FS / ((float32_t)SG_LEN))
#define F0                    56710000000.0f
#define LAMBDA                (299792458.0f / F0)

enum cmd {
    CMD_PWR_ON = cmd2uint('c', 'm', 'd', '0'),
    CMD_PWR_OFF = cmd2uint('c', 'm', 'd', '1'),
    CMD_EXTI_ON = cmd2uint('c', 'm', 'd', '2'),
    CMD_SET_TIM = cmd2uint('c', 'm', 'd', '3'),
    CMD_SET_SG_LEN = cmd2uint('c', 'm', 'd', '4'),
    CMD_SET_FFT_LEN = cmd2uint('c', 'm', 'd', '5'),
    CMD_SEND_RES = cmd2uint('c', 'm', 'd', '6'),
    CMD_START_CONV = cmd2uint('c', 'm', 'd', '7'),
    CMD_SEND_FFT = cmd2uint('c', 'm', 'd', '8'),
    CMD_SEND_SG = cmd2uint('c', 'm', 'd', '9'),
    CMD_PING = cmd2uint('p', 'i', 'n', 'g'),
    CMD_NONE,
};
static enum cmd cmd = CMD_NONE;

ALIGN_32BYTES __attribute__((section(".dma.rx_buf"))) union {
    struct {
        uint32_t cmd;
        uint32_t arg;
        uint16_t crc;
    };

    uint8_t bytes[CMD_LEN + ARG_LEN + 2];
} rx_buf;

ALIGN_32BYTES __attribute__((section(".dma.tx_buf"))) struct {
    uint32_t cmd;
    float32_t v_max;
    uint16_t crc;
} tx_buf;

ALIGN_32BYTES __attribute__((section(".dma.adc_data"))) struct {
    uint16_t sg[SG_LEN_MAX];
    uint32_t len;
    uint32_t cnt2send;
} adc_data = {.sg = {0}, .len = SG_LEN_MAX};

static struct {
    cmplx64_t x[FFT_LEN_MAX];
    uint32_t len;
    uint32_t cnt2send;
} fft = {.x = {0}, .len = FFT_LEN_MAX};

ALIGN_32BYTES __attribute__((section(".res"))) static float32_t fft_mag_sq[FFT_LEN_MAX / 2] = {0};

#ifdef BURG
static cmplx64_t pxx[SG_LEN_MAX] = {0};
#endif

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

static void fft_dopp_calc()
{
    uint32_t halflen = adc_data.len / 2;
    array_ui16_to_cmplxf32(adc_data.sg, fft.x, adc_data.len);
#ifdef BURG
    burg(fft.x, pxx, SG_LEN);
    arm_cmplx_mag_f32((float32_t *)pxx, fft_mag_sq, halflen);
#else
    cmplx_fft_calc(fft.x, adc_data.len);
    arm_cmplx_mag_f32((float32_t *)fft.x, fft_mag_sq, halflen);
#endif
    float32_t fft_max = 0.0f;
    uint32_t i_max = 0;
    arm_max_f32(fft_mag_sq, halflen, &fft_max, &i_max);
    tx_buf.v_max = F_STEP * ((float32_t)i_max) * LAMBDA / 2;
}

static void crc_check()
{
    uint16_t crc = crc_calc(rx_buf.bytes, CMD_LEN + ARG_LEN);
    if (rx_buf.crc != crc) {
        uart_state = UART_STATE_ERROR;
    }
    LL_CRC_SetInitialData(CRC, 0xFFFF);
}

static void dis_echo()
{
    uart_dma_send(&rx_buf, CMD_LEN + ARG_LEN + 2);
    uart_state = UART_STATE_RCV;
}

void dis_init()
{
    uart_timeout_config();

    adc_dma_config(&adc_data, adc_data.len);
    uart_dma_tx_config(&tx_buf, CMD_LEN + RES_LEN + 2);
    uart_dma_rx_config(&rx_buf, CMD_LEN + ARG_LEN + 2);

    adc_calibration();
    adc_en();

    LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0);
}

void dis_work()
{
    switch (adc_state) {
    case ADC_STATE_RDY:
        fft_dopp_calc();
        adc_state = ADC_STATE_IDLE;
        tx_buf.cmd = CMD_SEND_RES;
        uart_state = UART_STATE_SEND_CRC;
        uart_dma_send(&tx_buf.cmd, CMD_LEN + RES_LEN);
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
    case CMD_SET_TIM: {
        if (rx_buf.arg < TIM_DLY_MAX) {
            tim_dly_set(rx_buf.arg);
        } else {
            rx_buf.arg = 0xFF;
        }
        dis_echo();
    } break;
    case CMD_SET_SG_LEN: {
        if (rx_buf.arg > SG_LEN_MIN && rx_buf.arg < SG_LEN_MAX) {
            adc_data.len = rx_buf.arg;
        } else {
            rx_buf.arg = 0xFF;
        }
        dis_echo();
    } break;
    case CMD_SET_FFT_LEN: {
        if (rx_buf.arg > FFT_LEN_MIN && rx_buf.arg < FFT_LEN_MAX) {
            fft.len = rx_buf.arg;
        } else {
            rx_buf.arg = 0xFF;
        }
        dis_echo();
    } break;
    case CMD_START_CONV: {
        adc_state = ADC_STATE_CONV;
        adc_dma_start(&adc_data, adc_data.len);
        tim_dly_on();
    } break;
    case CMD_SEND_RES: {
        uart_state = UART_STATE_SEND_CRC;
        uart_dma_send(&tx_buf.cmd, CMD_LEN + RES_LEN);
        crc_calc((uint8_t *)&cmd, CMD_LEN + RES_LEN);
    } break;
    case CMD_SEND_FFT: {
        uart_state = UART_STATE_SEND_FFT;
        fft.cnt2send = fft.len * 2;
        uart_dma_send(&tx_buf.cmd, CMD_LEN);
        crc_calc((uint8_t *)&cmd, CMD_LEN);
    } break;
    case CMD_SEND_SG: {
        uart_state = UART_STATE_SEND_SG;
        adc_data.cnt2send = adc_data.len * 2;
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
    static uint32_t idx = 0;
    switch (uart_state) {
    case UART_STATE_ERROR: {
        uart_state = UART_STATE_RCV;
    } break;
    case UART_STATE_SEND_FFT: {
        if (fft.cnt2send > SG_CHUNK_BYTE_LEN_MAX) {
            fft.cnt2send -= SG_CHUNK_BYTE_LEN_MAX;
            crc_calc((uint8_t *)&fft.x, SG_CHUNK_BYTE_LEN_MAX);
            uart_dma_send(&fft.x, SG_CHUNK_BYTE_LEN_MAX);
            idx = SG_CHUNK_BYTE_LEN_MAX;
        } else {
            uart_dma_send(&fft.x[idx], fft.cnt2send);
            crc_calc((uint8_t *)&fft.x[idx], fft.cnt2send);
            idx = 0;
            uart_state = UART_STATE_SEND_CRC;
        }
    } break;
    case UART_STATE_SEND_SG: {
        if (adc_data.cnt2send > SG_CHUNK_BYTE_LEN_MAX) {
            adc_data.cnt2send -= SG_CHUNK_BYTE_LEN_MAX;
            crc_calc((uint8_t *)&adc_data, SG_CHUNK_BYTE_LEN_MAX);
            uart_dma_send(&adc_data, SG_CHUNK_BYTE_LEN_MAX);
            idx = SG_CHUNK_BYTE_LEN_MAX;
        } else {
            uart_dma_send(&adc_data.sg[idx], adc_data.cnt2send);
            crc_calc((uint8_t *)&adc_data.sg[idx], adc_data.cnt2send);
            idx = 0;
            uart_state = UART_STATE_SEND_CRC;
        }
    } break;
    case UART_STATE_SEND_CRC: {
        uart_dma_send(&tx_buf.crc, 2);
        LL_CRC_SetInitialData(CRC, 0xFFFF);
        uart_state = UART_STATE_RCV;
    } break;
    case UART_STATE_ECHO: {
        uart_state = UART_STATE_RCV;
    } break;
    default: {
        uart_state = UART_STATE_RCV;
    } break;
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
        adc_dma_start(&adc_data, adc_data.len);
        adc_state = ADC_STATE_CONV;
        tim_dly_on();
        LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_0);
    }
}

void adc_dma_callback()
{
    tim_adc_off();
    if (fft.len > adc_data.len) {
        for (uint32_t i = adc_data.len; i < fft.len; i++) {
            fft.x[i] = 0;
        }
    }
    adc_state = ADC_STATE_RDY;
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
