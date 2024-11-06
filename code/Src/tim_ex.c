#include "tim_ex.h"

void tim_adc_on()
{
    LL_TIM_EnableCounter(TIM3);
}

void tim_adc_off()
{
    LL_TIM_DisableCounter(TIM3);
    LL_TIM_SetCounter(TIM3, 0);
}

void tim_dly_on()
{
    LL_TIM_EnableCounter(TIM1);
}

void tim_dly_set(uint32_t ms)
{
    LL_TIM_SetAutoReload(TIM1, 10*ms);
}