#ifndef __CMPLX_H
#define __CMPLX_H

#include <arm_math_types.h>
#include <arm_const_structs.h>
#include <arm_const_structs_extra.h>
#include "extra_ffts.h"
#include "complex.h"

typedef float complex cmplx64_t;

inline static void cmplx_fft_calc(cmplx64_t *data, uint32_t len)
{
    switch (len) {
    case 1024: {
        arm_cfft_f32(&arm_cfft_sR_f32_len1024, (float32_t *)data, 0, 1);
    } break;
    case 2048: {
        arm_cfft_f32(&arm_cfft_sR_f32_len2048, (float32_t *)data, 0, 1);
    } break;
    case 4096: {
        arm_cfft_f32(&arm_cfft_sR_f32_len4096, (float32_t *)data, 0, 1);
    } break;
    case 8192: {
        arm_cfft_f32_extra(&arm_cfft_sR_f32_len8192_extra, (float32_t *)data, 0, 1);
    } break;
    case 16384: {
        arm_cfft_f32_extra(&arm_cfft_sR_f32_len16384_extra, (float32_t *)data, 0, 1);
    } break;
    case 32768: {
        arm_cfft_f32_extra(&arm_cfft_sR_f32_len32768_extra, (float32_t *)data, 0, 1);
    } break;
    case 65536: {
        arm_cfft_f32_extra(&arm_cfft_sR_f32_len65536_extra, (float32_t *)data, 0, 1);
    } break;
    default:
        break;
    }

    data[0] = 0;
}

#endif
