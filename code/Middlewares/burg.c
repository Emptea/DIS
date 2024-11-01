#include "array.h"
#include "burg.h"

#define ORD 64
uint32_t var = 0;
cmplx64_t f[SG_LEN] = {0};
cmplx64_t b[SG_LEN] = {0};

void burg(const cmplx64_t *x, cmplx64_t *res, const uint32_t len)
{
    array_cmplxf32_to_cmplxf32(x, f, len);
    array_cmplxf32_to_cmplxf32(x, b, len);
    
    // init Ak & A0
    cmplx64_t Ak[ORD] = {0.0};
    Ak[0] = (cmplx64_t)1.0;

    cmplx64_t Dk = {0.0};
    for (uint32_t j = 0; j < len; j++) {
        Dk += (cmplx64_t)2.0 * x[j] * x[j]; // D0
    }
    Dk -= x[0] * x[0] + x[len] * x[len];

    // burg recursion
    for (uint32_t k = 0; k < ORD; k++) {
        cmplx64_t mu = 0.0;
        for (uint32_t i = 0; i < len - ORD; i++) {
            mu += f[i + k + 1] * b[i];
        }
        mu = -(cmplx64_t)2.0 * mu / Dk;

        for (uint32_t i = 0; i < len - ORD; i++) {
            cmplx64_t Ak_i = Ak[i];
            Ak[i] = Ak_i + mu * Ak[k + 1 - i];
            Ak[k + 1 - i] = Ak[k + 1 - i] + mu * Ak_i;
        }

        for (uint32_t i = 0; i < len - ORD; i++) {
            cmplx64_t b_i = b[i];
            b[i] = b_i + mu * f[i + k + 1];
            f[i + k + 1] = f[i + k + 1] + mu * b_i;
        }
        cmplx64_t f_sq = f[k + 1] * f[k + 1];
        cmplx64_t b_sq = b[len - k - 1] * b[len - k - 1];
        Dk = ((cmplx64_t)1.0 - mu * mu) * Dk -  f_sq - b_sq;
    }

//    for (uint32_t i = ORD; i < len; i++) {
//        res[i] = (cmplx64_t) 0.0;
//        for (uint32_t j = 0; j < ORD; j++) {
//            res[i] -= Ak[j] * x[i - 1 - j];
//        }
//    }
}