#ifndef __ADC_EX_H__
#define __ADC_EX_H__
#include "stm32h7xx_ll_adc.h"
#include "stm32h7xx_ll_dma.h"

#define RE_SG_LEN 1024//64000

extern int16_t adc_buf[RE_SG_LEN];

void adc_calibration();
void adc_en();
void adc_dma_config();
#endif