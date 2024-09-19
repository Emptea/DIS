/*
 * File: fft.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 18-Sep-2024 13:56:03
 */

/* Include Files */
#include "fft.h"
#include "FFTImplementationCallback.h"
#include "estimate_psd_emxutil.h"
#include "estimate_psd_types.h"
#include "rt_nonfinite.h"

/* Function Definitions */
/*
 * Arguments    : const emxArray_real32_T *x
 *                double varargin_1
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void b_fft(const emxArray_real32_T *x, double varargin_1, emxArray_creal32_T *y)
{
  emxArray_creal32_T *yCol;
  emxArray_real32_T b_x;
  emxArray_real32_T *costab;
  emxArray_real32_T *sintab;
  emxArray_real32_T *sintabinv;
  creal32_T *yCol_data;
  creal32_T *y_data;
  int N2blue;
  int c_x;
  int d_x;
  int nRows;
  emxInit_creal32_T(&yCol, 1);
  emxInit_real32_T(&costab, 2);
  emxInit_real32_T(&sintab, 2);
  emxInit_real32_T(&sintabinv, 2);
  if ((x->size[1] == 0) || ((int)varargin_1 == 0)) {
    N2blue = y->size[0] * y->size[1];
    y->size[0] = 1;
    nRows = (int)varargin_1;
    y->size[1] = (int)varargin_1;
    emxEnsureCapacity_creal32_T(y, N2blue);
    y_data = y->data;
    for (N2blue = 0; N2blue < nRows; N2blue++) {
      y_data[N2blue].re = 0.0F;
      y_data[N2blue].im = 0.0F;
    }
  } else {
    bool useRadix2;
    useRadix2 = (((int)varargin_1 > 0) &&
                 (((int)varargin_1 & ((int)varargin_1 - 1)) == 0));
    N2blue =
        c_FFTImplementationCallback_get((int)varargin_1, useRadix2, &nRows);
    c_FFTImplementationCallback_gen(nRows, useRadix2, costab, sintab,
                                    sintabinv);
    if (useRadix2) {
      nRows = x->size[1];
      b_x = *x;
      d_x = nRows;
      b_x.size = &d_x;
      b_x.numDimensions = 1;
      c_FFTImplementationCallback_r2b(&b_x, (int)varargin_1, costab, sintab,
                                      yCol);
      yCol_data = yCol->data;
    } else {
      nRows = x->size[1];
      b_x = *x;
      c_x = nRows;
      b_x.size = &c_x;
      b_x.numDimensions = 1;
      c_FFTImplementationCallback_dob(&b_x, N2blue, (int)varargin_1, costab,
                                      sintab, sintabinv, yCol);
      yCol_data = yCol->data;
    }
    N2blue = y->size[0] * y->size[1];
    y->size[0] = 1;
    nRows = (int)varargin_1;
    y->size[1] = (int)varargin_1;
    emxEnsureCapacity_creal32_T(y, N2blue);
    y_data = y->data;
    for (N2blue = 0; N2blue < nRows; N2blue++) {
      y_data[N2blue] = yCol_data[N2blue];
    }
  }
  emxFree_real32_T(&sintabinv);
  emxFree_real32_T(&sintab);
  emxFree_real32_T(&costab);
  emxFree_creal32_T(&yCol);
}

/*
 * Arguments    : const emxArray_real32_T *x
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void fft(const emxArray_real32_T *x, emxArray_creal32_T *y)
{
  emxArray_creal32_T *yCol;
  emxArray_real32_T b_x;
  emxArray_real32_T *costab;
  emxArray_real32_T *sintab;
  emxArray_real32_T *sintabinv;
  creal32_T *yCol_data;
  creal32_T *y_data;
  int N2blue;
  int c_x;
  int d_x;
  int nRows;
  if (x->size[1] == 0) {
    y->size[0] = 1;
    y->size[1] = 0;
  } else {
    bool useRadix2;
    useRadix2 = ((x->size[1] & (x->size[1] - 1)) == 0);
    N2blue = c_FFTImplementationCallback_get(x->size[1], useRadix2, &nRows);
    emxInit_real32_T(&costab, 2);
    emxInit_real32_T(&sintab, 2);
    emxInit_real32_T(&sintabinv, 2);
    c_FFTImplementationCallback_gen(nRows, useRadix2, costab, sintab,
                                    sintabinv);
    emxInit_creal32_T(&yCol, 1);
    if (useRadix2) {
      nRows = x->size[1];
      b_x = *x;
      d_x = nRows;
      b_x.size = &d_x;
      b_x.numDimensions = 1;
      c_FFTImplementationCallback_r2b(&b_x, x->size[1], costab, sintab, yCol);
      yCol_data = yCol->data;
    } else {
      nRows = x->size[1];
      b_x = *x;
      c_x = nRows;
      b_x.size = &c_x;
      b_x.numDimensions = 1;
      c_FFTImplementationCallback_dob(&b_x, N2blue, x->size[1], costab, sintab,
                                      sintabinv, yCol);
      yCol_data = yCol->data;
    }
    emxFree_real32_T(&sintabinv);
    emxFree_real32_T(&sintab);
    emxFree_real32_T(&costab);
    N2blue = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = x->size[1];
    emxEnsureCapacity_creal32_T(y, N2blue);
    y_data = y->data;
    nRows = x->size[1];
    for (N2blue = 0; N2blue < nRows; N2blue++) {
      y_data[N2blue] = yCol_data[N2blue];
    }
    emxFree_creal32_T(&yCol);
  }
}

/*
 * File trailer for fft.c
 *
 * [EOF]
 */
