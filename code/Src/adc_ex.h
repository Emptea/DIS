#ifndef __ADC_EX_H__
#define __ADC_EX_H__
#include "stm32h7xx_ll_adc.h"
#include "stm32h7xx_ll_dma.h"
#include "protocol.h"

void adc_calibration();
void adc_en();
void adc_dma_config();
void adc_start();
#endif