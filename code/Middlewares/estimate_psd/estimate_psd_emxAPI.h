/*
 * File: estimate_psd_emxAPI.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 18-Sep-2024 14:52:21
 */

#ifndef ESTIMATE_PSD_EMXAPI_H
#define ESTIMATE_PSD_EMXAPI_H

/* Include Files */
#include "estimate_psd_types.h"
#include "rtwtypes.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
extern emxArray_real32_T *emxCreateND_real32_T(int numDimensions,
                                               const int *size);

extern emxArray_real32_T *
emxCreateWrapperND_real32_T(float *data, int numDimensions, const int *size);

extern emxArray_real32_T *emxCreateWrapper_real32_T(float *data, int rows,
                                                    int cols);

extern emxArray_real32_T *emxCreate_real32_T(int rows, int cols);

extern void emxDestroyArray_real32_T(emxArray_real32_T *emxArray);

extern void emxInitArray_real32_T(emxArray_real32_T **pEmxArray,
                                  int numDimensions);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for estimate_psd_emxAPI.h
 *
 * [EOF]
 */
