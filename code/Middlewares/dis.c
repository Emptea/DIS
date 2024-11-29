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

#define SG_LEN_MIN            1024
#define SG_LEN_MAX            32768

#define FFT_LEN_MIN           1024
#define FFT_LEN_MAX           32768

#define CMD_SZ                4
#define ARG_SZ                4
#define RES_SZ                4
#define HEADER_SZ             (CMD_SZ + RES_SZ + 2)

#define SG_CHUNK_BYTE_LEN_MAX UINT16_MAX

#define FS                    2000000.0f
#define F_STEP                (FS / ((float32_t)SG_LEN))
#define F0                    56710000000.0f
#define LAMBDA                (299792458.0f / F0)

enum {
    ERR_NONE = 0,
    ERR_CMD = 1,
    ERR_ARG = 2,
    ERR_NO_RSLT = 3,
    ERR_ARG_TIM = 0x00000102,
    ERR_ARG_SG = 0x00000202,
    ERR_ARG_FFT = 0x00000302,
    ERR_ARG_NBYTES = 0x00000402,
};

enum res_state {
    RES_RDY = 0,
    RES_NONE = 1,
};
static enum res_state res_state = RES_NONE;

enum cmd {
    CMD_PING = cmd2uint('p', 'i', 'n', 'g'),
    CMD_PWR_ON_OFF = cmd2uint('c', 'm', 'd', '0'),
    CMD_EXTI_ON_OFF = cmd2uint('c', 'm', 'd', '1'),
    CMD_SET_TIM = cmd2uint('c', 'm', 'd', '2'),
    CMD_SET_SG_LEN = cmd2uint('c', 'm', 'd', '3'),
    CMD_SET_FFT_LEN = cmd2uint('c', 'm', 'd', '4'),
    CMD_SEND_RES = cmd2uint('c', 'm', 'd', '5'),
    CMD_START_CONV = cmd2uint('c', 'm', 'd', '7'),
    CMD_SEND_SG = cmd2uint('c', 'm', 'd', '8'),
    CMD_SEND_FFT = cmd2uint('c', 'm', 'd', '9'),
};
static enum cmd cmd = CMD_PING;

enum header {
    HEADER_RSLT = cmd2uint('r', 's', 'l', 't'),
    HEADER_PULSE = cmd2uint('p', 'u', 'l', 's'),
    HEADER_SG = cmd2uint('p', 's', 'i', 'g'),
    HEADER_FFT = cmd2uint('p', 'f', 'f', 't'),
    HEADER_ERR = cmd2uint('e', 'r', 'n', 'o'),
};
static enum header header = HEADER_ERR;

ALIGN_32BYTES __attribute__((section(".dma.rx_buf"))) union {
    struct {
        uint32_t cmd;
        uint32_t arg;
        uint16_t crc;
    };

    uint8_t bytes[CMD_SZ + ARG_SZ + 2];
} rx_buf;

ALIGN_32BYTES __attribute__((section(".dma.tx_buf"))) struct tx_buf {
    uint32_t cmd;
    uint32_t err;
    uint16_t header_crc;
    uint16_t data_crc;
} tx_buf;

ALIGN_32BYTES __attribute__((section(".dma.rslt_buf"))) struct {
    uint32_t cmd;
    float32_t v_max;
    uint16_t crc;
} rslt_buf = {.cmd = HEADER_RSLT};

ALIGN_32BYTES __attribute__((section(".dma.adc_data"))) struct {
    union {
        uint16_t data[SG_LEN_MAX];
        uint8_t bytes[SG_LEN_MAX * 2];
    } sg;

    uint32_t len;
    uint32_t cnt2send;
} adc_data = {.sg = {0}, .len = SG_LEN_MAX};

static struct {
    union {
        cmplx64_t cmplx[FFT_LEN_MAX];
        uint8_t bytes[FFT_LEN_MAX * 8];
    } x;

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
    UART_STATE_SEND_DATA_CRC = 2,
    UART_STATE_ECHO = 3,
    UART_STATE_ERROR = 4,
    // debug states
    UART_STATE_SEND_FFT,
    UART_STATE_SEND_SG,
} uart_state = UART_STATE_RCV;

inline static uint32_t res_check()
{
    if (res_state != RES_RDY) {
        return ERR_NO_RSLT;
    } else {
        return ERR_NONE;
    }
}

static void res_send()
{
    rslt_buf.crc = crc_calc((uint8_t *)&rslt_buf, CMD_SZ + ARG_SZ);
    LL_CRC_ResetCRCCalculationUnit(CRC);
    uart_dma_send(&rslt_buf.cmd, HEADER_SZ);
}

static void dopp_calc()
{
    uint32_t halflen = fft.len / 2;
    array_ui16_to_cmplxf32(adc_data.sg.data, fft.x.cmplx, adc_data.len);
    if (fft.len > adc_data.len) {
        for (uint32_t i = adc_data.len; i < fft.len; i++) {
            fft.x.cmplx[i] = 0;
        }
    }
#ifdef BURG
    burg(fft.x, pxx, SG_LEN);
    arm_cmplx_mag_f32((float32_t *)pxx, fft_mag_sq, halflen);
#else
    cmplx_fft_calc(fft.x.cmplx, fft.len);
    arm_cmplx_mag_f32((float32_t *)fft.x.cmplx, fft_mag_sq, halflen);
#endif
    float32_t fft_max = 0.0f;
    uint32_t i_max = 0;
    arm_max_f32(fft_mag_sq, halflen, &fft_max, &i_max);
    rslt_buf.v_max = F_STEP * ((float32_t)i_max) * LAMBDA / 2;
}

static void crc_check()
{
    uint16_t crc = crc_calc(rx_buf.bytes, CMD_SZ + ARG_SZ);
    if (rx_buf.crc != crc) {
        uart_state = UART_STATE_ERROR;
    }
    LL_CRC_ResetCRCCalculationUnit(CRC);
    // LL_CRC_SetInitialData(CRC, 0xFFFF);
}

static void dis_echo()
{
    uart_dma_send(&rx_buf, HEADER_SZ);
    uart_state = UART_STATE_RCV;
}

void dis_init()
{
    pwr_on_off(ARG_OFF);
    uart_timeout_config();

    adc_dma_config(&adc_data, adc_data.len);
    uart_dma_tx_config(&tx_buf, HEADER_SZ);
    uart_dma_rx_config(&rx_buf, HEADER_SZ);

    adc_calibration();
    adc_en();

    LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0);
}

void dis_work()
{
    if (adc_is_rdy()) {
        dopp_calc();
        adc_to_idle();
        res_state = RES_RDY;
        res_send();
    }
}

// move into different file?
inline static uint32_t check_len_boundaries(uint32_t arg, uint32_t min, uint32_t max)
{
    return (arg < min || arg > max);
}

inline static uint32_t err_sg_fft_work(uint32_t err_arg)
{
    uint32_t err = check_len_boundaries(rx_buf.arg, SG_LEN_MIN, adc_data.len);
    if (err) {
        return err_arg;
    } else if (res_state != RES_RDY) {
        return ERR_NO_RSLT;
    }
    return err;
}

inline static uint32_t set_len(uint32_t *len, uint32_t arg, uint32_t min, uint32_t max)
{
    uint32_t err = check_len_boundaries(arg, min, max);
    if (!err) {
        *len = arg;
    }
    return err;
}

inline static uint32_t sg_send(struct tx_buf *buf)
{
    uint32_t err = err_sg_fft_work(ERR_ARG_SG);
    if (!err) {
        uart_state = UART_STATE_SEND_SG;
        adc_data.cnt2send = rx_buf.arg * 2;
        buf->cmd = HEADER_SG;
        err = adc_data.cnt2send;
    }
    return err;
}

inline static uint32_t fft_send(struct tx_buf *buf)
{
    uint32_t err = err_sg_fft_work(ERR_ARG_FFT);
    if (!err) {
        uart_state = UART_STATE_SEND_FFT;
        fft.cnt2send = rx_buf.arg * 8;
        buf->cmd = HEADER_FFT;
        err = fft.cnt2send;
    }
    return err;
}

void cmd_work()
{
    tx_buf.cmd = HEADER_ERR;

    switch (rx_buf.cmd) {
    case CMD_PWR_ON_OFF: {
        tx_buf.err = pwr_on_off(rx_buf.arg);
    } break;
    case CMD_EXTI_ON_OFF: {
        tx_buf.err = exti_on_off(rx_buf.arg);
    } break;
    case CMD_SET_TIM: {
        tx_buf.err = tim_dly_set(rx_buf.arg);
    } break;
    case CMD_SET_SG_LEN: {
        tx_buf.err = set_len(&adc_data.len, rx_buf.arg, SG_LEN_MIN, SG_LEN_MAX);
    } break;
    case CMD_SET_FFT_LEN: {
        tx_buf.err = set_len(&fft.len, rx_buf.arg, FFT_LEN_MIN, FFT_LEN_MAX);
    } break;
    case CMD_SEND_RES: {
        tx_buf.err = res_check();
        if (!tx_buf.err) {
            res_send();
            return;
        }
    } break;
    case CMD_START_CONV: {
        adc_start_conv(&adc_data, adc_data.len);
        tx_buf.err = ERR_NONE;
    } break;
    case CMD_SEND_FFT: {
        tx_buf.err = fft_send(&tx_buf);
    } break;
    case CMD_SEND_SG: {
        tx_buf.err = sg_send(&tx_buf);
    } break;
    case CMD_PING: {
        tx_buf.err = ERR_NONE;
    } break;
    default: {
        tx_buf.err = ERR_CMD;
    } break;
    }

    tx_buf.header_crc = crc_calc((uint8_t *)&tx_buf.cmd, CMD_SZ + RES_SZ);
    uart_dma_send(&tx_buf.cmd, CMD_SZ + RES_SZ + 2);
    LL_CRC_SetInitialData(CRC, 0xFFFF);
    if (uart_state == UART_STATE_RCV) {
        LL_USART_EnableRxTimeout(USART1);
        uart_dma_rcv(&rx_buf, HEADER_SZ);
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
            crc_calc((uint8_t *)&fft.x.bytes[idx], SG_CHUNK_BYTE_LEN_MAX);
            uart_dma_send(&fft.x.bytes[idx], SG_CHUNK_BYTE_LEN_MAX);
            idx += SG_CHUNK_BYTE_LEN_MAX;
        } else {
            uart_dma_send(&fft.x.bytes[idx], fft.cnt2send);
            tx_buf.data_crc = crc_calc((uint8_t *)&fft.x.bytes[idx], fft.cnt2send);
            idx = 0;
            uart_state = UART_STATE_SEND_DATA_CRC;
        }
    } break;
    case UART_STATE_SEND_SG: {
        if (adc_data.cnt2send > SG_CHUNK_BYTE_LEN_MAX) {
            adc_data.cnt2send -= SG_CHUNK_BYTE_LEN_MAX;
            crc_calc((uint8_t *)&adc_data.sg.bytes[idx], SG_CHUNK_BYTE_LEN_MAX);
            uart_dma_send(&adc_data.sg.bytes[idx], SG_CHUNK_BYTE_LEN_MAX);
            idx += SG_CHUNK_BYTE_LEN_MAX;
        } else {
            tx_buf.data_crc = crc_calc((uint8_t *)&adc_data.sg.bytes[idx], adc_data.cnt2send);
            uart_dma_send(&adc_data.sg.bytes[idx], adc_data.cnt2send);
            idx = 0;
            uart_state = UART_STATE_SEND_DATA_CRC;
        }
    } break;
    case UART_STATE_SEND_DATA_CRC: {
        uart_dma_send(&tx_buf.data_crc, 2);
        LL_CRC_SetInitialData(CRC, 0xFFFF);
        LL_USART_EnableRxTimeout(USART1);
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

inline static void check_pack()
{
    uart_state = UART_STATE_ERROR * uart_integrity_check(HEADER_SZ);
    crc_check();
}

static void uart_state_work()
{
    switch (uart_state) {
    case UART_STATE_ERROR: {
        LL_USART_EnableRxTimeout(USART1);
        uart_dma_rcv(&rx_buf, HEADER_SZ);
        uart_state = UART_STATE_RCV;
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
}

void uart_recv_dma_callback()
{
    check_pack();
    uart_state_work();
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_1);
}

void EXTI0_IRQHandler(void)
{
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET && LL_EXTI_IsEnabledIT_0_31(LL_EXTI_LINE_0)) {
        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
        adc_start_conv(&adc_data, adc_data.len);
        LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_0);
    }
}
