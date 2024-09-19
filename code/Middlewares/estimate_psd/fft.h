/*
 * File: fft.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 18-Sep-2024 13:56:03
 */

#ifndef FFT_H
#define FFT_H

/* Include Files */
#include "estimate_psd_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void b_fft(const emxArray_real32_T *x, double varargin_1,
           emxArray_creal32_T *y);

void fft(const emxArray_real32_T *x, emxArray_creal32_T *y);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for fft.h
 *
 * [EOF]
 */
