#include "adc_ex.h"

void adc_calibration()
{
    //TODO
}

void adc_en()
{
    LL_ADC_ClearFlag_ADRDY(ADC3);
    LL_ADC_Enable(ADC3);
    while(!LL_ADC_IsActiveFlag_ADRDY(ADC3))
    {
        ;
    }
    LL_ADC_ClearFlag_ADRDY(ADC3);
}

void adc_dma_config()
{
    LL_DMA_ConfigAddresses(
			DMA1,
			LL_DMA_STREAM_0,
            LL_ADC_DMA_GetRegAddr(ADC3 ,LL_ADC_DMA_REG_REGULAR_DATA),
			(uint32_t)adc_buf,
			LL_DMA_GetDataTransferDirection(DMA1, LL_DMA_STREAM_0));
	LL_DMA_SetDataLength(DMA1, LL_DMA_STREAM_0, RE_SG_LEN);
	
	LL_DMA_ClearFlag_TC0(DMA1);
	LL_DMA_EnableIT_TC(DMA1, LL_DMA_STREAM_0);	
}