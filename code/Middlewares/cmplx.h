#ifndef __CMPLX_H
#define __CMPLX_H

#include <arm_math_types.h>
#include <arm_const_structs.h>
#include <arm_const_structs_extra.h>
#include "extra_ffts.h"
#include "complex.h"

typedef float complex cmplx64_t;

inline static void cmplx_fft_calc(cmplx64_t *data)
{
arm_cfft_f32_extra(&arm_cfft_sR_f32_len16384_extra, (float32_t *)data, 0, 1);
data[0] = 0;
}


#endif
