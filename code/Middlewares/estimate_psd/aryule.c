/*
 * File: aryule.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 18-Sep-2024 13:56:03
 */

/* Include Files */
#include "aryule.h"
#include "FFTImplementationCallback.h"
#include "estimate_psd_emxutil.h"
#include "estimate_psd_rtwutil.h"
#include "estimate_psd_types.h"
#include "ifft.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static double rt_powd_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_powd_snf(double u0, double u1)
{
  double y;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = rtNaN;
  } else {
    double d;
    double d1;
    d = fabs(u0);
    d1 = fabs(u1);
    if (rtIsInf(u1)) {
      if (d == 1.0) {
        y = 1.0;
      } else if (d > 1.0) {
        if (u1 > 0.0) {
          y = rtInf;
        } else {
          y = 0.0;
        }
      } else if (u1 > 0.0) {
        y = 0.0;
      } else {
        y = rtInf;
      }
    } else if (d1 == 0.0) {
      y = 1.0;
    } else if (d1 == 1.0) {
      if (u1 > 0.0) {
        y = u0;
      } else {
        y = 1.0 / u0;
      }
    } else if (u1 == 2.0) {
      y = u0 * u0;
    } else if ((u1 == 0.5) && (u0 >= 0.0)) {
      y = sqrt(u0);
    } else if ((u0 < 0.0) && (u1 > floor(u1))) {
      y = rtNaN;
    } else {
      y = pow(u0, u1);
    }
  }
  return y;
}

/*
 * Arguments    : const emxArray_real32_T *x
 *                unsigned int p
 *                emxArray_real32_T *a
 * Return Type  : float
 */
float aryule(const emxArray_real32_T *x, unsigned int p, emxArray_real32_T *a)
{
  emxArray_creal32_T *X;
  emxArray_int32_T *y;
  emxArray_real32_T *b_c1;
  emxArray_real32_T *c1;
  emxArray_real32_T *sintabinv;
  emxArray_real32_T *temp_a;
  emxArray_real32_T *temp_auf;
  emxArray_real32_T *xm;
  creal32_T *X_data;
  double f_tmp;
  double m2;
  const float *x_data;
  float e;
  float s;
  float *a_data;
  float *b_c1_data;
  float *c1_data;
  float *temp_auf_data;
  float *xm_data;
  int ceilLog2;
  int i;
  int k;
  int mxl;
  int nx;
  int *y_data;
  x_data = x->data;
  emxInit_real32_T(&xm, 1);
  if (x->size[0] == 1) {
    i = xm->size[0];
    xm->size[0] = 1;
    emxEnsureCapacity_real32_T(xm, i);
    xm_data = xm->data;
    xm_data[0] = x_data[0];
  } else {
    i = xm->size[0];
    xm->size[0] = x->size[0];
    emxEnsureCapacity_real32_T(xm, i);
    xm_data = xm->data;
    nx = x->size[0];
    for (i = 0; i < nx; i++) {
      xm_data[i] = x_data[i];
    }
  }
  i = a->size[0] * a->size[1];
  a->size[0] = 1;
  a->size[1] = (int)((double)p + 1.0);
  emxEnsureCapacity_real32_T(a, i);
  a_data = a->data;
  emxInit_real32_T(&c1, 1);
  i = c1->size[0];
  c1->size[0] = xm->size[0];
  emxEnsureCapacity_real32_T(c1, i);
  c1_data = c1->data;
  nx = xm->size[0];
  for (i = 0; i < nx; i++) {
    c1_data[i] = xm_data[i];
  }
  mxl = (int)fmin(p, (double)xm->size[0] - 1.0);
  f_tmp = 2.0 * (double)xm->size[0] - 1.0;
  m2 = frexp(fabs(f_tmp), &ceilLog2);
  if (m2 == 0.5) {
    ceilLog2--;
  }
  m2 = rt_powd_snf(2.0, ceilLog2);
  emxInit_real32_T(&temp_a, 2);
  emxInit_real32_T(&temp_auf, 2);
  emxInit_real32_T(&b_c1, 1);
  if (f_tmp + (double)mxl * ((f_tmp - (double)mxl) - 1.0) <
      m2 * (15.0 * (double)ceilLog2 + 6.0)) {
    mxl = (int)fmin(mxl, (double)xm->size[0] - 1.0);
    ceilLog2 = (int)(2.0 * (double)mxl + 1.0);
    i = b_c1->size[0];
    b_c1->size[0] = ceilLog2;
    emxEnsureCapacity_real32_T(b_c1, i);
    b_c1_data = b_c1->data;
    for (i = 0; i < ceilLog2; i++) {
      b_c1_data[i] = 0.0F;
    }
    for (k = 0; k <= mxl; k++) {
      s = 0.0F;
      i = xm->size[0] - k;
      for (ceilLog2 = 0; ceilLog2 < i; ceilLog2++) {
        s += xm_data[ceilLog2] * xm_data[k + ceilLog2];
      }
      b_c1_data[mxl - k] = s;
      b_c1_data[mxl + k] = s;
    }
  } else {
    emxInit_creal32_T(&X, 1);
    emxInit_real32_T(&sintabinv, 2);
    if ((xm->size[0] == 0) || ((int)m2 == 0)) {
      nx = (int)m2;
      i = X->size[0];
      X->size[0] = (int)m2;
      emxEnsureCapacity_creal32_T(X, i);
      X_data = X->data;
      for (i = 0; i < nx; i++) {
        X_data[i].re = 0.0F;
        X_data[i].im = 0.0F;
      }
    } else {
      bool useRadix2;
      useRadix2 = (((int)m2 > 0) && (((int)m2 & ((int)m2 - 1)) == 0));
      ceilLog2 = c_FFTImplementationCallback_get((int)m2, useRadix2, &nx);
      c_FFTImplementationCallback_gen(nx, useRadix2, temp_a, temp_auf,
                                      sintabinv);
      if (useRadix2) {
        c_FFTImplementationCallback_r2b(c1, (int)m2, temp_a, temp_auf, X);
        X_data = X->data;
      } else {
        c_FFTImplementationCallback_dob(c1, ceilLog2, (int)m2, temp_a, temp_auf,
                                        sintabinv, X);
        X_data = X->data;
      }
    }
    emxFree_real32_T(&sintabinv);
    nx = X->size[0];
    i = c1->size[0];
    c1->size[0] = X->size[0];
    emxEnsureCapacity_real32_T(c1, i);
    c1_data = c1->data;
    for (k = 0; k < nx; k++) {
      c1_data[k] = rt_hypotf_snf(X_data[k].re, X_data[k].im);
    }
    nx = c1->size[0];
    for (i = 0; i < nx; i++) {
      s = c1_data[i];
      c1_data[i] = s * s;
    }
    ifft(c1, X);
    X_data = X->data;
    i = c1->size[0];
    c1->size[0] = X->size[0];
    emxEnsureCapacity_real32_T(c1, i);
    c1_data = c1->data;
    nx = X->size[0];
    for (i = 0; i < nx; i++) {
      c1_data[i] = X_data[i].re;
    }
    emxFree_creal32_T(&X);
    if (mxl + 1 < 1) {
      nx = -1;
    } else {
      nx = mxl;
    }
    emxInit_int32_T(&y, 2);
    y_data = y->data;
    if (mxl < 1) {
      y->size[0] = 1;
      y->size[1] = 0;
    } else {
      i = y->size[0] * y->size[1];
      y->size[0] = 1;
      y->size[1] = mxl;
      emxEnsureCapacity_int32_T(y, i);
      y_data = y->data;
      for (i = 0; i < mxl; i++) {
        y_data[i] = i + 1;
      }
    }
    m2 -= (double)mxl;
    i = b_c1->size[0];
    b_c1->size[0] = (y->size[1] + nx) + 1;
    emxEnsureCapacity_real32_T(b_c1, i);
    b_c1_data = b_c1->data;
    ceilLog2 = y->size[1];
    for (i = 0; i < ceilLog2; i++) {
      b_c1_data[i] = c1_data[(int)(m2 + (double)y_data[i]) - 1];
    }
    for (i = 0; i <= nx; i++) {
      b_c1_data[i + y->size[1]] = c1_data[i];
    }
    emxFree_int32_T(&y);
  }
  nx = b_c1->size[0];
  for (i = 0; i < nx; i++) {
    b_c1_data[i] /= (float)xm->size[0];
  }
  nx = (int)(2.0 * (double)p + 1.0);
  i = c1->size[0];
  c1->size[0] = nx;
  emxEnsureCapacity_real32_T(c1, i);
  c1_data = c1->data;
  for (i = 0; i < nx; i++) {
    c1_data[i] = 0.0F;
  }
  if (p > (double)xm->size[0] - 1.0) {
    m2 = (double)p - ((double)xm->size[0] - 1.0);
    i = b_c1->size[0];
    for (ceilLog2 = 0; ceilLog2 < i; ceilLog2++) {
      f_tmp = fmod(m2, 4.294967296E+9);
      if (f_tmp < 0.0) {
        nx = -(int)(unsigned int)-f_tmp;
      } else {
        nx = (int)(unsigned int)f_tmp;
      }
      c1_data[nx + ceilLog2] = b_c1_data[ceilLog2];
    }
  } else {
    nx = b_c1->size[0];
    for (i = 0; i < nx; i++) {
      c1_data[i] = b_c1_data[i];
    }
  }
  emxFree_real32_T(&xm);
  if (p != 0U) {
    if ((double)p + 1.0 > c1->size[0]) {
      i = 0;
      mxl = -1;
    } else {
      i = (int)((double)p + 1.0) - 1;
      mxl = c1->size[0] - 1;
    }
    m2 = p;
    mxl -= i;
    if ((p == (double)mxl + 1.0) || (p > (double)mxl + 1.0)) {
      m2 = ((double)mxl + 1.0) - 1.0;
    }
    if ((float)m2 == 0.0F) {
      e = c1_data[i];
      i = temp_a->size[0] * temp_a->size[1];
      temp_a->size[0] = 1;
      temp_a->size[1] = 1;
      emxEnsureCapacity_real32_T(temp_a, i);
      xm_data = temp_a->data;
      xm_data[0] = 1.0F;
    } else if (mxl + 1 == 1) {
      mxl = temp_a->size[0] * temp_a->size[1];
      temp_a->size[0] = 1;
      temp_a->size[1] = 1;
      emxEnsureCapacity_real32_T(temp_a, mxl);
      xm_data = temp_a->data;
      xm_data[0] = 1.0F;
      e = c1_data[i];
    } else {
      mxl = temp_a->size[0] * temp_a->size[1];
      temp_a->size[0] = 1;
      ceilLog2 = (int)(float)m2;
      temp_a->size[1] = (int)(float)m2;
      emxEnsureCapacity_real32_T(temp_a, mxl);
      xm_data = temp_a->data;
      for (mxl = 0; mxl < ceilLog2; mxl++) {
        xm_data[mxl] = 0.0F;
      }
      mxl = b_c1->size[0];
      b_c1->size[0] = (int)(float)m2;
      emxEnsureCapacity_real32_T(b_c1, mxl);
      b_c1_data = b_c1->data;
      for (mxl = 0; mxl < ceilLog2; mxl++) {
        b_c1_data[mxl] = 0.0F;
      }
      mxl = temp_auf->size[0] * temp_auf->size[1];
      temp_auf->size[0] = 1;
      temp_auf->size[1] = (int)(float)m2;
      emxEnsureCapacity_real32_T(temp_auf, mxl);
      temp_auf_data = temp_auf->data;
      for (mxl = 0; mxl < ceilLog2; mxl++) {
        temp_auf_data[mxl] = 0.0F;
      }
      xm_data[0] = -c1_data[i];
      e = c1_data[i];
      for (k = 0; k < ceilLog2; k++) {
        s = 0.0F;
        mxl = (int)(((float)k + 1.0F) - 1.0F);
        for (nx = 0; nx < mxl; nx++) {
          float temp_auf_tmp;
          temp_auf_tmp = xm_data[(int)((float)nx + 1.0F) - 1];
          temp_auf_data[(int)((float)nx + 1.0F) - 1] = temp_auf_tmp;
          s += temp_auf_tmp *
               c1_data[i + (int)(((float)k + 1.0F) - ((float)nx + 1.0F))];
        }
        s = -(c1_data[i + (int)((float)k + 1.0F)] + s) / e;
        e *= 1.0F - s * s;
        for (nx = 0; nx < mxl; nx++) {
          xm_data[(int)((float)nx + 1.0F) - 1] =
              temp_auf_data[(int)((float)nx + 1.0F) - 1] +
              s * temp_auf_data[(int)(((float)k + 1.0F) - ((float)nx + 1.0F)) -
                                1];
        }
        xm_data[(int)((float)k + 1.0F) - 1] = s;
      }
      for (i = 0; i < ceilLog2; i++) {
        b_c1_data[i] = xm_data[i];
      }
      if (b_c1->size[0] < 1) {
        nx = 0;
      } else {
        nx = b_c1->size[0];
      }
      i = temp_a->size[0] * temp_a->size[1];
      temp_a->size[0] = 1;
      temp_a->size[1] = nx + 1;
      emxEnsureCapacity_real32_T(temp_a, i);
      xm_data = temp_a->data;
      xm_data[0] = 1.0F;
      for (i = 0; i < nx; i++) {
        xm_data[i + 1] = b_c1_data[i];
      }
    }
    nx = temp_a->size[1];
    for (i = 0; i < nx; i++) {
      a_data[i] = xm_data[i];
    }
  } else {
    a_data[0] = 1.0F;
    e = c1_data[0];
  }
  emxFree_real32_T(&c1);
  emxFree_real32_T(&b_c1);
  emxFree_real32_T(&temp_auf);
  emxFree_real32_T(&temp_a);
  return e;
}

/*
 * File trailer for aryule.c
 *
 * [EOF]
 */
