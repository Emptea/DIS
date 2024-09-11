#ifndef __PROTOCOL_EX_H__
#define __PROTOCOL_EX_H__
#include "stm32h7xx.h"

#define RE_SG_LEN 1024//64000
#define CMD_LEN 4
#define USART_TO_VAL 8*(5*4)



extern int16_t sg[RE_SG_LEN];
void cmd_work(uint32_t cmd);
#endif