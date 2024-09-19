/*
 * File: FFTImplementationCallback.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 18-Sep-2024 13:56:03
 */

#ifndef FFTIMPLEMENTATIONCALLBACK_H
#define FFTIMPLEMENTATIONCALLBACK_H

/* Include Files */
#include "estimate_psd_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void c_FFTImplementationCallback_dob(const emxArray_real32_T *x, int n2blue,
                                     int nfft, const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     const emxArray_real32_T *sintabinv,
                                     emxArray_creal32_T *y);

void c_FFTImplementationCallback_gen(int nRows, bool useRadix2,
                                     emxArray_real32_T *costab,
                                     emxArray_real32_T *sintab,
                                     emxArray_real32_T *sintabinv);

int c_FFTImplementationCallback_get(int nfft, bool useRadix2, int *nRows);

void c_FFTImplementationCallback_r2b(const emxArray_real32_T *x,
                                     int unsigned_nRows,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     emxArray_creal32_T *y);

void d_FFTImplementationCallback_r2b(const emxArray_creal32_T *x,
                                     int unsigned_nRows,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     emxArray_creal32_T *y);

void e_FFTImplementationCallback_doH(
    const emxArray_real32_T *x, emxArray_creal32_T *y, int nrowsx, int nRows,
    int nfft, const emxArray_creal32_T *wwc, const emxArray_real32_T *costab,
    const emxArray_real32_T *sintab, const emxArray_real32_T *costabinv,
    const emxArray_real32_T *sintabinv);

void f_FFTImplementationCallback_doH(float x, emxArray_creal32_T *y,
                                     int unsigned_nRows,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab);

void g_FFTImplementationCallback_doH(float x, emxArray_creal32_T *y, int nRows,
                                     int nfft, const emxArray_creal32_T *wwc,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     const emxArray_real32_T *costabinv,
                                     const emxArray_real32_T *sintabinv);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for FFTImplementationCallback.h
 *
 * [EOF]
 */
