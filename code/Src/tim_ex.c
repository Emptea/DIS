#include "tim_ex.h"

#define TIM_DLY_MAX 1000

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

static uint32_t tim_dly_is_valid(uint32_t ms)
{
    return (ms <= TIM_DLY_MAX);
}

uint32_t tim_dly_set(uint32_t ms)
{
    if (!tim_dly_is_valid(ms)) {
        return 1;
    }
    LL_TIM_SetAutoReload(TIM1, 10 * ms);
    return 0;
}