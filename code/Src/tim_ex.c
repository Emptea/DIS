#include "tim_ex.h"

void tim_on()
{
    LL_TIM_EnableCounter(TIM3);
}

void tim_off()
{
    LL_TIM_DisableCounter(TIM3);
    LL_TIM_GenerateEvent_UPDATE(TIM3);
}