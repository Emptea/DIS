/*
 * File: ifft.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 18-Sep-2024 13:56:03
 */

/* Include Files */
#include "ifft.h"
#include "FFTImplementationCallback.h"
#include "estimate_psd_data.h"
#include "estimate_psd_emxutil.h"
#include "estimate_psd_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : const emxArray_real32_T *x
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void ifft(const emxArray_real32_T *x, emxArray_creal32_T *y)
{
  emxArray_creal32_T *b_fv;
  emxArray_creal32_T *fv;
  emxArray_creal32_T *wwc;
  emxArray_real32_T *costab;
  emxArray_real32_T *costab1q;
  emxArray_real32_T *sintab;
  emxArray_real32_T *sintabinv;
  creal32_T *b_fv_data;
  creal32_T *fv_data;
  creal32_T *wwc_data;
  creal32_T *y_data;
  const float *x_data;
  float *costab1q_data;
  float *costab_data;
  float *sintab_data;
  float *sintabinv_data;
  int i;
  int k;
  int nd2;
  int nfft;
  x_data = x->data;
  nfft = x->size[0];
  if (x->size[0] == 0) {
    y->size[0] = 0;
  } else {
    float nt_im;
    int N2blue;
    int nInt2m1;
    int rt;
    bool useRadix2;
    useRadix2 = ((x->size[0] & (x->size[0] - 1)) == 0);
    N2blue = c_FFTImplementationCallback_get(x->size[0], useRadix2, &nd2);
    nt_im = 6.28318548F / (float)nd2;
    rt = (int)((unsigned int)nd2 >> 1) >> 1;
    emxInit_real32_T(&costab1q, 2);
    i = costab1q->size[0] * costab1q->size[1];
    costab1q->size[0] = 1;
    costab1q->size[1] = rt + 1;
    emxEnsureCapacity_real32_T(costab1q, i);
    costab1q_data = costab1q->data;
    costab1q_data[0] = 1.0F;
    nd2 = rt / 2 - 1;
    for (k = 0; k <= nd2; k++) {
      costab1q_data[k + 1] = cosf(nt_im * (float)(k + 1));
    }
    i = nd2 + 2;
    nInt2m1 = rt - 1;
    for (k = i; k <= nInt2m1; k++) {
      costab1q_data[k] = sinf(nt_im * (float)(rt - k));
    }
    costab1q_data[rt] = 0.0F;
    emxInit_real32_T(&costab, 2);
    emxInit_real32_T(&sintab, 2);
    emxInit_real32_T(&sintabinv, 2);
    if (!useRadix2) {
      rt = costab1q->size[1] - 1;
      nd2 = (costab1q->size[1] - 1) << 1;
      i = costab->size[0] * costab->size[1];
      costab->size[0] = 1;
      costab->size[1] = nd2 + 1;
      emxEnsureCapacity_real32_T(costab, i);
      costab_data = costab->data;
      i = sintab->size[0] * sintab->size[1];
      sintab->size[0] = 1;
      sintab->size[1] = nd2 + 1;
      emxEnsureCapacity_real32_T(sintab, i);
      sintab_data = sintab->data;
      costab_data[0] = 1.0F;
      sintab_data[0] = 0.0F;
      i = sintabinv->size[0] * sintabinv->size[1];
      sintabinv->size[0] = 1;
      sintabinv->size[1] = nd2 + 1;
      emxEnsureCapacity_real32_T(sintabinv, i);
      sintabinv_data = sintabinv->data;
      for (k = 0; k < rt; k++) {
        sintabinv_data[k + 1] = costab1q_data[(rt - k) - 1];
      }
      i = costab1q->size[1];
      for (k = i; k <= nd2; k++) {
        sintabinv_data[k] = costab1q_data[k - rt];
      }
      for (k = 0; k < rt; k++) {
        costab_data[k + 1] = costab1q_data[k + 1];
        sintab_data[k + 1] = -costab1q_data[(rt - k) - 1];
      }
      i = costab1q->size[1];
      for (k = i; k <= nd2; k++) {
        costab_data[k] = -costab1q_data[nd2 - k];
        sintab_data[k] = -costab1q_data[k - rt];
      }
    } else {
      rt = costab1q->size[1] - 1;
      nd2 = (costab1q->size[1] - 1) << 1;
      i = costab->size[0] * costab->size[1];
      costab->size[0] = 1;
      costab->size[1] = nd2 + 1;
      emxEnsureCapacity_real32_T(costab, i);
      costab_data = costab->data;
      i = sintab->size[0] * sintab->size[1];
      sintab->size[0] = 1;
      sintab->size[1] = nd2 + 1;
      emxEnsureCapacity_real32_T(sintab, i);
      sintab_data = sintab->data;
      costab_data[0] = 1.0F;
      sintab_data[0] = 0.0F;
      for (k = 0; k < rt; k++) {
        costab_data[k + 1] = costab1q_data[k + 1];
        sintab_data[k + 1] = costab1q_data[(rt - k) - 1];
      }
      i = costab1q->size[1];
      for (k = i; k <= nd2; k++) {
        costab_data[k] = -costab1q_data[nd2 - k];
        sintab_data[k] = costab1q_data[k - rt];
      }
      sintabinv->size[0] = 1;
      sintabinv->size[1] = 0;
    }
    emxFree_real32_T(&costab1q);
    if (useRadix2) {
      c_FFTImplementationCallback_r2b(x, x->size[0], costab, sintab, y);
      y_data = y->data;
      if (y->size[0] > 1) {
        nt_im = 1.0F / (float)y->size[0];
        nd2 = y->size[0];
        for (i = 0; i < nd2; i++) {
          y_data[i].re *= nt_im;
          y_data[i].im *= nt_im;
        }
      }
    } else {
      emxInit_creal32_T(&wwc, 1);
      if ((x->size[0] != 1) && ((x->size[0] & 1) == 0)) {
        int nInt2;
        nd2 = (int)((unsigned int)x->size[0] >> 1);
        nInt2m1 = (nd2 + nd2) - 1;
        i = wwc->size[0];
        wwc->size[0] = nInt2m1;
        emxEnsureCapacity_creal32_T(wwc, i);
        wwc_data = wwc->data;
        rt = 0;
        wwc_data[nd2 - 1].re = 1.0F;
        wwc_data[nd2 - 1].im = 0.0F;
        nInt2 = nd2 << 1;
        for (k = 0; k <= nd2 - 2; k++) {
          int b_y;
          b_y = ((k + 1) << 1) - 1;
          if (nInt2 - rt <= b_y) {
            rt += b_y - nInt2;
          } else {
            rt += b_y;
          }
          nt_im = 3.14159274F * (float)rt / (float)nd2;
          i = (nd2 - k) - 2;
          wwc_data[i].re = cosf(nt_im);
          wwc_data[i].im = -sinf(nt_im);
        }
        i = nInt2m1 - 1;
        for (k = i; k >= nd2; k--) {
          wwc_data[k] = wwc_data[(nInt2m1 - k) - 1];
        }
      } else {
        int nInt2;
        nInt2m1 = (x->size[0] + x->size[0]) - 1;
        i = wwc->size[0];
        wwc->size[0] = nInt2m1;
        emxEnsureCapacity_creal32_T(wwc, i);
        wwc_data = wwc->data;
        rt = 0;
        wwc_data[x->size[0] - 1].re = 1.0F;
        wwc_data[x->size[0] - 1].im = 0.0F;
        nInt2 = x->size[0] << 1;
        i = x->size[0];
        for (k = 0; k <= i - 2; k++) {
          int b_y;
          b_y = ((k + 1) << 1) - 1;
          if (nInt2 - rt <= b_y) {
            rt += b_y - nInt2;
          } else {
            rt += b_y;
          }
          nt_im = 3.14159274F * (float)rt / (float)x->size[0];
          wwc_data[(x->size[0] - k) - 2].re = cosf(nt_im);
          wwc_data[(x->size[0] - k) - 2].im = -sinf(nt_im);
        }
        i = nInt2m1 - 1;
        for (k = i; k >= nfft; k--) {
          wwc_data[k] = wwc_data[(nInt2m1 - k) - 1];
        }
      }
      i = y->size[0];
      y->size[0] = x->size[0];
      emxEnsureCapacity_creal32_T(y, i);
      y_data = y->data;
      emxInit_creal32_T(&fv, 1);
      emxInit_creal32_T(&b_fv, 1);
      if ((N2blue != 1) && ((x->size[0] & 1) == 0)) {
        e_FFTImplementationCallback_doH(x, y, x->size[0], x->size[0], N2blue,
                                        wwc, costab, sintab, costab, sintabinv);
      } else {
        float b_re_tmp;
        float c_re_tmp;
        float re_tmp;
        nd2 = x->size[0];
        for (k = 0; k < nd2; k++) {
          i = (nfft + k) - 1;
          y_data[k].re = wwc_data[i].re * x_data[k];
          y_data[k].im = wwc_data[i].im * -x_data[k];
        }
        i = x->size[0] + 1;
        for (k = i; k <= nfft; k++) {
          y_data[k - 1].re = 0.0F;
          y_data[k - 1].im = 0.0F;
        }
        d_FFTImplementationCallback_r2b(y, N2blue, costab, sintab, fv);
        fv_data = fv->data;
        d_FFTImplementationCallback_r2b(wwc, N2blue, costab, sintab, b_fv);
        i = b_fv->size[0];
        b_fv->size[0] = fv->size[0];
        emxEnsureCapacity_creal32_T(b_fv, i);
        b_fv_data = b_fv->data;
        nd2 = fv->size[0];
        for (i = 0; i < nd2; i++) {
          nt_im = fv_data[i].re;
          re_tmp = b_fv_data[i].im;
          b_re_tmp = fv_data[i].im;
          c_re_tmp = b_fv_data[i].re;
          b_fv_data[i].re = nt_im * c_re_tmp - b_re_tmp * re_tmp;
          b_fv_data[i].im = nt_im * re_tmp + b_re_tmp * c_re_tmp;
        }
        d_FFTImplementationCallback_r2b(b_fv, N2blue, costab, sintabinv, fv);
        fv_data = fv->data;
        if (fv->size[0] > 1) {
          nt_im = 1.0F / (float)fv->size[0];
          nd2 = fv->size[0];
          for (i = 0; i < nd2; i++) {
            fv_data[i].re *= nt_im;
            fv_data[i].im *= nt_im;
          }
        }
        i = (int)(float)x->size[0];
        nInt2m1 = wwc->size[0];
        for (k = i; k <= nInt2m1; k++) {
          float ar;
          nt_im = wwc_data[k - 1].re;
          re_tmp = fv_data[k - 1].im;
          b_re_tmp = wwc_data[k - 1].im;
          c_re_tmp = fv_data[k - 1].re;
          ar = nt_im * c_re_tmp + b_re_tmp * re_tmp;
          nt_im = nt_im * re_tmp - b_re_tmp * c_re_tmp;
          if (nt_im == 0.0F) {
            nd2 = k - i;
            y_data[nd2].re = ar / (float)nfft;
            y_data[nd2].im = 0.0F;
          } else if (ar == 0.0F) {
            nd2 = k - i;
            y_data[nd2].re = 0.0F;
            y_data[nd2].im = nt_im / (float)nfft;
          } else {
            nd2 = k - i;
            y_data[nd2].re = ar / (float)nfft;
            y_data[nd2].im = nt_im / (float)nfft;
          }
        }
      }
      emxFree_creal32_T(&b_fv);
      emxFree_creal32_T(&fv);
      emxFree_creal32_T(&wwc);
    }
    emxFree_real32_T(&sintabinv);
    emxFree_real32_T(&sintab);
    emxFree_real32_T(&costab);
  }
}

/*
 * File trailer for ifft.c
 *
 * [EOF]
 */
