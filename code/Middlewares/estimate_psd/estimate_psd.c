/*
 * File: estimate_psd.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 18-Sep-2024 13:56:03
 */

/* Include Files */
#include "estimate_psd.h"
#include "aryule.h"
#include "datawrap.h"
#include "estimate_psd_data.h"
#include "estimate_psd_emxutil.h"
#include "estimate_psd_rtwutil.h"
#include "estimate_psd_types.h"
#include "fft.h"
#include "freqz.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Estimate power spectral density using Yule-Walker method
 *
 * Arguments    : const emxArray_real32_T *x
 *                unsigned int order
 *                unsigned int nfft
 *                unsigned int fs
 *                emxArray_real32_T *Pxx
 *                emxArray_real32_T *F
 * Return Type  : void
 */
void estimate_psd(const emxArray_real32_T *x, unsigned int order,
                  unsigned int nfft, unsigned int fs, emxArray_real32_T *Pxx,
                  emxArray_real32_T *F)
{
  emxArray_creal32_T *den;
  emxArray_creal32_T *h;
  emxArray_creal32_T *num;
  emxArray_real32_T *Sxx_unscaled;
  emxArray_real32_T *a;
  emxArray_real32_T *a__1;
  emxArray_real32_T *b;
  emxArray_real32_T *r;
  emxArray_real32_T *w1;
  emxArray_real_T *b_select;
  creal32_T *den_data;
  creal32_T *h_data;
  creal32_T *num_data;
  double d2_tmp;
  double delta1;
  double *select_data;
  float bim;
  float v;
  float *Pxx_data;
  float *a_data;
  float *b_data;
  float *w1_data;
  int i;
  int i1;
  int k;
  int nx;
  emxInit_real32_T(&b, 2);
  v = aryule(x, order, b);
  b_data = b->data;
  d2_tmp = (double)fs - (double)fs / (double)nfft;
  emxInit_real_T(&b_select);
  i = b_select->size[0] * b_select->size[1];
  b_select->size[0] = 1;
  b_select->size[1] = (int)nfft;
  emxEnsureCapacity_real_T(b_select, i);
  select_data = b_select->data;
  if ((int)nfft >= 1) {
    nx = (int)nfft - 1;
    select_data[(int)nfft - 1] = d2_tmp;
    if (b_select->size[1] >= 2) {
      select_data[0] = 0.0;
      if (b_select->size[1] >= 3) {
        if (-d2_tmp == 0.0) {
          delta1 = d2_tmp / ((double)b_select->size[1] - 1.0);
          for (k = 2; k <= nx; k++) {
            select_data[k - 1] =
                (double)(((k << 1) - b_select->size[1]) - 1) * delta1;
          }
          if ((b_select->size[1] & 1) == 1) {
            select_data[b_select->size[1] >> 1] = 0.0;
          }
        } else if ((d2_tmp < 0.0) && (fabs(d2_tmp) > 8.9884656743115785E+307)) {
          delta1 = d2_tmp / ((double)b_select->size[1] - 1.0);
          i = b_select->size[1];
          for (k = 0; k <= i - 3; k++) {
            select_data[k + 1] = delta1 * ((double)k + 1.0);
          }
        } else {
          delta1 = d2_tmp / ((double)b_select->size[1] - 1.0);
          i = b_select->size[1];
          for (k = 0; k <= i - 3; k++) {
            select_data[k + 1] = ((double)k + 1.0) * delta1;
          }
        }
      }
    }
  }
  if (nfft == 0U) {
    i = 0;
  } else {
    i = (int)fmod(nfft, 2.0);
  }
  if (i == 1) {
    double b_select_tmp;
    double select_tmp;
    delta1 = (double)fs / 2.0;
    select_tmp = (double)fs / (2.0 * (double)nfft);
    b_select_tmp = ((double)nfft + 1.0) / 2.0;
    select_data[(int)b_select_tmp - 1] = delta1 - select_tmp;
    select_data[(int)(b_select_tmp + 1.0) - 1] = delta1 + select_tmp;
  } else {
    select_data[(int)((double)nfft / 2.0 + 1.0) - 1] = (double)fs / 2.0;
  }
  select_data[(int)nfft - 1] = d2_tmp;
  emxInit_creal32_T(&h, 1);
  if (b->size[1] == 1) {
    firfreqz(1.0F / b_data[0], nfft, h);
    h_data = h->data;
  } else {
    emxInit_real32_T(&a, 2);
    i1 = a->size[0] * a->size[1];
    a->size[0] = 1;
    a->size[1] = b->size[1];
    emxEnsureCapacity_real32_T(a, i1);
    a_data = a->data;
    nx = b->size[1];
    for (i1 = 0; i1 < nx; i1++) {
      a_data[i1] = b_data[i1];
    }
    nx = (int)fmax(0.0, (double)b->size[1] - 1.0);
    i1 = b->size[0] * b->size[1];
    b->size[0] = 1;
    b->size[1] = nx + 1;
    emxEnsureCapacity_real32_T(b, i1);
    b_data = b->data;
    b_data[0] = 1.0F;
    for (i1 = 0; i1 < nx; i1++) {
      b_data[i1 + 1] = 0.0F;
    }
    emxInit_creal32_T(&num, 2);
    emxInit_creal32_T(&den, 2);
    if (nfft < (unsigned int)a->size[1]) {
      emxInit_real32_T(&r, 2);
      datawrap(b, nfft, r);
      fft(r, num);
      num_data = num->data;
      datawrap(a, nfft, r);
      fft(r, den);
      den_data = den->data;
      emxFree_real32_T(&r);
      if (num->size[1] == den->size[1]) {
        i1 = h->size[0];
        h->size[0] = num->size[1];
        emxEnsureCapacity_creal32_T(h, i1);
        h_data = h->data;
        nx = num->size[1];
        for (i1 = 0; i1 < nx; i1++) {
          float ai;
          float ar;
          float bi;
          float br;
          ar = num_data[i1].re;
          ai = num_data[i1].im;
          br = den_data[i1].re;
          bi = den_data[i1].im;
          if (bi == 0.0F) {
            if (ai == 0.0F) {
              h_data[i1].re = ar / br;
              h_data[i1].im = 0.0F;
            } else if (ar == 0.0F) {
              h_data[i1].re = 0.0F;
              h_data[i1].im = ai / br;
            } else {
              h_data[i1].re = ar / br;
              h_data[i1].im = ai / br;
            }
          } else if (br == 0.0F) {
            if (ar == 0.0F) {
              h_data[i1].re = ai / bi;
              h_data[i1].im = 0.0F;
            } else if (ai == 0.0F) {
              h_data[i1].re = 0.0F;
              h_data[i1].im = -(ar / bi);
            } else {
              h_data[i1].re = ai / bi;
              h_data[i1].im = -(ar / bi);
            }
          } else {
            float brm;
            brm = fabsf(br);
            bim = fabsf(bi);
            if (brm > bim) {
              float s;
              s = bi / br;
              bim = br + s * bi;
              h_data[i1].re = (ar + s * ai) / bim;
              h_data[i1].im = (ai - s * ar) / bim;
            } else if (bim == brm) {
              float s;
              if (br > 0.0F) {
                s = 0.5F;
              } else {
                s = -0.5F;
              }
              if (bi > 0.0F) {
                bim = 0.5F;
              } else {
                bim = -0.5F;
              }
              h_data[i1].re = (ar * s + ai * bim) / brm;
              h_data[i1].im = (ai * s - ar * bim) / brm;
            } else {
              float s;
              s = br / bi;
              bim = bi + s * br;
              h_data[i1].re = (s * ar + ai) / bim;
              h_data[i1].im = (s * ai - ar) / bim;
            }
          }
        }
      } else {
        binary_expand_op(h, num, den);
        h_data = h->data;
      }
    } else {
      b_fft(b, nfft, num);
      num_data = num->data;
      b_fft(a, nfft, den);
      den_data = den->data;
      if (num->size[1] == den->size[1]) {
        i1 = h->size[0];
        h->size[0] = num->size[1];
        emxEnsureCapacity_creal32_T(h, i1);
        h_data = h->data;
        nx = num->size[1];
        for (i1 = 0; i1 < nx; i1++) {
          float ai;
          float ar;
          float bi;
          float br;
          ar = num_data[i1].re;
          ai = num_data[i1].im;
          br = den_data[i1].re;
          bi = den_data[i1].im;
          if (bi == 0.0F) {
            if (ai == 0.0F) {
              h_data[i1].re = ar / br;
              h_data[i1].im = 0.0F;
            } else if (ar == 0.0F) {
              h_data[i1].re = 0.0F;
              h_data[i1].im = ai / br;
            } else {
              h_data[i1].re = ar / br;
              h_data[i1].im = ai / br;
            }
          } else if (br == 0.0F) {
            if (ar == 0.0F) {
              h_data[i1].re = ai / bi;
              h_data[i1].im = 0.0F;
            } else if (ai == 0.0F) {
              h_data[i1].re = 0.0F;
              h_data[i1].im = -(ar / bi);
            } else {
              h_data[i1].re = ai / bi;
              h_data[i1].im = -(ar / bi);
            }
          } else {
            float brm;
            brm = fabsf(br);
            bim = fabsf(bi);
            if (brm > bim) {
              float s;
              s = bi / br;
              bim = br + s * bi;
              h_data[i1].re = (ar + s * ai) / bim;
              h_data[i1].im = (ai - s * ar) / bim;
            } else if (bim == brm) {
              float s;
              if (br > 0.0F) {
                s = 0.5F;
              } else {
                s = -0.5F;
              }
              if (bi > 0.0F) {
                bim = 0.5F;
              } else {
                bim = -0.5F;
              }
              h_data[i1].re = (ar * s + ai * bim) / brm;
              h_data[i1].im = (ai * s - ar * bim) / brm;
            } else {
              float s;
              s = br / bi;
              bim = bi + s * br;
              h_data[i1].re = (s * ar + ai) / bim;
              h_data[i1].im = (s * ai - ar) / bim;
            }
          }
        }
      } else {
        binary_expand_op(h, num, den);
        h_data = h->data;
      }
    }
    emxFree_creal32_T(&den);
    emxFree_creal32_T(&num);
    emxFree_real32_T(&a);
  }
  emxFree_real32_T(&b);
  if (nfft < (unsigned int)h->size[0]) {
    i1 = h->size[0];
    if ((int)nfft < 1) {
      h->size[0] = 0;
    } else {
      h->size[0] = (int)nfft;
    }
    emxEnsureCapacity_creal32_T(h, i1);
    h_data = h->data;
  }
  emxInit_real32_T(&w1, 1);
  i1 = w1->size[0];
  w1->size[0] = b_select->size[1];
  emxEnsureCapacity_real32_T(w1, i1);
  w1_data = w1->data;
  nx = b_select->size[1];
  for (i1 = 0; i1 < nx; i1++) {
    w1_data[i1] = (float)select_data[i1];
  }
  nx = h->size[0];
  emxInit_real32_T(&Sxx_unscaled, 1);
  i1 = Sxx_unscaled->size[0];
  Sxx_unscaled->size[0] = h->size[0];
  emxEnsureCapacity_real32_T(Sxx_unscaled, i1);
  b_data = Sxx_unscaled->data;
  for (k = 0; k < nx; k++) {
    b_data[k] = rt_hypotf_snf(h_data[k].re, h_data[k].im);
  }
  emxFree_creal32_T(&h);
  nx = Sxx_unscaled->size[0];
  for (i1 = 0; i1 < nx; i1++) {
    bim = b_data[i1];
    b_data[i1] = bim * bim;
  }
  emxInit_real32_T(&a__1, 1);
  i1 = a__1->size[0];
  a__1->size[0] = Sxx_unscaled->size[0];
  emxEnsureCapacity_real32_T(a__1, i1);
  a_data = a__1->data;
  if (Sxx_unscaled->size[0] != 0) {
    nx = (Sxx_unscaled->size[0] != 1);
    i1 = Sxx_unscaled->size[0] - 1;
    for (k = 0; k <= i1; k++) {
      a_data[k] = v * b_data[nx * k];
    }
  }
  if (i == 1) {
    delta1 = ((double)nfft + 1.0) / 2.0;
    if (delta1 < 1.0) {
      b_select->size[0] = 1;
      b_select->size[1] = 0;
    } else {
      i = b_select->size[0] * b_select->size[1];
      b_select->size[0] = 1;
      b_select->size[1] = (int)(delta1 - 1.0) + 1;
      emxEnsureCapacity_real_T(b_select, i);
      select_data = b_select->data;
      nx = (int)(delta1 - 1.0);
      for (i = 0; i <= nx; i++) {
        select_data[i] = (double)i + 1.0;
      }
    }
    i = Sxx_unscaled->size[0];
    Sxx_unscaled->size[0] = b_select->size[1];
    emxEnsureCapacity_real32_T(Sxx_unscaled, i);
    b_data = Sxx_unscaled->data;
    nx = b_select->size[1];
    for (i = 0; i < nx; i++) {
      b_data[i] = a_data[(int)select_data[i] - 1];
    }
    if (b_select->size[1] < 2) {
      i = 0;
      i1 = 0;
    } else {
      i = 1;
      i1 = b_select->size[1];
    }
    nx = i1 - i;
    i1 = Pxx->size[0];
    Pxx->size[0] = nx + 1;
    emxEnsureCapacity_real32_T(Pxx, i1);
    Pxx_data = Pxx->data;
    Pxx_data[0] = a_data[(int)select_data[0] - 1];
    for (i1 = 0; i1 < nx; i1++) {
      Pxx_data[i1 + 1] = 2.0F * b_data[i + i1];
    }
  } else {
    delta1 = (double)nfft / 2.0 + 1.0;
    i = b_select->size[0] * b_select->size[1];
    b_select->size[0] = 1;
    b_select->size[1] = (int)(delta1 - 1.0) + 1;
    emxEnsureCapacity_real_T(b_select, i);
    select_data = b_select->data;
    nx = (int)(delta1 - 1.0);
    for (i = 0; i <= nx; i++) {
      select_data[i] = (double)i + 1.0;
    }
    i = Sxx_unscaled->size[0];
    Sxx_unscaled->size[0] = (int)(delta1 - 1.0) + 1;
    emxEnsureCapacity_real32_T(Sxx_unscaled, i);
    b_data = Sxx_unscaled->data;
    for (i = 0; i <= nx; i++) {
      b_data[i] = a_data[i];
    }
    if (b_select->size[1] - 1 < 2) {
      i = 0;
      i1 = 0;
    } else {
      i = 1;
      i1 = (int)(delta1 - 1.0);
    }
    nx = i1 - i;
    i1 = Pxx->size[0];
    Pxx->size[0] = nx + 2;
    emxEnsureCapacity_real32_T(Pxx, i1);
    Pxx_data = Pxx->data;
    Pxx_data[0] = a_data[(int)select_data[0] - 1];
    for (i1 = 0; i1 < nx; i1++) {
      Pxx_data[i1 + 1] = 2.0F * b_data[i + i1];
    }
    Pxx_data[nx + 1] = a_data[(int)select_data[b_select->size[1] - 1] - 1];
  }
  emxFree_real32_T(&Sxx_unscaled);
  emxFree_real32_T(&a__1);
  nx = Pxx->size[0];
  for (i = 0; i < nx; i++) {
    Pxx_data[i] /= (float)fs;
  }
  i = F->size[0];
  F->size[0] = b_select->size[1];
  emxEnsureCapacity_real32_T(F, i);
  b_data = F->data;
  nx = b_select->size[1];
  for (i = 0; i < nx; i++) {
    b_data[i] = w1_data[(int)select_data[i] - 1];
  }
  emxFree_real_T(&b_select);
  emxFree_real32_T(&w1);
}

/*
 * File trailer for estimate_psd.c
 *
 * [EOF]
 */
