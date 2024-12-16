#include "adc_ex.h"
#include "tim_ex.h"

static volatile enum {
    ADC_STATE_IDLE = 0,
    ADC_STATE_CONV = 1,
    ADC_STATE_RDY = 2,
    ADC_STATE_ERROR = 3,
} adc_state = ADC_STATE_IDLE;

void adc_calibration()
{
    LL_ADC_StartCalibration(ADC3, LL_ADC_CALIB_OFFSET_LINEARITY, LL_ADC_SINGLE_ENDED);
    while (LL_ADC_IsCalibrationOnGoing(ADC3)) {
        ;
    }
}

void adc_en()
{
    LL_ADC_ClearFlag_ADRDY(ADC3);
    LL_ADC_Enable(ADC3);
    while (!LL_ADC_IsActiveFlag_ADRDY(ADC3)) {
        ;
    }
    LL_ADC_ClearFlag_ADRDY(ADC3);
    LL_ADC_REG_StartConversion(ADC3);
}

void adc_dma_start(void *buf, uint32_t len)
{
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_STREAM_0, (uint32_t)buf);
    LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, len);
    LL_ADC_REG_StartConversion(ADC3);
    LL_DMA_EnableStream(DMA1, LL_DMA_STREAM_0);
}

__WEAK void adc_dma_callback()
{
}

void adc_dma_config(void *buf, uint32_t len)
{
    LL_ADC_REG_SetDataTransferMode(ADC3, LL_ADC_REG_DMA_TRANSFER_LIMITED);
    LL_DMA_ConfigAddresses(
        DMA1,
        LL_DMA_STREAM_0,
        LL_ADC_DMA_GetRegAddr(ADC3, LL_ADC_DMA_REG_REGULAR_DATA),
        (uint32_t)buf,
        LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_0));
    LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, len);

    LL_DMA_ClearFlag_TE0(DMA1);
    LL_DMA_EnableIT_TE(DMA1, LL_DMA_STREAM_0);
    LL_DMA_ClearFlag_TC0(DMA1);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_0);
}

uint32_t adc_start_conv(void *buf, uint32_t len)
{
    adc_state = ADC_STATE_CONV;
    adc_dma_start(buf, len);
    if (!tim_dly_get()) {
        tim_adc_on();
        return 1;
    } else {
        tim_dly_on();
    }
		return 0;
}

uint32_t adc_is_rdy()
{
    return adc_state == ADC_STATE_RDY;
}

void adc_to_idle()
{
    adc_state = ADC_STATE_IDLE;
}

/**
 * @brief This function handles DMA1 stream0 global interrupt (ADC).
 */
void DMA1_Stream0_IRQHandler(void)
{
    if (LL_DMA_IsActiveFlag_TC0(DMA1)) {
        LL_DMA_ClearFlag_TC0(DMA1);
        tim_adc_off();
        adc_state = ADC_STATE_RDY;
    }

    if (LL_DMA_IsActiveFlag_TE0(DMA1)) {
        LL_DMA_ClearFlag_TE0(DMA1);
        adc_state = ADC_STATE_ERROR;
    }
}