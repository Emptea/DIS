#ifndef __GPIO_EX_H__
#define __GPIO_EX_H__

#include "stm32h7xx.h"
#include "stm32h7xx_ll_gpio.h"

#define PWR_Pin  LL_GPIO_PIN_8
#define PWR_GPIO GPIOA

inline static void pwr_off()
{
    LL_GPIO_SetOutputPin(PWR_GPIO, PWR_Pin);
}

inline static void pwr_on()
{
    LL_GPIO_ResetOutputPin(PWR_GPIO, PWR_Pin);
}
#endif
