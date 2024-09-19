/*
 * File: freqz.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 18-Sep-2024 13:56:03
 */

#ifndef FREQZ_H
#define FREQZ_H

/* Include Files */
#include "estimate_psd_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void binary_expand_op(emxArray_creal32_T *in1, const emxArray_creal32_T *in2,
                      const emxArray_creal32_T *in3);

void firfreqz(float bin, double npoints, emxArray_creal32_T *h);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for freqz.h
 *
 * [EOF]
 */
