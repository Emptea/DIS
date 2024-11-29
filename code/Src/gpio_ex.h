#ifndef __GPIO_EX_H__
#define __GPIO_EX_H__

#include "stm32h7xx.h"
#include "stm32h7xx_ll_gpio.h"
#include "stm32h7xx_ll_exti.h"

#define PWR_Pin  LL_GPIO_PIN_8
#define PWR_GPIO GPIOA

enum {
    ARG_OFF = 0x00000000,
    ARG_ON = 0x000000FF,
};

inline static uint32_t pwr_on_off(uint32_t on_off)
{
    if (on_off == ARG_ON) {
        LL_GPIO_ResetOutputPin(PWR_GPIO, PWR_Pin);
    } else if (on_off == ARG_OFF) {
        LL_GPIO_SetOutputPin(PWR_GPIO, PWR_Pin);
    } else {
        return 2;
    }
    return 0;
}

inline static uint32_t exti_on_off(uint32_t on_off)
{
    if (on_off == ARG_ON) {
        LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);
    } else if (on_off == ARG_OFF) {
        LL_EXTI_DisableIT_0_31(LL_EXTI_LINE_0);
    } else {
        return 2;
    }
    return 0;
}

#endif /*__GPIO_EX_H__*/
