/*
 * File: estimate_psd_types.h
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 18-Sep-2024 13:56:03
 */

#ifndef ESTIMATE_PSD_TYPES_H
#define ESTIMATE_PSD_TYPES_H

/* Include Files */
#include "rtwtypes.h"

/* Type Definitions */
#ifndef struct_emxArray_real32_T
#define struct_emxArray_real32_T
struct emxArray_real32_T {
  float *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  bool canFreeData;
};
#endif /* struct_emxArray_real32_T */
#ifndef typedef_emxArray_real32_T
#define typedef_emxArray_real32_T
typedef struct emxArray_real32_T emxArray_real32_T;
#endif /* typedef_emxArray_real32_T */

#ifndef typedef_emxArray_creal32_T
#define typedef_emxArray_creal32_T
typedef struct {
  creal32_T *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  bool canFreeData;
} emxArray_creal32_T;
#endif /* typedef_emxArray_creal32_T */

#ifndef struct_emxArray_real_T
#define struct_emxArray_real_T
struct emxArray_real_T {
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  bool canFreeData;
};
#endif /* struct_emxArray_real_T */
#ifndef typedef_emxArray_real_T
#define typedef_emxArray_real_T
typedef struct emxArray_real_T emxArray_real_T;
#endif /* typedef_emxArray_real_T */

#ifndef struct_emxArray_int32_T
#define struct_emxArray_int32_T
struct emxArray_int32_T {
  int *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  bool canFreeData;
};
#endif /* struct_emxArray_int32_T */
#ifndef typedef_emxArray_int32_T
#define typedef_emxArray_int32_T
typedef struct emxArray_int32_T emxArray_int32_T;
#endif /* typedef_emxArray_int32_T */

#endif
/*
 * File trailer for estimate_psd_types.h
 *
 * [EOF]
 */
