#ifndef __ADC_EX_H__
#define __ADC_EX_H__
#include "stm32h7xx_ll_adc.h"
#include "stm32h7xx_ll_dma.h"

#define ADC_RES LL_ADC_RESOLUTION_16B

void adc_calibration();
void adc_en();
void adc_dma_config(void *buf, uint32_t size);
void adc_dma_start(void *buf, uint32_t size);
void adc_dma_callback();

uint32_t adc_start_conv(void *buf, uint32_t len);
uint32_t adc_is_rdy();
void adc_to_idle();
#endif