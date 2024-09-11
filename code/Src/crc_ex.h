#ifndef __CRC_EX_H__
#define __CRC_EX_H__

#include "stm32h7xx.h"
#include "stm32h7xx_ll_crc.h"

uint16_t crc_calc(uint8_t *pbuf, uint32_t sz);
#endif