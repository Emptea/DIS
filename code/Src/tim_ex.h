#ifndef _TIM_EX_H__
#define _TIM_EX_H__
#include "stm32h7xx_ll_tim.h"
#include "stm32h7xx_ll_dma.h"

void tim_adc_on();
void tim_adc_off();
void tim_dly_on();
uint32_t tim_dly_set(uint32_t ms);

inline static uint32_t tim_dly_done()
{
    return LL_TIM_IsActiveFlag_UPDATE(TIM1);
}

inline static void tim_dly_reset()
{
    return LL_TIM_ClearFlag_UPDATE(TIM1);
}
#endif