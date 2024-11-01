#ifndef __ARRAY_H
#define __ARRAY_H

#include "cmplx.h"

struct span {
    int32_t start;
    int32_t end;
};

void array_cmplx_sum2(
    const float32_t *arr0,
    const float32_t *arr1,
    float32_t *res,
    uint32_t len);
void array_cmplx_sum4(
    const cmplx64_t *arr0,
    const cmplx64_t *arr1,
    const cmplx64_t *arr2,
    const cmplx64_t *arr3,
    cmplx64_t *res,
    uint32_t len);
void array_sum4(
    const float32_t *arr0,
    const float32_t *arr1,
    const float32_t *arr2,
    const float32_t *arr3,
    float32_t *res,
    uint32_t len);

float32_t array_sum(float32_t *arr, uint32_t len);

inline static void array_i16_to_cmplx_arrf32(int16_t *src, float32_t *dst, uint32_t len)
{
    for (uint32_t tmp_index = 0; tmp_index < len; tmp_index++) {
        dst[2 * tmp_index] = (float32_t)src[tmp_index]; //(float32_t)65536.0;
        dst[2 * tmp_index + 1] = 0;
    }
}

inline static void array_ui16_to_cmplxf32(uint16_t *src, cmplx64_t *dst, uint32_t len)
{
    do {
        *dst++ = *src++;
    } while (--len);
}

inline static void array_i16_to_cmplxf32(int16_t *src, cmplx64_t *dst, uint32_t len)
{
    do {
        *dst++ = *src++;
    } while (--len);
}

inline static void array_cpy_cmplx64(
    const void *src,
    void *dst,
    uint32_t size)
{
//    const cmplx64_t *psrc = src;
//    cmplx64_t *pdst = dst;
//    size = size / 8;
//    do {
//        *pdst++ = *psrc++;
//    } while (--size);
    
    const uint32_t *psrc = src;
    uint32_t *pdst = dst;
    size = size / 4;
    do {
        *pdst++ = *psrc++;
    } while (--size);
}

inline static void array_cpy_u32(
    const void *src,
    void *dst,
    uint32_t size)
{
    const uint32_t *psrc = src;
    uint32_t *pdst = dst;
    size /= 4;
    do {
        *pdst++ = *psrc++;
    } while (--size);
}

inline static void array_cpy_u16(
    const void *src,
    void *dst,
    uint32_t size)
{
    const uint16_t *psrc = src;
    uint16_t *pdst = dst;
    size /= 2;
    do {
        *pdst++ = *psrc++;
    } while (--size);
}

void array_mult_cmplx_coeff(cmplx64_t *arr, cmplx64_t coeff, uint32_t len);
void array_clear_u32(void *arr, uint32_t size);
float32_t array_circ_sum_span(const float32_t *arr, uint32_t len, struct span span);
float32_t array_sum_span(const float32_t *arr, struct span span);

#endif
