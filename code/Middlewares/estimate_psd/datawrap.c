/*
 * File: datawrap.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 18-Sep-2024 13:56:03
 */

/* Include Files */
#include "datawrap.h"
#include "estimate_psd_emxutil.h"
#include "estimate_psd_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const emxArray_real32_T *x
 *                double nfft
 *                emxArray_real32_T *y
 * Return Type  : void
 */
void datawrap(const emxArray_real32_T *x, unsigned int nfft, emxArray_real32_T *y)
{
  const float *x_data;
  float *y_data;
  int b_remainder;
  int i;
  int k;
  int ny_tmp_tmp_tmp;
  int offset;
  x_data = x->data;
  ny_tmp_tmp_tmp = nfft;
  i = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = nfft;
  emxEnsureCapacity_real32_T(y, i);
  y_data = y->data;
  for (i = 0; i < ny_tmp_tmp_tmp; i++) {
    y_data[i] = 0.0F;
  }
  if (nfft == 0U) {
    i = MAX_int32_T;
  } else {
    i = (int)((unsigned int)x->size[1] / nfft);
  }
  offset = i * (int)nfft;
  b_remainder = (x->size[1] - offset) - 1;
  for (k = 0; k <= b_remainder; k++) {
    y_data[k] = x_data[offset + k];
  }
  offset = b_remainder + 2;
  for (k = offset; k <= ny_tmp_tmp_tmp; k++) {
    y_data[k - 1] = 0.0F;
  }
  for (b_remainder = 0; b_remainder < i; b_remainder++) {
    offset = b_remainder * nfft;
    for (k = 0; k < ny_tmp_tmp_tmp; k++) {
      y_data[k] += x_data[offset + k];
    }
  }
}

/*
 * File trailer for datawrap.c
 *
 * [EOF]
 */
