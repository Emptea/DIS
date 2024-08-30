#ifndef __PROTOCOL_EX_H__
#define __PROTOCOL_EX_H__
#include "stm32h7xx.h"

#define RE_SG_LEN 1024//64000

extern int16_t sg[RE_SG_LEN];
void cmd_work(uint8_t cmd);
#endif