#include "array.h"
#include "cmplx.h"

void array_cmplx_sum4(
    const cmplx64_t *arr0,
    const cmplx64_t *arr1,
    const cmplx64_t *arr2,
    const cmplx64_t *arr3,
    cmplx64_t *res,
    uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        res[i] = arr0[i] + arr1[i] + arr2[i] + arr3[i];
    }
}

void array_sum4(
    const float32_t *arr0,
    const float32_t *arr1,
    const float32_t *arr2,
    const float32_t *arr3,
    float32_t *res,
    uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        res[i] = arr0[i] + arr1[i] + arr2[i] + arr3[i];
    }
}

float32_t array_sum(float32_t *arr, uint32_t len)
{
    float32_t sum = 0;
    for (uint32_t i = 0; i < len; i++) {
        sum += arr[i];
    }
    return sum;
}

// циклическая сумма диапазона из массива, последний индекс НЕ включается
// len - длина массива, должна быть 2^n
float32_t array_circ_sum_span(const float32_t *arr, uint32_t len, struct span span)
{
    float32_t sum = 0;
    for (int32_t i = span.start; i < span.end; i++) {
        int32_t idx = i & (len - 1);
        sum += arr[idx];
    }
    return sum;
}

// сумма диапазона из массива, последний индекс НЕ включается
// диапазон должен выходить в массив
float32_t array_sum_span(const float32_t *arr, struct span span)
{
    float32_t sum = 0;
    for (int32_t i = span.start; i < span.end; i++) {
        sum += arr[i];
    }
    return sum;
}

void array_mult_cmplx_coeff(cmplx64_t *arr, cmplx64_t coeff, uint32_t len)
{
    for (uint32_t i = 0; i < len; i++) {
        arr[i] *= coeff;
    }
}

void array_clear_u32(void *arr, uint32_t size)
{
    uint32_t *p = arr;
    for (uint32_t i = 0; i < size / 4; i++) {
        *p++ = 0;
    }
}
