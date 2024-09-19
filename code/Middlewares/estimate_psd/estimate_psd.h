/*
 * File: estimate_psd.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 18-Sep-2024 13:56:03
 */

#ifndef ESTIMATE_PSD_H
#define ESTIMATE_PSD_H

/* Include Files */
#include "estimate_psd_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void estimate_psd(const emxArray_real32_T *x, unsigned int order,
                         unsigned int nfft, unsigned int fs,
                         emxArray_real32_T *Pxx, emxArray_real32_T *F);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for estimate_psd.h
 *
 * [EOF]
 */
