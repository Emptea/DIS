/*
 * File: FFTImplementationCallback.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 18-Sep-2024 13:56:03
 */

/* Include Files */
#include "FFTImplementationCallback.h"
#include "estimate_psd_data.h"
#include "estimate_psd_emxutil.h"
#include "estimate_psd_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Declarations */
static void c_FFTImplementationCallback_doH(const emxArray_real32_T *x,
                                            emxArray_creal32_T *y,
                                            int unsigned_nRows,
                                            const emxArray_real32_T *costab,
                                            const emxArray_real32_T *sintab);

static void d_FFTImplementationCallback_doH(
    const emxArray_real32_T *x, emxArray_creal32_T *y, int nrowsx, int nRows,
    int nfft, const emxArray_creal32_T *wwc, const emxArray_real32_T *costab,
    const emxArray_real32_T *sintab, const emxArray_real32_T *costabinv,
    const emxArray_real32_T *sintabinv);

static void d_FFTImplementationCallback_gen(int nRows,
                                            emxArray_real32_T *costab,
                                            emxArray_real32_T *sintab,
                                            emxArray_real32_T *sintabinv);

static void d_FFTImplementationCallback_get(int nRowsM1, int nfftLen,
                                            emxArray_int32_T *bitrevIndex);

static void e_FFTImplementationCallback_get(
    const emxArray_real32_T *costab, const emxArray_real32_T *sintab,
    const emxArray_real32_T *costabinv, const emxArray_real32_T *sintabinv,
    emxArray_real32_T *hcostab, emxArray_real32_T *hsintab,
    emxArray_real32_T *hcostabinv, emxArray_real32_T *hsintabinv);

static void f_FFTImplementationCallback_get(emxArray_creal32_T *y,
                                            const emxArray_creal32_T *reconVar1,
                                            const emxArray_creal32_T *reconVar2,
                                            const emxArray_int32_T *wrapIndex,
                                            int hnRows);

/* Function Definitions */
/*
 * Arguments    : const emxArray_real32_T *x
 *                emxArray_creal32_T *y
 *                int unsigned_nRows
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 * Return Type  : void
 */
static void c_FFTImplementationCallback_doH(const emxArray_real32_T *x,
                                            emxArray_creal32_T *y,
                                            int unsigned_nRows,
                                            const emxArray_real32_T *costab,
                                            const emxArray_real32_T *sintab)
{
  emxArray_creal32_T *reconVar1;
  emxArray_creal32_T *reconVar2;
  emxArray_int32_T *bitrevIndex;
  emxArray_int32_T *wrapIndex;
  emxArray_real32_T *hcostab;
  emxArray_real32_T *hsintab;
  creal32_T *reconVar1_data;
  creal32_T *reconVar2_data;
  creal32_T *y_data;
  double d;
  double z;
  const float *costab_data;
  const float *sintab_data;
  const float *x_data;
  float im;
  float re;
  float temp_im;
  float temp_re;
  float twid_im;
  float twid_re;
  float *hcostab_data;
  float *hsintab_data;
  int hszCostab;
  int i;
  int iDelta;
  int iDelta2;
  int iheight;
  int istart;
  int k;
  int nRows;
  int nRowsD2;
  int nRowsM2;
  int *wrapIndex_data;
  bool nxeven;
  sintab_data = sintab->data;
  costab_data = costab->data;
  y_data = y->data;
  x_data = x->data;
  nRows = (int)((unsigned int)unsigned_nRows >> 1);
  nRowsM2 = nRows - 2;
  nRowsD2 = nRows / 2;
  k = nRowsD2 / 2;
  hszCostab = (int)((unsigned int)costab->size[1] >> 1);
  emxInit_real32_T(&hcostab, 2);
  istart = hcostab->size[0] * hcostab->size[1];
  hcostab->size[0] = 1;
  hcostab->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(hcostab, istart);
  hcostab_data = hcostab->data;
  emxInit_real32_T(&hsintab, 2);
  istart = hsintab->size[0] * hsintab->size[1];
  hsintab->size[0] = 1;
  hsintab->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(hsintab, istart);
  hsintab_data = hsintab->data;
  for (i = 0; i < hszCostab; i++) {
    istart = ((i + 1) << 1) - 2;
    hcostab_data[i] = costab_data[istart];
    hsintab_data[i] = sintab_data[istart];
  }
  emxInit_creal32_T(&reconVar1, 1);
  istart = reconVar1->size[0];
  reconVar1->size[0] = nRows;
  emxEnsureCapacity_creal32_T(reconVar1, istart);
  reconVar1_data = reconVar1->data;
  emxInit_creal32_T(&reconVar2, 1);
  istart = reconVar2->size[0];
  reconVar2->size[0] = nRows;
  emxEnsureCapacity_creal32_T(reconVar2, istart);
  reconVar2_data = reconVar2->data;
  emxInit_int32_T(&wrapIndex, 2);
  istart = wrapIndex->size[0] * wrapIndex->size[1];
  wrapIndex->size[0] = 1;
  wrapIndex->size[1] = nRows;
  emxEnsureCapacity_int32_T(wrapIndex, istart);
  wrapIndex_data = wrapIndex->data;
  for (i = 0; i < nRows; i++) {
    re = sintab_data[i];
    im = costab_data[i];
    reconVar1_data[i].re = re + 1.0F;
    reconVar1_data[i].im = -im;
    reconVar2_data[i].re = 1.0F - re;
    reconVar2_data[i].im = im;
    if (i + 1 != 1) {
      wrapIndex_data[i] = (nRows - i) + 1;
    } else {
      wrapIndex_data[0] = 1;
    }
  }
  z = (double)unsigned_nRows / 2.0;
  emxInit_int32_T(&bitrevIndex, 1);
  istart = y->size[0];
  if (istart > nRows) {
    istart = nRows;
  }
  d_FFTImplementationCallback_get(istart - 1, (int)z, bitrevIndex);
  wrapIndex_data = bitrevIndex->data;
  if ((x->size[0] & 1) == 0) {
    nxeven = true;
    istart = x->size[0];
  } else if (x->size[0] >= unsigned_nRows) {
    nxeven = true;
    istart = unsigned_nRows;
  } else {
    nxeven = false;
    istart = x->size[0] - 1;
  }
  if (istart <= unsigned_nRows) {
    hszCostab = istart;
  } else {
    hszCostab = unsigned_nRows;
  }
  d = (double)hszCostab / 2.0;
  if (istart > unsigned_nRows) {
    istart = unsigned_nRows;
  }
  istart = (int)((double)istart / 2.0);
  for (i = 0; i < istart; i++) {
    hszCostab = i << 1;
    y_data[wrapIndex_data[i] - 1].re = x_data[hszCostab];
    y_data[wrapIndex_data[i] - 1].im = x_data[hszCostab + 1];
  }
  if (!nxeven) {
    istart = wrapIndex_data[(int)d] - 1;
    y_data[istart].re = x_data[(int)d << 1];
    y_data[istart].im = 0.0F;
  }
  emxFree_int32_T(&bitrevIndex);
  if (nRows > 1) {
    for (i = 0; i <= nRowsM2; i += 2) {
      re = y_data[i + 1].re;
      im = y_data[i + 1].im;
      temp_re = re;
      temp_im = im;
      twid_re = y_data[i].re;
      twid_im = y_data[i].im;
      re = twid_re - re;
      im = twid_im - im;
      y_data[i + 1].re = re;
      y_data[i + 1].im = im;
      twid_re += temp_re;
      twid_im += temp_im;
      y_data[i].re = twid_re;
      y_data[i].im = twid_im;
    }
  }
  iDelta = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      hszCostab = i + iDelta;
      temp_re = y_data[hszCostab].re;
      temp_im = y_data[hszCostab].im;
      y_data[hszCostab].re = y_data[i].re - temp_re;
      y_data[hszCostab].im = y_data[i].im - temp_im;
      y_data[i].re += temp_re;
      y_data[i].im += temp_im;
    }
    istart = 1;
    for (hszCostab = k; hszCostab < nRowsD2; hszCostab += k) {
      twid_re = hcostab_data[hszCostab];
      twid_im = hsintab_data[hszCostab];
      i = istart;
      nRows = istart + iheight;
      while (i < nRows) {
        nRowsM2 = i + iDelta;
        re = y_data[nRowsM2].im;
        im = y_data[nRowsM2].re;
        temp_re = twid_re * im - twid_im * re;
        temp_im = twid_re * re + twid_im * im;
        y_data[nRowsM2].re = y_data[i].re - temp_re;
        y_data[nRowsM2].im = y_data[i].im - temp_im;
        y_data[i].re += temp_re;
        y_data[i].im += temp_im;
        i += iDelta2;
      }
      istart++;
    }
    k /= 2;
    iDelta = iDelta2;
    iDelta2 += iDelta2;
    iheight -= iDelta;
  }
  emxFree_real32_T(&hsintab);
  emxFree_real32_T(&hcostab);
  f_FFTImplementationCallback_get(y, reconVar1, reconVar2, wrapIndex, (int)z);
  emxFree_creal32_T(&reconVar2);
  emxFree_creal32_T(&reconVar1);
  emxFree_int32_T(&wrapIndex);
}

/*
 * Arguments    : const emxArray_real32_T *x
 *                emxArray_creal32_T *y
 *                int nrowsx
 *                int nRows
 *                int nfft
 *                const emxArray_creal32_T *wwc
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                const emxArray_real32_T *costabinv
 *                const emxArray_real32_T *sintabinv
 * Return Type  : void
 */
static void d_FFTImplementationCallback_doH(
    const emxArray_real32_T *x, emxArray_creal32_T *y, int nrowsx, int nRows,
    int nfft, const emxArray_creal32_T *wwc, const emxArray_real32_T *costab,
    const emxArray_real32_T *sintab, const emxArray_real32_T *costabinv,
    const emxArray_real32_T *sintabinv)
{
  emxArray_creal32_T *b_fv;
  emxArray_creal32_T *fv;
  emxArray_creal32_T *reconVar1;
  emxArray_creal32_T *reconVar2;
  emxArray_creal32_T *ytmp;
  emxArray_int32_T *wrapIndex;
  emxArray_real32_T *a__1;
  emxArray_real32_T *costable;
  emxArray_real32_T *hcostabinv;
  emxArray_real32_T *hsintab;
  emxArray_real32_T *hsintabinv;
  emxArray_real32_T *sintable;
  const creal32_T *wwc_data;
  creal32_T *b_fv_data;
  creal32_T *fv_data;
  creal32_T *reconVar1_data;
  creal32_T *reconVar2_data;
  creal32_T *y_data;
  creal32_T *ytmp_data;
  double z_tmp;
  const float *x_data;
  float a_im;
  float a_re;
  float b_im;
  float b_re;
  float *costable_data;
  float *sintable_data;
  int b_i;
  int hnRows;
  int i;
  int k1;
  int u0;
  int *wrapIndex_data;
  bool nxeven;
  wwc_data = wwc->data;
  y_data = y->data;
  x_data = x->data;
  hnRows = (int)((unsigned int)nRows >> 1);
  emxInit_creal32_T(&ytmp, 1);
  i = ytmp->size[0];
  ytmp->size[0] = hnRows;
  emxEnsureCapacity_creal32_T(ytmp, i);
  ytmp_data = ytmp->data;
  if (hnRows > nrowsx) {
    i = ytmp->size[0];
    ytmp->size[0] = hnRows;
    emxEnsureCapacity_creal32_T(ytmp, i);
    ytmp_data = ytmp->data;
    for (i = 0; i < hnRows; i++) {
      ytmp_data[i].re = 0.0F;
      ytmp_data[i].im = 0.0F;
    }
  }
  if ((x->size[0] & 1) == 0) {
    nxeven = true;
    u0 = x->size[0];
  } else if (x->size[0] >= nRows) {
    nxeven = true;
    u0 = nRows;
  } else {
    nxeven = false;
    u0 = x->size[0] - 1;
  }
  if (u0 > nRows) {
    u0 = nRows;
  }
  emxInit_real32_T(&a__1, 2);
  emxInit_real32_T(&costable, 2);
  emxInit_real32_T(&sintable, 2);
  d_FFTImplementationCallback_gen(nRows << 1, costable, sintable, a__1);
  sintable_data = sintable->data;
  costable_data = costable->data;
  emxInit_real32_T(&hsintab, 2);
  emxInit_real32_T(&hcostabinv, 2);
  emxInit_real32_T(&hsintabinv, 2);
  e_FFTImplementationCallback_get(costab, sintab, costabinv, sintabinv, a__1,
                                  hsintab, hcostabinv, hsintabinv);
  emxInit_creal32_T(&reconVar1, 1);
  i = reconVar1->size[0];
  reconVar1->size[0] = hnRows;
  emxEnsureCapacity_creal32_T(reconVar1, i);
  reconVar1_data = reconVar1->data;
  emxInit_creal32_T(&reconVar2, 1);
  i = reconVar2->size[0];
  reconVar2->size[0] = hnRows;
  emxEnsureCapacity_creal32_T(reconVar2, i);
  reconVar2_data = reconVar2->data;
  emxInit_int32_T(&wrapIndex, 2);
  i = wrapIndex->size[0] * wrapIndex->size[1];
  wrapIndex->size[0] = 1;
  wrapIndex->size[1] = hnRows;
  emxEnsureCapacity_int32_T(wrapIndex, i);
  wrapIndex_data = wrapIndex->data;
  for (b_i = 0; b_i < hnRows; b_i++) {
    i = b_i << 1;
    a_re = sintable_data[i];
    a_im = costable_data[i];
    reconVar1_data[b_i].re = a_re + 1.0F;
    reconVar1_data[b_i].im = -a_im;
    reconVar2_data[b_i].re = 1.0F - a_re;
    reconVar2_data[b_i].im = a_im;
    if (b_i + 1 != 1) {
      wrapIndex_data[b_i] = (hnRows - b_i) + 1;
    } else {
      wrapIndex_data[0] = 1;
    }
  }
  emxFree_real32_T(&sintable);
  emxFree_real32_T(&costable);
  z_tmp = (double)u0 / 2.0;
  i = (int)z_tmp;
  for (k1 = 0; k1 < i; k1++) {
    a_re = wwc_data[(hnRows + k1) - 1].re;
    a_im = wwc_data[(hnRows + k1) - 1].im;
    b_i = k1 << 1;
    b_re = x_data[b_i];
    b_im = x_data[b_i + 1];
    ytmp_data[k1].re = a_re * b_re + a_im * b_im;
    ytmp_data[k1].im = a_re * b_im - a_im * b_re;
  }
  if (!nxeven) {
    a_re = wwc_data[(hnRows + (int)z_tmp) - 1].re;
    a_im = wwc_data[(hnRows + (int)z_tmp) - 1].im;
    b_re = x_data[(int)z_tmp << 1];
    ytmp_data[(int)((double)u0 / 2.0)].re = a_re * b_re + a_im * 0.0F;
    ytmp_data[(int)((double)u0 / 2.0)].im = a_re * 0.0F - a_im * b_re;
    if ((int)z_tmp + 2 <= hnRows) {
      i = (int)z_tmp + 2;
      for (b_i = i; b_i <= hnRows; b_i++) {
        ytmp_data[b_i - 1].re = 0.0F;
        ytmp_data[b_i - 1].im = 0.0F;
      }
    }
  } else if ((int)z_tmp + 1 <= hnRows) {
    i = (int)z_tmp + 1;
    for (b_i = i; b_i <= hnRows; b_i++) {
      ytmp_data[b_i - 1].re = 0.0F;
      ytmp_data[b_i - 1].im = 0.0F;
    }
  }
  b_i = (int)((double)nfft / 2.0);
  emxInit_creal32_T(&fv, 1);
  d_FFTImplementationCallback_r2b(ytmp, b_i, a__1, hsintab, fv);
  fv_data = fv->data;
  emxInit_creal32_T(&b_fv, 1);
  d_FFTImplementationCallback_r2b(wwc, b_i, a__1, hsintab, b_fv);
  emxFree_real32_T(&hsintab);
  emxFree_real32_T(&a__1);
  i = b_fv->size[0];
  b_fv->size[0] = fv->size[0];
  emxEnsureCapacity_creal32_T(b_fv, i);
  b_fv_data = b_fv->data;
  u0 = fv->size[0];
  for (i = 0; i < u0; i++) {
    a_re = fv_data[i].re;
    a_im = b_fv_data[i].im;
    b_re = fv_data[i].im;
    b_im = b_fv_data[i].re;
    b_fv_data[i].re = a_re * b_im - b_re * a_im;
    b_fv_data[i].im = a_re * a_im + b_re * b_im;
  }
  d_FFTImplementationCallback_r2b(b_fv, b_i, hcostabinv, hsintabinv, fv);
  fv_data = fv->data;
  emxFree_creal32_T(&b_fv);
  emxFree_real32_T(&hsintabinv);
  emxFree_real32_T(&hcostabinv);
  if (fv->size[0] > 1) {
    a_re = 1.0F / (float)fv->size[0];
    u0 = fv->size[0];
    for (i = 0; i < u0; i++) {
      fv_data[i].re *= a_re;
      fv_data[i].im *= a_re;
    }
  }
  i = (int)(float)hnRows;
  b_i = wwc->size[0];
  for (u0 = i; u0 <= b_i; u0++) {
    a_re = wwc_data[u0 - 1].re;
    a_im = fv_data[u0 - 1].im;
    b_re = wwc_data[u0 - 1].im;
    b_im = fv_data[u0 - 1].re;
    ytmp_data[u0 - (int)(float)hnRows].re = a_re * b_im + b_re * a_im;
    ytmp_data[u0 - (int)(float)hnRows].im = a_re * a_im - b_re * b_im;
  }
  emxFree_creal32_T(&fv);
  for (b_i = 0; b_i < hnRows; b_i++) {
    float b_ytmp_re_tmp;
    float c_ytmp_re_tmp;
    float ytmp_im_tmp;
    float ytmp_re_tmp;
    i = wrapIndex_data[b_i];
    a_re = ytmp_data[b_i].re;
    a_im = reconVar1_data[b_i].im;
    b_re = ytmp_data[b_i].im;
    b_im = reconVar1_data[b_i].re;
    ytmp_re_tmp = ytmp_data[i - 1].re;
    ytmp_im_tmp = -ytmp_data[i - 1].im;
    b_ytmp_re_tmp = reconVar2_data[b_i].im;
    c_ytmp_re_tmp = reconVar2_data[b_i].re;
    y_data[b_i].re =
        0.5F * ((a_re * b_im - b_re * a_im) +
                (ytmp_re_tmp * c_ytmp_re_tmp - ytmp_im_tmp * b_ytmp_re_tmp));
    y_data[b_i].im =
        0.5F * ((a_re * a_im + b_re * b_im) +
                (ytmp_re_tmp * b_ytmp_re_tmp + ytmp_im_tmp * c_ytmp_re_tmp));
    y_data[hnRows + b_i].re =
        0.5F * ((a_re * c_ytmp_re_tmp - b_re * b_ytmp_re_tmp) +
                (ytmp_re_tmp * b_im - ytmp_im_tmp * a_im));
    y_data[hnRows + b_i].im =
        0.5F * ((a_re * b_ytmp_re_tmp + b_re * c_ytmp_re_tmp) +
                (ytmp_re_tmp * a_im + ytmp_im_tmp * b_im));
  }
  emxFree_creal32_T(&reconVar2);
  emxFree_creal32_T(&reconVar1);
  emxFree_int32_T(&wrapIndex);
  emxFree_creal32_T(&ytmp);
}

/*
 * Arguments    : int nRows
 *                emxArray_real32_T *costab
 *                emxArray_real32_T *sintab
 *                emxArray_real32_T *sintabinv
 * Return Type  : void
 */
static void d_FFTImplementationCallback_gen(int nRows,
                                            emxArray_real32_T *costab,
                                            emxArray_real32_T *sintab,
                                            emxArray_real32_T *sintabinv)
{
  emxArray_real32_T *costab1q;
  float e;
  float *costab1q_data;
  float *costab_data;
  float *sintab_data;
  float *sintabinv_data;
  int i;
  int k;
  int n;
  int nd2;
  e = 6.28318548F / (float)nRows;
  n = (int)((unsigned int)nRows >> 1) >> 1;
  emxInit_real32_T(&costab1q, 2);
  i = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = n + 1;
  emxEnsureCapacity_real32_T(costab1q, i);
  costab1q_data = costab1q->data;
  costab1q_data[0] = 1.0F;
  nd2 = n / 2 - 1;
  for (k = 0; k <= nd2; k++) {
    costab1q_data[k + 1] = cosf(e * (float)(k + 1));
  }
  i = nd2 + 2;
  nd2 = n - 1;
  for (k = i; k <= nd2; k++) {
    costab1q_data[k] = sinf(e * (float)(n - k));
  }
  costab1q_data[n] = 0.0F;
  n = costab1q->size[1] - 1;
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
  for (k = 0; k < n; k++) {
    sintabinv_data[k + 1] = costab1q_data[(n - k) - 1];
  }
  i = costab1q->size[1];
  for (k = i; k <= nd2; k++) {
    sintabinv_data[k] = costab1q_data[k - n];
  }
  for (k = 0; k < n; k++) {
    costab_data[k + 1] = costab1q_data[k + 1];
    sintab_data[k + 1] = -costab1q_data[(n - k) - 1];
  }
  i = costab1q->size[1];
  for (k = i; k <= nd2; k++) {
    costab_data[k] = -costab1q_data[nd2 - k];
    sintab_data[k] = -costab1q_data[k - n];
  }
  emxFree_real32_T(&costab1q);
}

/*
 * Arguments    : int nRowsM1
 *                int nfftLen
 *                emxArray_int32_T *bitrevIndex
 * Return Type  : void
 */
static void d_FFTImplementationCallback_get(int nRowsM1, int nfftLen,
                                            emxArray_int32_T *bitrevIndex)
{
  int b_j1;
  int iy;
  int ju;
  int *bitrevIndex_data;
  ju = 0;
  iy = 1;
  b_j1 = bitrevIndex->size[0];
  bitrevIndex->size[0] = nfftLen;
  emxEnsureCapacity_int32_T(bitrevIndex, b_j1);
  bitrevIndex_data = bitrevIndex->data;
  for (b_j1 = 0; b_j1 < nfftLen; b_j1++) {
    bitrevIndex_data[b_j1] = 0;
  }
  for (b_j1 = 0; b_j1 < nRowsM1; b_j1++) {
    bool tst;
    bitrevIndex_data[b_j1] = iy;
    iy = nfftLen;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }
    iy = ju + 1;
  }
  bitrevIndex_data[nRowsM1] = iy;
}

/*
 * Arguments    : const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                const emxArray_real32_T *costabinv
 *                const emxArray_real32_T *sintabinv
 *                emxArray_real32_T *hcostab
 *                emxArray_real32_T *hsintab
 *                emxArray_real32_T *hcostabinv
 *                emxArray_real32_T *hsintabinv
 * Return Type  : void
 */
static void e_FFTImplementationCallback_get(
    const emxArray_real32_T *costab, const emxArray_real32_T *sintab,
    const emxArray_real32_T *costabinv, const emxArray_real32_T *sintabinv,
    emxArray_real32_T *hcostab, emxArray_real32_T *hsintab,
    emxArray_real32_T *hcostabinv, emxArray_real32_T *hsintabinv)
{
  const float *costab_data;
  const float *costabinv_data;
  const float *sintab_data;
  const float *sintabinv_data;
  float *hcostab_data;
  float *hcostabinv_data;
  float *hsintab_data;
  float *hsintabinv_data;
  int hcostab_tmp;
  int hszCostab;
  int i;
  sintabinv_data = sintabinv->data;
  costabinv_data = costabinv->data;
  sintab_data = sintab->data;
  costab_data = costab->data;
  hszCostab = (int)((unsigned int)costab->size[1] >> 1);
  hcostab_tmp = hcostab->size[0] * hcostab->size[1];
  hcostab->size[0] = 1;
  hcostab->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(hcostab, hcostab_tmp);
  hcostab_data = hcostab->data;
  hcostab_tmp = hsintab->size[0] * hsintab->size[1];
  hsintab->size[0] = 1;
  hsintab->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(hsintab, hcostab_tmp);
  hsintab_data = hsintab->data;
  hcostab_tmp = hcostabinv->size[0] * hcostabinv->size[1];
  hcostabinv->size[0] = 1;
  hcostabinv->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(hcostabinv, hcostab_tmp);
  hcostabinv_data = hcostabinv->data;
  hcostab_tmp = hsintabinv->size[0] * hsintabinv->size[1];
  hsintabinv->size[0] = 1;
  hsintabinv->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(hsintabinv, hcostab_tmp);
  hsintabinv_data = hsintabinv->data;
  for (i = 0; i < hszCostab; i++) {
    hcostab_tmp = ((i + 1) << 1) - 2;
    hcostab_data[i] = costab_data[hcostab_tmp];
    hsintab_data[i] = sintab_data[hcostab_tmp];
    hcostabinv_data[i] = costabinv_data[hcostab_tmp];
    hsintabinv_data[i] = sintabinv_data[hcostab_tmp];
  }
}

/*
 * Arguments    : emxArray_creal32_T *y
 *                const emxArray_creal32_T *reconVar1
 *                const emxArray_creal32_T *reconVar2
 *                const emxArray_int32_T *wrapIndex
 *                int hnRows
 * Return Type  : void
 */
static void f_FFTImplementationCallback_get(emxArray_creal32_T *y,
                                            const emxArray_creal32_T *reconVar1,
                                            const emxArray_creal32_T *reconVar2,
                                            const emxArray_int32_T *wrapIndex,
                                            int hnRows)
{
  const creal32_T *reconVar1_data;
  const creal32_T *reconVar2_data;
  creal32_T *y_data;
  float b_temp1_re_tmp;
  float b_temp2_re_tmp;
  float b_y_re_tmp;
  float c_y_re_tmp;
  float d_y_re_tmp;
  float temp1_im_tmp;
  float temp1_re_tmp;
  float temp1_re_tmp_tmp;
  float y_im_tmp;
  float y_re_tmp;
  const int *wrapIndex_data;
  int i;
  int iterVar;
  wrapIndex_data = wrapIndex->data;
  reconVar2_data = reconVar2->data;
  reconVar1_data = reconVar1->data;
  y_data = y->data;
  iterVar = (int)((unsigned int)hnRows >> 1);
  temp1_re_tmp_tmp = y_data[0].re;
  temp1_im_tmp = y_data[0].im;
  y_re_tmp = temp1_re_tmp_tmp * reconVar1_data[0].re;
  y_im_tmp = temp1_re_tmp_tmp * reconVar1_data[0].im;
  b_y_re_tmp = temp1_re_tmp_tmp * reconVar2_data[0].re;
  temp1_re_tmp_tmp *= reconVar2_data[0].im;
  y_data[0].re = 0.5F * ((y_re_tmp - temp1_im_tmp * reconVar1_data[0].im) +
                         (b_y_re_tmp - -temp1_im_tmp * reconVar2_data[0].im));
  y_data[0].im =
      0.5F * ((y_im_tmp + temp1_im_tmp * reconVar1_data[0].re) +
              (temp1_re_tmp_tmp + -temp1_im_tmp * reconVar2_data[0].re));
  y_data[hnRows].re =
      0.5F * ((b_y_re_tmp - temp1_im_tmp * reconVar2_data[0].im) +
              (y_re_tmp - -temp1_im_tmp * reconVar1_data[0].im));
  y_data[hnRows].im =
      0.5F * ((temp1_re_tmp_tmp + temp1_im_tmp * reconVar2_data[0].re) +
              (y_im_tmp + -temp1_im_tmp * reconVar1_data[0].re));
  for (i = 2; i <= iterVar; i++) {
    float temp2_im_tmp;
    float temp2_re_tmp;
    int b_i;
    int i1;
    temp1_re_tmp = y_data[i - 1].re;
    temp1_im_tmp = y_data[i - 1].im;
    b_i = wrapIndex_data[i - 1];
    temp2_re_tmp = y_data[b_i - 1].re;
    temp2_im_tmp = y_data[b_i - 1].im;
    y_re_tmp = reconVar1_data[i - 1].im;
    b_y_re_tmp = reconVar1_data[i - 1].re;
    c_y_re_tmp = reconVar2_data[i - 1].im;
    d_y_re_tmp = reconVar2_data[i - 1].re;
    y_data[i - 1].re =
        0.5F * ((temp1_re_tmp * b_y_re_tmp - temp1_im_tmp * y_re_tmp) +
                (temp2_re_tmp * d_y_re_tmp - -temp2_im_tmp * c_y_re_tmp));
    y_data[i - 1].im =
        0.5F * ((temp1_re_tmp * y_re_tmp + temp1_im_tmp * b_y_re_tmp) +
                (temp2_re_tmp * c_y_re_tmp + -temp2_im_tmp * d_y_re_tmp));
    i1 = (hnRows + i) - 1;
    y_data[i1].re =
        0.5F * ((temp1_re_tmp * d_y_re_tmp - temp1_im_tmp * c_y_re_tmp) +
                (temp2_re_tmp * b_y_re_tmp - -temp2_im_tmp * y_re_tmp));
    y_data[i1].im =
        0.5F * ((temp1_re_tmp * c_y_re_tmp + temp1_im_tmp * d_y_re_tmp) +
                (temp2_re_tmp * y_re_tmp + -temp2_im_tmp * b_y_re_tmp));
    temp1_re_tmp_tmp = reconVar1_data[b_i - 1].im;
    b_temp2_re_tmp = reconVar1_data[b_i - 1].re;
    b_temp1_re_tmp = reconVar2_data[b_i - 1].im;
    y_im_tmp = reconVar2_data[b_i - 1].re;
    y_data[b_i - 1].re =
        0.5F *
        ((temp2_re_tmp * b_temp2_re_tmp - temp2_im_tmp * temp1_re_tmp_tmp) +
         (temp1_re_tmp * y_im_tmp - -temp1_im_tmp * b_temp1_re_tmp));
    y_data[b_i - 1].im =
        0.5F *
        ((temp2_re_tmp * temp1_re_tmp_tmp + temp2_im_tmp * b_temp2_re_tmp) +
         (temp1_re_tmp * b_temp1_re_tmp + -temp1_im_tmp * y_im_tmp));
    b_i = (b_i + hnRows) - 1;
    y_data[b_i].re =
        0.5F *
        ((temp2_re_tmp * y_im_tmp - temp2_im_tmp * b_temp1_re_tmp) +
         (temp1_re_tmp * b_temp2_re_tmp - -temp1_im_tmp * temp1_re_tmp_tmp));
    y_data[b_i].im =
        0.5F *
        ((temp2_re_tmp * b_temp1_re_tmp + temp2_im_tmp * y_im_tmp) +
         (temp1_re_tmp * temp1_re_tmp_tmp + -temp1_im_tmp * b_temp2_re_tmp));
  }
  if (iterVar != 0) {
    temp1_re_tmp = y_data[iterVar].re;
    temp1_im_tmp = y_data[iterVar].im;
    y_re_tmp = reconVar1_data[iterVar].im;
    b_y_re_tmp = reconVar1_data[iterVar].re;
    c_y_re_tmp = temp1_re_tmp * b_y_re_tmp;
    y_im_tmp = temp1_re_tmp * y_re_tmp;
    d_y_re_tmp = reconVar2_data[iterVar].im;
    b_temp2_re_tmp = reconVar2_data[iterVar].re;
    b_temp1_re_tmp = temp1_re_tmp * b_temp2_re_tmp;
    temp1_re_tmp_tmp = temp1_re_tmp * d_y_re_tmp;
    y_data[iterVar].re = 0.5F * ((c_y_re_tmp - temp1_im_tmp * y_re_tmp) +
                                 (b_temp1_re_tmp - -temp1_im_tmp * d_y_re_tmp));
    y_data[iterVar].im =
        0.5F * ((y_im_tmp + temp1_im_tmp * b_y_re_tmp) +
                (temp1_re_tmp_tmp + -temp1_im_tmp * b_temp2_re_tmp));
    y_data[hnRows + iterVar].re =
        0.5F * ((b_temp1_re_tmp - temp1_im_tmp * d_y_re_tmp) +
                (c_y_re_tmp - -temp1_im_tmp * y_re_tmp));
    y_data[hnRows + iterVar].im =
        0.5F * ((temp1_re_tmp_tmp + temp1_im_tmp * b_temp2_re_tmp) +
                (y_im_tmp + -temp1_im_tmp * b_y_re_tmp));
  }
}

/*
 * Arguments    : const emxArray_real32_T *x
 *                int n2blue
 *                int nfft
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                const emxArray_real32_T *sintabinv
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void c_FFTImplementationCallback_dob(const emxArray_real32_T *x, int n2blue,
                                     int nfft, const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     const emxArray_real32_T *sintabinv,
                                     emxArray_creal32_T *y)
{
  emxArray_creal32_T *b_fv;
  emxArray_creal32_T *fv;
  emxArray_creal32_T *wwc;
  creal32_T *b_fv_data;
  creal32_T *fv_data;
  creal32_T *wwc_data;
  creal32_T *y_data;
  const float *x_data;
  float nt_im;
  int i;
  int k;
  int minNrowsNx;
  int nInt2m1;
  x_data = x->data;
  emxInit_creal32_T(&wwc, 1);
  if ((nfft != 1) && ((nfft & 1) == 0)) {
    int nInt2;
    int nRows;
    int rt;
    nRows = (int)((unsigned int)nfft >> 1);
    nInt2m1 = (nRows + nRows) - 1;
    i = wwc->size[0];
    wwc->size[0] = nInt2m1;
    emxEnsureCapacity_creal32_T(wwc, i);
    wwc_data = wwc->data;
    rt = 0;
    wwc_data[nRows - 1].re = 1.0F;
    wwc_data[nRows - 1].im = 0.0F;
    nInt2 = nRows << 1;
    for (k = 0; k <= nRows - 2; k++) {
      minNrowsNx = ((k + 1) << 1) - 1;
      if (nInt2 - rt <= minNrowsNx) {
        rt += minNrowsNx - nInt2;
      } else {
        rt += minNrowsNx;
      }
      nt_im = -3.14159274F * (float)rt / (float)nRows;
      wwc_data[(nRows - k) - 2].re = cosf(nt_im);
      wwc_data[(nRows - k) - 2].im = -sinf(nt_im);
    }
    i = nInt2m1 - 1;
    for (k = i; k >= nRows; k--) {
      wwc_data[k] = wwc_data[(nInt2m1 - k) - 1];
    }
  } else {
    int nInt2;
    int rt;
    nInt2m1 = (nfft + nfft) - 1;
    i = wwc->size[0];
    wwc->size[0] = nInt2m1;
    emxEnsureCapacity_creal32_T(wwc, i);
    wwc_data = wwc->data;
    rt = 0;
    wwc_data[nfft - 1].re = 1.0F;
    wwc_data[nfft - 1].im = 0.0F;
    nInt2 = nfft << 1;
    for (k = 0; k <= nfft - 2; k++) {
      minNrowsNx = ((k + 1) << 1) - 1;
      if (nInt2 - rt <= minNrowsNx) {
        rt += minNrowsNx - nInt2;
      } else {
        rt += minNrowsNx;
      }
      nt_im = -3.14159274F * (float)rt / (float)nfft;
      i = (nfft - k) - 2;
      wwc_data[i].re = cosf(nt_im);
      wwc_data[i].im = -sinf(nt_im);
    }
    i = nInt2m1 - 1;
    for (k = i; k >= nfft; k--) {
      wwc_data[k] = wwc_data[(nInt2m1 - k) - 1];
    }
  }
  i = y->size[0];
  y->size[0] = nfft;
  emxEnsureCapacity_creal32_T(y, i);
  y_data = y->data;
  if (nfft > x->size[0]) {
    i = y->size[0];
    y->size[0] = nfft;
    emxEnsureCapacity_creal32_T(y, i);
    y_data = y->data;
    for (i = 0; i < nfft; i++) {
      y_data[i].re = 0.0F;
      y_data[i].im = 0.0F;
    }
  }
  emxInit_creal32_T(&fv, 1);
  emxInit_creal32_T(&b_fv, 1);
  if ((n2blue != 1) && ((nfft & 1) == 0)) {
    d_FFTImplementationCallback_doH(x, y, x->size[0], nfft, n2blue, wwc, costab,
                                    sintab, costab, sintabinv);
  } else {
    float b_re_tmp;
    float c_re_tmp;
    float re_tmp;
    minNrowsNx = x->size[0];
    if (nfft <= minNrowsNx) {
      minNrowsNx = nfft;
    }
    for (k = 0; k < minNrowsNx; k++) {
      nInt2m1 = (nfft + k) - 1;
      y_data[k].re = wwc_data[nInt2m1].re * x_data[k];
      y_data[k].im = wwc_data[nInt2m1].im * -x_data[k];
    }
    i = minNrowsNx + 1;
    for (k = i; k <= nfft; k++) {
      y_data[k - 1].re = 0.0F;
      y_data[k - 1].im = 0.0F;
    }
    d_FFTImplementationCallback_r2b(y, n2blue, costab, sintab, fv);
    fv_data = fv->data;
    d_FFTImplementationCallback_r2b(wwc, n2blue, costab, sintab, b_fv);
    i = b_fv->size[0];
    b_fv->size[0] = fv->size[0];
    emxEnsureCapacity_creal32_T(b_fv, i);
    b_fv_data = b_fv->data;
    minNrowsNx = fv->size[0];
    for (i = 0; i < minNrowsNx; i++) {
      nt_im = fv_data[i].re;
      re_tmp = b_fv_data[i].im;
      b_re_tmp = fv_data[i].im;
      c_re_tmp = b_fv_data[i].re;
      b_fv_data[i].re = nt_im * c_re_tmp - b_re_tmp * re_tmp;
      b_fv_data[i].im = nt_im * re_tmp + b_re_tmp * c_re_tmp;
    }
    d_FFTImplementationCallback_r2b(b_fv, n2blue, costab, sintabinv, fv);
    fv_data = fv->data;
    if (fv->size[0] > 1) {
      nt_im = 1.0F / (float)fv->size[0];
      minNrowsNx = fv->size[0];
      for (i = 0; i < minNrowsNx; i++) {
        fv_data[i].re *= nt_im;
        fv_data[i].im *= nt_im;
      }
    }
    i = (int)(float)nfft;
    minNrowsNx = wwc->size[0];
    for (k = i; k <= minNrowsNx; k++) {
      nt_im = wwc_data[k - 1].re;
      re_tmp = fv_data[k - 1].im;
      b_re_tmp = wwc_data[k - 1].im;
      c_re_tmp = fv_data[k - 1].re;
      nInt2m1 = k - (int)(float)nfft;
      y_data[nInt2m1].re = nt_im * c_re_tmp + b_re_tmp * re_tmp;
      y_data[nInt2m1].im = nt_im * re_tmp - b_re_tmp * c_re_tmp;
    }
  }
  emxFree_creal32_T(&b_fv);
  emxFree_creal32_T(&fv);
  emxFree_creal32_T(&wwc);
}

/*
 * Arguments    : int nRows
 *                bool useRadix2
 *                emxArray_real32_T *costab
 *                emxArray_real32_T *sintab
 *                emxArray_real32_T *sintabinv
 * Return Type  : void
 */
void c_FFTImplementationCallback_gen(int nRows, bool useRadix2,
                                     emxArray_real32_T *costab,
                                     emxArray_real32_T *sintab,
                                     emxArray_real32_T *sintabinv)
{
  emxArray_real32_T *costab1q;
  float e;
  float *costab1q_data;
  float *costab_data;
  float *sintab_data;
  float *sintabinv_data;
  int i;
  int k;
  int n;
  int nd2;
  e = 6.28318548F / (float)nRows;
  n = (int)((unsigned int)nRows >> 1) >> 1;
  emxInit_real32_T(&costab1q, 2);
  i = costab1q->size[0] * costab1q->size[1];
  costab1q->size[0] = 1;
  costab1q->size[1] = n + 1;
  emxEnsureCapacity_real32_T(costab1q, i);
  costab1q_data = costab1q->data;
  costab1q_data[0] = 1.0F;
  nd2 = n / 2 - 1;
  for (k = 0; k <= nd2; k++) {
    costab1q_data[k + 1] = cosf(e * (float)(k + 1));
  }
  i = nd2 + 2;
  nd2 = n - 1;
  for (k = i; k <= nd2; k++) {
    costab1q_data[k] = sinf(e * (float)(n - k));
  }
  costab1q_data[n] = 0.0F;
  if (!useRadix2) {
    n = costab1q->size[1] - 1;
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
    for (k = 0; k < n; k++) {
      sintabinv_data[k + 1] = costab1q_data[(n - k) - 1];
    }
    i = costab1q->size[1];
    for (k = i; k <= nd2; k++) {
      sintabinv_data[k] = costab1q_data[k - n];
    }
    for (k = 0; k < n; k++) {
      costab_data[k + 1] = costab1q_data[k + 1];
      sintab_data[k + 1] = -costab1q_data[(n - k) - 1];
    }
    i = costab1q->size[1];
    for (k = i; k <= nd2; k++) {
      costab_data[k] = -costab1q_data[nd2 - k];
      sintab_data[k] = -costab1q_data[k - n];
    }
  } else {
    n = costab1q->size[1] - 1;
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
    for (k = 0; k < n; k++) {
      costab_data[k + 1] = costab1q_data[k + 1];
      sintab_data[k + 1] = -costab1q_data[(n - k) - 1];
    }
    i = costab1q->size[1];
    for (k = i; k <= nd2; k++) {
      costab_data[k] = -costab1q_data[nd2 - k];
      sintab_data[k] = -costab1q_data[k - n];
    }
    sintabinv->size[0] = 1;
    sintabinv->size[1] = 0;
  }
  emxFree_real32_T(&costab1q);
}

/*
 * Arguments    : int nfft
 *                bool useRadix2
 *                int *nRows
 * Return Type  : int
 */
int c_FFTImplementationCallback_get(int nfft, bool useRadix2, int *nRows)
{
  int n2blue;
  n2blue = 1;
  if (useRadix2) {
    *nRows = nfft;
  } else {
    if (nfft > 0) {
      int n;
      int pmax;
      n = (nfft + nfft) - 1;
      pmax = 31;
      if (n <= 1) {
        pmax = 0;
      } else {
        int pmin;
        bool exitg1;
        pmin = 0;
        exitg1 = false;
        while ((!exitg1) && (pmax - pmin > 1)) {
          int k;
          int pow2p;
          k = (pmin + pmax) >> 1;
          pow2p = 1 << k;
          if (pow2p == n) {
            pmax = k;
            exitg1 = true;
          } else if (pow2p > n) {
            pmax = k;
          } else {
            pmin = k;
          }
        }
      }
      n2blue = 1 << pmax;
    }
    *nRows = n2blue;
  }
  return n2blue;
}

/*
 * Arguments    : const emxArray_real32_T *x
 *                int unsigned_nRows
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void c_FFTImplementationCallback_r2b(const emxArray_real32_T *x,
                                     int unsigned_nRows,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     emxArray_creal32_T *y)
{
  creal32_T *y_data;
  const float *x_data;
  int u0;
  x_data = x->data;
  u0 = y->size[0];
  y->size[0] = unsigned_nRows;
  emxEnsureCapacity_creal32_T(y, u0);
  y_data = y->data;
  if (unsigned_nRows > x->size[0]) {
    u0 = y->size[0];
    y->size[0] = unsigned_nRows;
    emxEnsureCapacity_creal32_T(y, u0);
    y_data = y->data;
    for (u0 = 0; u0 < unsigned_nRows; u0++) {
      y_data[u0].re = 0.0F;
      y_data[u0].im = 0.0F;
    }
  }
  if (unsigned_nRows != 1) {
    c_FFTImplementationCallback_doH(x, y, unsigned_nRows, costab, sintab);
  } else {
    u0 = x->size[0];
    if (u0 > 1) {
      u0 = 1;
    }
    y_data[0].re = x_data[u0 - 1];
    y_data[0].im = 0.0F;
  }
}

/*
 * Arguments    : const emxArray_creal32_T *x
 *                int unsigned_nRows
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                emxArray_creal32_T *y
 * Return Type  : void
 */
void d_FFTImplementationCallback_r2b(const emxArray_creal32_T *x,
                                     int unsigned_nRows,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     emxArray_creal32_T *y)
{
  const creal32_T *x_data;
  creal32_T *y_data;
  const float *costab_data;
  const float *sintab_data;
  float temp_im;
  float temp_re;
  float temp_re_tmp;
  float twid_re;
  int i;
  int iDelta;
  int iDelta2;
  int iheight;
  int ihi;
  int iy;
  int j;
  int ju;
  int k;
  int nRowsD2;
  sintab_data = sintab->data;
  costab_data = costab->data;
  x_data = x->data;
  iy = y->size[0];
  y->size[0] = unsigned_nRows;
  emxEnsureCapacity_creal32_T(y, iy);
  y_data = y->data;
  if (unsigned_nRows > x->size[0]) {
    iy = y->size[0];
    y->size[0] = unsigned_nRows;
    emxEnsureCapacity_creal32_T(y, iy);
    y_data = y->data;
    for (iy = 0; iy < unsigned_nRows; iy++) {
      y_data[iy].re = 0.0F;
      y_data[iy].im = 0.0F;
    }
  }
  j = x->size[0];
  if (j > unsigned_nRows) {
    j = unsigned_nRows;
  }
  ihi = unsigned_nRows - 2;
  nRowsD2 = (int)((unsigned int)unsigned_nRows >> 1);
  k = nRowsD2 / 2;
  iy = 0;
  ju = 0;
  for (i = 0; i <= j - 2; i++) {
    bool tst;
    y_data[iy] = x_data[i];
    iy = unsigned_nRows;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }
    iy = ju;
  }
  y_data[iy] = x_data[j - 1];
  if (unsigned_nRows > 1) {
    for (i = 0; i <= ihi; i += 2) {
      temp_re_tmp = y_data[i + 1].re;
      temp_im = y_data[i + 1].im;
      temp_re = y_data[i].re;
      twid_re = y_data[i].im;
      y_data[i + 1].re = temp_re - temp_re_tmp;
      y_data[i + 1].im = twid_re - temp_im;
      y_data[i].re = temp_re + temp_re_tmp;
      y_data[i].im = twid_re + temp_im;
    }
  }
  iDelta = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      iy = i + iDelta;
      temp_re = y_data[iy].re;
      temp_im = y_data[iy].im;
      y_data[iy].re = y_data[i].re - temp_re;
      y_data[iy].im = y_data[i].im - temp_im;
      y_data[i].re += temp_re;
      y_data[i].im += temp_im;
    }
    iy = 1;
    for (j = k; j < nRowsD2; j += k) {
      float twid_im;
      twid_re = costab_data[j];
      twid_im = sintab_data[j];
      i = iy;
      ihi = iy + iheight;
      while (i < ihi) {
        ju = i + iDelta;
        temp_re_tmp = y_data[ju].im;
        temp_im = y_data[ju].re;
        temp_re = twid_re * temp_im - twid_im * temp_re_tmp;
        temp_im = twid_re * temp_re_tmp + twid_im * temp_im;
        y_data[ju].re = y_data[i].re - temp_re;
        y_data[ju].im = y_data[i].im - temp_im;
        y_data[i].re += temp_re;
        y_data[i].im += temp_im;
        i += iDelta2;
      }
      iy++;
    }
    k /= 2;
    iDelta = iDelta2;
    iDelta2 += iDelta2;
    iheight -= iDelta;
  }
}

/*
 * Arguments    : const emxArray_real32_T *x
 *                emxArray_creal32_T *y
 *                int nrowsx
 *                int nRows
 *                int nfft
 *                const emxArray_creal32_T *wwc
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                const emxArray_real32_T *costabinv
 *                const emxArray_real32_T *sintabinv
 * Return Type  : void
 */
void e_FFTImplementationCallback_doH(
    const emxArray_real32_T *x, emxArray_creal32_T *y, int nrowsx, int nRows,
    int nfft, const emxArray_creal32_T *wwc, const emxArray_real32_T *costab,
    const emxArray_real32_T *sintab, const emxArray_real32_T *costabinv,
    const emxArray_real32_T *sintabinv)
{
  emxArray_creal32_T *b_fv;
  emxArray_creal32_T *fv;
  emxArray_creal32_T *reconVar1;
  emxArray_creal32_T *reconVar2;
  emxArray_creal32_T *ytmp;
  emxArray_int32_T *wrapIndex;
  emxArray_real32_T *a__1;
  emxArray_real32_T *costable;
  emxArray_real32_T *hcostabinv;
  emxArray_real32_T *hsintab;
  emxArray_real32_T *hsintabinv;
  emxArray_real32_T *sintable;
  const creal32_T *wwc_data;
  creal32_T *b_fv_data;
  creal32_T *fv_data;
  creal32_T *reconVar1_data;
  creal32_T *reconVar2_data;
  creal32_T *y_data;
  creal32_T *ytmp_data;
  double z_tmp;
  const float *x_data;
  float a_im;
  float a_re;
  float ar;
  float b_im;
  float b_re;
  float *costable_data;
  float *sintable_data;
  int b_i;
  int hnRows;
  int i;
  int k1;
  int u0;
  int *wrapIndex_data;
  bool nxeven;
  wwc_data = wwc->data;
  y_data = y->data;
  x_data = x->data;
  hnRows = (int)((unsigned int)nRows >> 1);
  emxInit_creal32_T(&ytmp, 1);
  i = ytmp->size[0];
  ytmp->size[0] = hnRows;
  emxEnsureCapacity_creal32_T(ytmp, i);
  ytmp_data = ytmp->data;
  if (hnRows > nrowsx) {
    i = ytmp->size[0];
    ytmp->size[0] = hnRows;
    emxEnsureCapacity_creal32_T(ytmp, i);
    ytmp_data = ytmp->data;
    for (i = 0; i < hnRows; i++) {
      ytmp_data[i].re = 0.0F;
      ytmp_data[i].im = 0.0F;
    }
  }
  if ((x->size[0] & 1) == 0) {
    nxeven = true;
    u0 = x->size[0];
  } else if (x->size[0] >= nRows) {
    nxeven = true;
    u0 = nRows;
  } else {
    nxeven = false;
    u0 = x->size[0] - 1;
  }
  if (u0 > nRows) {
    u0 = nRows;
  }
  emxInit_real32_T(&a__1, 2);
  emxInit_real32_T(&costable, 2);
  emxInit_real32_T(&sintable, 2);
  d_FFTImplementationCallback_gen(nRows << 1, costable, sintable, a__1);
  sintable_data = sintable->data;
  costable_data = costable->data;
  emxInit_real32_T(&hsintab, 2);
  emxInit_real32_T(&hcostabinv, 2);
  emxInit_real32_T(&hsintabinv, 2);
  e_FFTImplementationCallback_get(costab, sintab, costabinv, sintabinv, a__1,
                                  hsintab, hcostabinv, hsintabinv);
  emxInit_creal32_T(&reconVar1, 1);
  i = reconVar1->size[0];
  reconVar1->size[0] = hnRows;
  emxEnsureCapacity_creal32_T(reconVar1, i);
  reconVar1_data = reconVar1->data;
  emxInit_creal32_T(&reconVar2, 1);
  i = reconVar2->size[0];
  reconVar2->size[0] = hnRows;
  emxEnsureCapacity_creal32_T(reconVar2, i);
  reconVar2_data = reconVar2->data;
  emxInit_int32_T(&wrapIndex, 2);
  i = wrapIndex->size[0] * wrapIndex->size[1];
  wrapIndex->size[0] = 1;
  wrapIndex->size[1] = hnRows;
  emxEnsureCapacity_int32_T(wrapIndex, i);
  wrapIndex_data = wrapIndex->data;
  for (b_i = 0; b_i < hnRows; b_i++) {
    i = b_i << 1;
    a_re = sintable_data[i];
    a_im = costable_data[i];
    reconVar1_data[b_i].re = 1.0F - a_re;
    reconVar1_data[b_i].im = -a_im;
    reconVar2_data[b_i].re = a_re + 1.0F;
    reconVar2_data[b_i].im = a_im;
    if (b_i + 1 != 1) {
      wrapIndex_data[b_i] = (hnRows - b_i) + 1;
    } else {
      wrapIndex_data[0] = 1;
    }
  }
  emxFree_real32_T(&sintable);
  emxFree_real32_T(&costable);
  z_tmp = (double)u0 / 2.0;
  i = (int)z_tmp;
  for (k1 = 0; k1 < i; k1++) {
    a_re = wwc_data[(hnRows + k1) - 1].re;
    a_im = wwc_data[(hnRows + k1) - 1].im;
    b_i = k1 << 1;
    b_re = x_data[b_i];
    b_im = x_data[b_i + 1];
    ytmp_data[k1].re = a_re * b_re + a_im * b_im;
    ytmp_data[k1].im = a_re * b_im - a_im * b_re;
  }
  if (!nxeven) {
    a_re = wwc_data[(hnRows + (int)z_tmp) - 1].re;
    a_im = wwc_data[(hnRows + (int)z_tmp) - 1].im;
    b_re = x_data[(int)z_tmp << 1];
    ytmp_data[(int)((double)u0 / 2.0)].re = a_re * b_re + a_im * 0.0F;
    ytmp_data[(int)((double)u0 / 2.0)].im = a_re * 0.0F - a_im * b_re;
    if ((int)z_tmp + 2 <= hnRows) {
      i = (int)z_tmp + 2;
      for (b_i = i; b_i <= hnRows; b_i++) {
        ytmp_data[b_i - 1].re = 0.0F;
        ytmp_data[b_i - 1].im = 0.0F;
      }
    }
  } else if ((int)z_tmp + 1 <= hnRows) {
    i = (int)z_tmp + 1;
    for (b_i = i; b_i <= hnRows; b_i++) {
      ytmp_data[b_i - 1].re = 0.0F;
      ytmp_data[b_i - 1].im = 0.0F;
    }
  }
  b_i = (int)((double)nfft / 2.0);
  emxInit_creal32_T(&fv, 1);
  d_FFTImplementationCallback_r2b(ytmp, b_i, a__1, hsintab, fv);
  fv_data = fv->data;
  emxInit_creal32_T(&b_fv, 1);
  d_FFTImplementationCallback_r2b(wwc, b_i, a__1, hsintab, b_fv);
  emxFree_real32_T(&hsintab);
  emxFree_real32_T(&a__1);
  i = b_fv->size[0];
  b_fv->size[0] = fv->size[0];
  emxEnsureCapacity_creal32_T(b_fv, i);
  b_fv_data = b_fv->data;
  u0 = fv->size[0];
  for (i = 0; i < u0; i++) {
    a_re = fv_data[i].re;
    a_im = b_fv_data[i].im;
    b_re = fv_data[i].im;
    b_im = b_fv_data[i].re;
    b_fv_data[i].re = a_re * b_im - b_re * a_im;
    b_fv_data[i].im = a_re * a_im + b_re * b_im;
  }
  d_FFTImplementationCallback_r2b(b_fv, b_i, hcostabinv, hsintabinv, fv);
  fv_data = fv->data;
  emxFree_creal32_T(&b_fv);
  emxFree_real32_T(&hsintabinv);
  emxFree_real32_T(&hcostabinv);
  if (fv->size[0] > 1) {
    a_re = 1.0F / (float)fv->size[0];
    u0 = fv->size[0];
    for (i = 0; i < u0; i++) {
      fv_data[i].re *= a_re;
      fv_data[i].im *= a_re;
    }
  }
  i = (int)(float)hnRows;
  b_i = wwc->size[0];
  for (u0 = i; u0 <= b_i; u0++) {
    a_re = wwc_data[u0 - 1].re;
    a_im = fv_data[u0 - 1].im;
    b_re = wwc_data[u0 - 1].im;
    b_im = fv_data[u0 - 1].re;
    ar = a_re * b_im + b_re * a_im;
    a_re = a_re * a_im - b_re * b_im;
    if (a_re == 0.0F) {
      ytmp_data[u0 - (int)(float)hnRows].re = ar / (float)hnRows;
      ytmp_data[u0 - (int)(float)hnRows].im = 0.0F;
    } else if (ar == 0.0F) {
      ytmp_data[u0 - (int)(float)hnRows].re = 0.0F;
      ytmp_data[u0 - (int)(float)hnRows].im = a_re / (float)hnRows;
    } else {
      ytmp_data[u0 - (int)(float)hnRows].re = ar / (float)hnRows;
      ytmp_data[u0 - (int)(float)hnRows].im = a_re / (float)hnRows;
    }
  }
  emxFree_creal32_T(&fv);
  for (b_i = 0; b_i < hnRows; b_i++) {
    float b_ytmp_re_tmp;
    float ytmp_im_tmp;
    float ytmp_re_tmp;
    i = wrapIndex_data[b_i];
    a_re = ytmp_data[b_i].re;
    a_im = reconVar1_data[b_i].im;
    b_re = ytmp_data[b_i].im;
    b_im = reconVar1_data[b_i].re;
    ar = ytmp_data[i - 1].re;
    ytmp_im_tmp = -ytmp_data[i - 1].im;
    ytmp_re_tmp = reconVar2_data[b_i].im;
    b_ytmp_re_tmp = reconVar2_data[b_i].re;
    y_data[b_i].re = 0.5F * ((a_re * b_im - b_re * a_im) +
                             (ar * b_ytmp_re_tmp - ytmp_im_tmp * ytmp_re_tmp));
    y_data[b_i].im = 0.5F * ((a_re * a_im + b_re * b_im) +
                             (ar * ytmp_re_tmp + ytmp_im_tmp * b_ytmp_re_tmp));
    y_data[hnRows + b_i].re =
        0.5F * ((a_re * b_ytmp_re_tmp - b_re * ytmp_re_tmp) +
                (ar * b_im - ytmp_im_tmp * a_im));
    y_data[hnRows + b_i].im =
        0.5F * ((a_re * ytmp_re_tmp + b_re * b_ytmp_re_tmp) +
                (ar * a_im + ytmp_im_tmp * b_im));
  }
  emxFree_creal32_T(&reconVar2);
  emxFree_creal32_T(&reconVar1);
  emxFree_int32_T(&wrapIndex);
  emxFree_creal32_T(&ytmp);
  u0 = y->size[0];
  for (i = 0; i < u0; i++) {
    ar = y_data[i].re;
    a_re = y_data[i].im;
    if (a_re == 0.0F) {
      a_im = ar / 2.0F;
      a_re = 0.0F;
    } else if (ar == 0.0F) {
      a_im = 0.0F;
      a_re /= 2.0F;
    } else {
      a_im = ar / 2.0F;
      a_re /= 2.0F;
    }
    y_data[i].re = a_im;
    y_data[i].im = a_re;
  }
}

/*
 * Arguments    : float x
 *                emxArray_creal32_T *y
 *                int unsigned_nRows
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 * Return Type  : void
 */
void f_FFTImplementationCallback_doH(float x, emxArray_creal32_T *y,
                                     int unsigned_nRows,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab)
{
  emxArray_creal32_T *reconVar1;
  emxArray_creal32_T *reconVar2;
  emxArray_int32_T *bitrevIndex;
  emxArray_int32_T *wrapIndex;
  emxArray_real32_T *hcostab;
  emxArray_real32_T *hsintab;
  creal32_T *reconVar1_data;
  creal32_T *reconVar2_data;
  creal32_T *y_data;
  double z;
  const float *costab_data;
  const float *sintab_data;
  float im;
  float re;
  float temp_im;
  float temp_re;
  float twid_im;
  float twid_re;
  float *hcostab_data;
  float *hsintab_data;
  int hszCostab;
  int i;
  int iDelta;
  int iDelta2;
  int iheight;
  int ihi;
  int istart;
  int k;
  int nRows;
  int nRowsD2;
  int *wrapIndex_data;
  bool nxeven;
  sintab_data = sintab->data;
  costab_data = costab->data;
  y_data = y->data;
  nRows = (int)((unsigned int)unsigned_nRows >> 1);
  iDelta = nRows - 2;
  nRowsD2 = nRows / 2;
  k = nRowsD2 / 2;
  hszCostab = (int)((unsigned int)costab->size[1] >> 1);
  emxInit_real32_T(&hcostab, 2);
  ihi = hcostab->size[0] * hcostab->size[1];
  hcostab->size[0] = 1;
  hcostab->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(hcostab, ihi);
  hcostab_data = hcostab->data;
  emxInit_real32_T(&hsintab, 2);
  ihi = hsintab->size[0] * hsintab->size[1];
  hsintab->size[0] = 1;
  hsintab->size[1] = hszCostab;
  emxEnsureCapacity_real32_T(hsintab, ihi);
  hsintab_data = hsintab->data;
  for (i = 0; i < hszCostab; i++) {
    istart = ((i + 1) << 1) - 2;
    hcostab_data[i] = costab_data[istart];
    hsintab_data[i] = sintab_data[istart];
  }
  emxInit_creal32_T(&reconVar1, 1);
  ihi = reconVar1->size[0];
  reconVar1->size[0] = nRows;
  emxEnsureCapacity_creal32_T(reconVar1, ihi);
  reconVar1_data = reconVar1->data;
  emxInit_creal32_T(&reconVar2, 1);
  ihi = reconVar2->size[0];
  reconVar2->size[0] = nRows;
  emxEnsureCapacity_creal32_T(reconVar2, ihi);
  reconVar2_data = reconVar2->data;
  emxInit_int32_T(&wrapIndex, 2);
  ihi = wrapIndex->size[0] * wrapIndex->size[1];
  wrapIndex->size[0] = 1;
  wrapIndex->size[1] = nRows;
  emxEnsureCapacity_int32_T(wrapIndex, ihi);
  wrapIndex_data = wrapIndex->data;
  for (i = 0; i < nRows; i++) {
    re = sintab_data[i];
    im = costab_data[i];
    reconVar1_data[i].re = re + 1.0F;
    reconVar1_data[i].im = -im;
    reconVar2_data[i].re = 1.0F - re;
    reconVar2_data[i].im = im;
    if (i + 1 != 1) {
      wrapIndex_data[i] = (nRows - i) + 1;
    } else {
      wrapIndex_data[0] = 1;
    }
  }
  z = (double)unsigned_nRows / 2.0;
  emxInit_int32_T(&bitrevIndex, 1);
  istart = y->size[0];
  if (istart > nRows) {
    istart = nRows;
  }
  d_FFTImplementationCallback_get(istart - 1, (int)z, bitrevIndex);
  wrapIndex_data = bitrevIndex->data;
  if (unsigned_nRows <= 1) {
    nxeven = true;
    istart = 1;
  } else {
    nxeven = false;
    istart = 0;
  }
  ihi = (int)((double)istart / 2.0);
  for (i = 0; i < ihi; i++) {
    y_data[wrapIndex_data[0] - 1].re = x;
    y_data[wrapIndex_data[0] - 1].im = x;
  }
  if (!nxeven) {
    ihi = wrapIndex_data[(int)((double)istart / 2.0)] - 1;
    y_data[ihi].re = x;
    y_data[ihi].im = 0.0F;
  }
  emxFree_int32_T(&bitrevIndex);
  if (nRows > 1) {
    for (i = 0; i <= iDelta; i += 2) {
      re = y_data[i + 1].re;
      im = y_data[i + 1].im;
      temp_re = re;
      temp_im = im;
      twid_re = y_data[i].re;
      twid_im = y_data[i].im;
      re = twid_re - re;
      im = twid_im - im;
      y_data[i + 1].re = re;
      y_data[i + 1].im = im;
      twid_re += temp_re;
      twid_im += temp_im;
      y_data[i].re = twid_re;
      y_data[i].im = twid_im;
    }
  }
  iDelta = 2;
  iDelta2 = 4;
  iheight = ((k - 1) << 2) + 1;
  while (k > 0) {
    for (i = 0; i < iheight; i += iDelta2) {
      istart = i + iDelta;
      temp_re = y_data[istart].re;
      temp_im = y_data[istart].im;
      y_data[istart].re = y_data[i].re - temp_re;
      y_data[istart].im = y_data[i].im - temp_im;
      y_data[i].re += temp_re;
      y_data[i].im += temp_im;
    }
    istart = 1;
    for (hszCostab = k; hszCostab < nRowsD2; hszCostab += k) {
      twid_re = hcostab_data[hszCostab];
      twid_im = hsintab_data[hszCostab];
      i = istart;
      ihi = istart + iheight;
      while (i < ihi) {
        nRows = i + iDelta;
        re = y_data[nRows].im;
        im = y_data[nRows].re;
        temp_re = twid_re * im - twid_im * re;
        temp_im = twid_re * re + twid_im * im;
        y_data[nRows].re = y_data[i].re - temp_re;
        y_data[nRows].im = y_data[i].im - temp_im;
        y_data[i].re += temp_re;
        y_data[i].im += temp_im;
        i += iDelta2;
      }
      istart++;
    }
    k /= 2;
    iDelta = iDelta2;
    iDelta2 += iDelta2;
    iheight -= iDelta;
  }
  emxFree_real32_T(&hsintab);
  emxFree_real32_T(&hcostab);
  f_FFTImplementationCallback_get(y, reconVar1, reconVar2, wrapIndex, (int)z);
  emxFree_creal32_T(&reconVar2);
  emxFree_creal32_T(&reconVar1);
  emxFree_int32_T(&wrapIndex);
}

/*
 * Arguments    : float x
 *                emxArray_creal32_T *y
 *                int nRows
 *                int nfft
 *                const emxArray_creal32_T *wwc
 *                const emxArray_real32_T *costab
 *                const emxArray_real32_T *sintab
 *                const emxArray_real32_T *costabinv
 *                const emxArray_real32_T *sintabinv
 * Return Type  : void
 */
void g_FFTImplementationCallback_doH(float x, emxArray_creal32_T *y, int nRows,
                                     int nfft, const emxArray_creal32_T *wwc,
                                     const emxArray_real32_T *costab,
                                     const emxArray_real32_T *sintab,
                                     const emxArray_real32_T *costabinv,
                                     const emxArray_real32_T *sintabinv)
{
  emxArray_creal32_T *b_fv;
  emxArray_creal32_T *fv;
  emxArray_creal32_T *reconVar1;
  emxArray_creal32_T *reconVar2;
  emxArray_creal32_T *ytmp;
  emxArray_int32_T *wrapIndex;
  emxArray_real32_T *a__1;
  emxArray_real32_T *costable;
  emxArray_real32_T *hcostabinv;
  emxArray_real32_T *hsintab;
  emxArray_real32_T *hsintabinv;
  emxArray_real32_T *sintable;
  const creal32_T *wwc_data;
  creal32_T *b_fv_data;
  creal32_T *fv_data;
  creal32_T *reconVar1_data;
  creal32_T *reconVar2_data;
  creal32_T *y_data;
  creal32_T *ytmp_data;
  double z_tmp;
  float a_im;
  float a_re;
  float b_re_tmp;
  float re_tmp;
  float *costable_data;
  float *sintable_data;
  int b_i;
  int hnRows;
  int i;
  int minHnrowsNx;
  int *wrapIndex_data;
  bool nxeven;
  wwc_data = wwc->data;
  y_data = y->data;
  hnRows = (int)((unsigned int)nRows >> 1);
  emxInit_creal32_T(&ytmp, 1);
  i = ytmp->size[0];
  ytmp->size[0] = hnRows;
  emxEnsureCapacity_creal32_T(ytmp, i);
  ytmp_data = ytmp->data;
  if (hnRows > 1) {
    i = ytmp->size[0];
    ytmp->size[0] = hnRows;
    emxEnsureCapacity_creal32_T(ytmp, i);
    ytmp_data = ytmp->data;
    for (i = 0; i < hnRows; i++) {
      ytmp_data[i].re = 0.0F;
      ytmp_data[i].im = 0.0F;
    }
  }
  if (nRows <= 1) {
    nxeven = true;
    minHnrowsNx = 1;
  } else {
    nxeven = false;
    minHnrowsNx = 0;
  }
  emxInit_real32_T(&a__1, 2);
  emxInit_real32_T(&costable, 2);
  emxInit_real32_T(&sintable, 2);
  d_FFTImplementationCallback_gen(nRows << 1, costable, sintable, a__1);
  sintable_data = sintable->data;
  costable_data = costable->data;
  emxInit_real32_T(&hsintab, 2);
  emxInit_real32_T(&hcostabinv, 2);
  emxInit_real32_T(&hsintabinv, 2);
  e_FFTImplementationCallback_get(costab, sintab, costabinv, sintabinv, a__1,
                                  hsintab, hcostabinv, hsintabinv);
  emxInit_creal32_T(&reconVar1, 1);
  i = reconVar1->size[0];
  reconVar1->size[0] = hnRows;
  emxEnsureCapacity_creal32_T(reconVar1, i);
  reconVar1_data = reconVar1->data;
  emxInit_creal32_T(&reconVar2, 1);
  i = reconVar2->size[0];
  reconVar2->size[0] = hnRows;
  emxEnsureCapacity_creal32_T(reconVar2, i);
  reconVar2_data = reconVar2->data;
  emxInit_int32_T(&wrapIndex, 2);
  i = wrapIndex->size[0] * wrapIndex->size[1];
  wrapIndex->size[0] = 1;
  wrapIndex->size[1] = hnRows;
  emxEnsureCapacity_int32_T(wrapIndex, i);
  wrapIndex_data = wrapIndex->data;
  for (b_i = 0; b_i < hnRows; b_i++) {
    i = b_i << 1;
    a_re = sintable_data[i];
    a_im = costable_data[i];
    reconVar1_data[b_i].re = a_re + 1.0F;
    reconVar1_data[b_i].im = -a_im;
    reconVar2_data[b_i].re = 1.0F - a_re;
    reconVar2_data[b_i].im = a_im;
    if (b_i + 1 != 1) {
      wrapIndex_data[b_i] = (hnRows - b_i) + 1;
    } else {
      wrapIndex_data[0] = 1;
    }
  }
  emxFree_real32_T(&sintable);
  emxFree_real32_T(&costable);
  z_tmp = (double)minHnrowsNx / 2.0;
  if ((int)z_tmp - 1 >= 0) {
    a_re = wwc_data[hnRows - 1].re * x;
    a_im = wwc_data[hnRows - 1].im * x;
    ytmp_data[0].re = a_re + a_im;
    ytmp_data[0].im = a_re - a_im;
  }
  if (!nxeven) {
    a_re = wwc_data[(hnRows + (int)z_tmp) - 1].re;
    a_im = wwc_data[(hnRows + (int)z_tmp) - 1].im;
    ytmp_data[(int)((double)minHnrowsNx / 2.0)].re = a_re * x + a_im * 0.0F;
    ytmp_data[(int)((double)minHnrowsNx / 2.0)].im = a_re * 0.0F - a_im * x;
    if ((int)z_tmp + 2 <= hnRows) {
      i = (int)z_tmp + 2;
      for (b_i = i; b_i <= hnRows; b_i++) {
        ytmp_data[b_i - 1].re = 0.0F;
        ytmp_data[b_i - 1].im = 0.0F;
      }
    }
  } else if ((int)z_tmp + 1 <= hnRows) {
    i = (int)z_tmp + 1;
    for (b_i = i; b_i <= hnRows; b_i++) {
      ytmp_data[b_i - 1].re = 0.0F;
      ytmp_data[b_i - 1].im = 0.0F;
    }
  }
  minHnrowsNx = (int)((double)nfft / 2.0);
  emxInit_creal32_T(&fv, 1);
  d_FFTImplementationCallback_r2b(ytmp, minHnrowsNx, a__1, hsintab, fv);
  fv_data = fv->data;
  emxInit_creal32_T(&b_fv, 1);
  d_FFTImplementationCallback_r2b(wwc, minHnrowsNx, a__1, hsintab, b_fv);
  emxFree_real32_T(&hsintab);
  emxFree_real32_T(&a__1);
  i = b_fv->size[0];
  b_fv->size[0] = fv->size[0];
  emxEnsureCapacity_creal32_T(b_fv, i);
  b_fv_data = b_fv->data;
  b_i = fv->size[0];
  for (i = 0; i < b_i; i++) {
    a_re = fv_data[i].re;
    a_im = b_fv_data[i].im;
    re_tmp = fv_data[i].im;
    b_re_tmp = b_fv_data[i].re;
    b_fv_data[i].re = a_re * b_re_tmp - re_tmp * a_im;
    b_fv_data[i].im = a_re * a_im + re_tmp * b_re_tmp;
  }
  d_FFTImplementationCallback_r2b(b_fv, minHnrowsNx, hcostabinv, hsintabinv,
                                  fv);
  fv_data = fv->data;
  emxFree_creal32_T(&b_fv);
  emxFree_real32_T(&hsintabinv);
  emxFree_real32_T(&hcostabinv);
  if (fv->size[0] > 1) {
    a_re = 1.0F / (float)fv->size[0];
    b_i = fv->size[0];
    for (i = 0; i < b_i; i++) {
      fv_data[i].re *= a_re;
      fv_data[i].im *= a_re;
    }
  }
  i = (int)(float)hnRows;
  minHnrowsNx = wwc->size[0];
  for (b_i = i; b_i <= minHnrowsNx; b_i++) {
    a_re = wwc_data[b_i - 1].re;
    a_im = fv_data[b_i - 1].im;
    re_tmp = wwc_data[b_i - 1].im;
    b_re_tmp = fv_data[b_i - 1].re;
    ytmp_data[b_i - (int)(float)hnRows].re = a_re * b_re_tmp + re_tmp * a_im;
    ytmp_data[b_i - (int)(float)hnRows].im = a_re * a_im - re_tmp * b_re_tmp;
  }
  emxFree_creal32_T(&fv);
  for (b_i = 0; b_i < hnRows; b_i++) {
    float b_ytmp_re_tmp;
    float c_ytmp_re_tmp;
    float ytmp_im_tmp;
    float ytmp_re_tmp;
    i = wrapIndex_data[b_i];
    a_re = ytmp_data[b_i].re;
    a_im = reconVar1_data[b_i].im;
    re_tmp = ytmp_data[b_i].im;
    b_re_tmp = reconVar1_data[b_i].re;
    ytmp_re_tmp = ytmp_data[i - 1].re;
    ytmp_im_tmp = -ytmp_data[i - 1].im;
    b_ytmp_re_tmp = reconVar2_data[b_i].im;
    c_ytmp_re_tmp = reconVar2_data[b_i].re;
    y_data[b_i].re =
        0.5F * ((a_re * b_re_tmp - re_tmp * a_im) +
                (ytmp_re_tmp * c_ytmp_re_tmp - ytmp_im_tmp * b_ytmp_re_tmp));
    y_data[b_i].im =
        0.5F * ((a_re * a_im + re_tmp * b_re_tmp) +
                (ytmp_re_tmp * b_ytmp_re_tmp + ytmp_im_tmp * c_ytmp_re_tmp));
    y_data[hnRows + b_i].re =
        0.5F * ((a_re * c_ytmp_re_tmp - re_tmp * b_ytmp_re_tmp) +
                (ytmp_re_tmp * b_re_tmp - ytmp_im_tmp * a_im));
    y_data[hnRows + b_i].im =
        0.5F * ((a_re * b_ytmp_re_tmp + re_tmp * c_ytmp_re_tmp) +
                (ytmp_re_tmp * a_im + ytmp_im_tmp * b_re_tmp));
  }
  emxFree_creal32_T(&reconVar2);
  emxFree_creal32_T(&reconVar1);
  emxFree_int32_T(&wrapIndex);
  emxFree_creal32_T(&ytmp);
}

/*
 * File trailer for FFTImplementationCallback.c
 *
 * [EOF]
 */
