#ifndef __BURG_H
#define __BURG_H
#include "cmplx.h"
#define SG_LEN           ((uint32_t)32768 / 8)

void burg(const cmplx64_t *x, cmplx64_t *res, const uint32_t len);

#endif