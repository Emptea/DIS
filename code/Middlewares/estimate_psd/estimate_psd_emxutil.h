/*
 * File: estimate_psd_emxutil.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 18-Sep-2024 13:56:03
 */

#ifndef ESTIMATE_PSD_EMXUTIL_H
#define ESTIMATE_PSD_EMXUTIL_H

/* Include Files */
#include "estimate_psd_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern void emxEnsureCapacity_creal32_T(emxArray_creal32_T *emxArray,
                                        int oldNumel);

extern void emxEnsureCapacity_int32_T(emxArray_int32_T *emxArray, int oldNumel);

extern void emxEnsureCapacity_real32_T(emxArray_real32_T *emxArray,
                                       int oldNumel);

extern void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel);

extern void emxFree_creal32_T(emxArray_creal32_T **pEmxArray);

extern void emxFree_int32_T(emxArray_int32_T **pEmxArray);

extern void emxFree_real32_T(emxArray_real32_T **pEmxArray);

extern void emxFree_real_T(emxArray_real_T **pEmxArray);

extern void emxInit_creal32_T(emxArray_creal32_T **pEmxArray,
                              int numDimensions);

extern void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions);

extern void emxInit_real32_T(emxArray_real32_T **pEmxArray, int numDimensions);

extern void emxInit_real_T(emxArray_real_T **pEmxArray);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for estimate_psd_emxutil.h
 *
 * [EOF]
 */
