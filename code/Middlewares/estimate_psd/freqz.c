/*
 * File: freqz.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 18-Sep-2024 13:56:03
 */

/* Include Files */
#include "freqz.h"
#include "FFTImplementationCallback.h"
#include "estimate_psd_data.h"
#include "estimate_psd_emxutil.h"
#include "estimate_psd_types.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : emxArray_creal32_T *in1
 *                const emxArray_creal32_T *in2
 *                const emxArray_creal32_T *in3
 * Return Type  : void
 */
void binary_expand_op(emxArray_creal32_T *in1, const emxArray_creal32_T *in2,
                      const emxArray_creal32_T *in3)
{
  const creal32_T *in2_data;
  const creal32_T *in3_data;
  creal32_T *in1_data;
  int i;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  in3_data = in3->data;
  in2_data = in2->data;
  if (in3->size[1] == 1) {
    loop_ub = in2->size[1];
  } else {
    loop_ub = in3->size[1];
  }
  i = in1->size[0];
  in1->size[0] = loop_ub;
  emxEnsureCapacity_creal32_T(in1, i);
  in1_data = in1->data;
  stride_0_0 = (in2->size[1] != 1);
  stride_1_0 = (in3->size[1] != 1);
  for (i = 0; i < loop_ub; i++) {
    float ai;
    float ar;
    float bi;
    float br;
    float im;
    float re;
    int ar_tmp;
    ar_tmp = i * stride_0_0;
    ar = in2_data[ar_tmp].re;
    ai = in2_data[ar_tmp].im;
    ar_tmp = i * stride_1_0;
    br = in3_data[ar_tmp].re;
    bi = in3_data[ar_tmp].im;
    if (bi == 0.0F) {
      if (ai == 0.0F) {
        re = ar / br;
        im = 0.0F;
      } else if (ar == 0.0F) {
        re = 0.0F;
        im = ai / br;
      } else {
        re = ar / br;
        im = ai / br;
      }
    } else if (br == 0.0F) {
      if (ar == 0.0F) {
        re = ai / bi;
        im = 0.0F;
      } else if (ai == 0.0F) {
        re = 0.0F;
        im = -(ar / bi);
      } else {
        re = ai / bi;
        im = -(ar / bi);
      }
    } else {
      float brm;
      brm = fabsf(br);
      im = fabsf(bi);
      if (brm > im) {
        float s;
        s = bi / br;
        im = br + s * bi;
        re = (ar + s * ai) / im;
        im = (ai - s * ar) / im;
      } else if (im == brm) {
        float s;
        if (br > 0.0F) {
          s = 0.5F;
        } else {
          s = -0.5F;
        }
        if (bi > 0.0F) {
          im = 0.5F;
        } else {
          im = -0.5F;
        }
        re = (ar * s + ai * im) / brm;
        im = (ai * s - ar * im) / brm;
      } else {
        float s;
        s = br / bi;
        im = bi + s * br;
        re = (s * ar + ai) / im;
        im = (s * ai - ar) / im;
      }
    }
    in1_data[i].re = re;
    in1_data[i].im = im;
  }
}

/*
 * Arguments    : float bin
 *                double npoints
 *                emxArray_creal32_T *h
 * Return Type  : void
 */
void firfreqz(float bin, double npoints, emxArray_creal32_T *h)
{
  emxArray_creal32_T *b_fv;
  emxArray_creal32_T *fv;
  emxArray_creal32_T *wwc;
  emxArray_creal32_T *yCol;
  emxArray_real32_T *costab;
  emxArray_real32_T *sintab;
  emxArray_real32_T *sintabinv;
  creal32_T *b_fv_data;
  creal32_T *fv_data;
  creal32_T *wwc_data;
  creal32_T *yCol_data;
  float *costab_data;
  float *sintab_data;
  int i;
  int iheight;
  int j;
  int k;
  if (!(npoints < 1.0)) {
    int iDelta2;
    int len_tmp_tmp_tmp;
    bool useRadix2;
    len_tmp_tmp_tmp = (int)npoints;
    useRadix2 = (((int)npoints & ((int)npoints - 1)) == 0);
    iDelta2 = c_FFTImplementationCallback_get((int)npoints, useRadix2, &j);
    emxInit_real32_T(&costab, 2);
    emxInit_real32_T(&sintab, 2);
    emxInit_real32_T(&sintabinv, 2);
    c_FFTImplementationCallback_gen(j, useRadix2, costab, sintab, sintabinv);
    sintab_data = sintab->data;
    costab_data = costab->data;
    emxInit_creal32_T(&yCol, 1);
    if (useRadix2) {
      iheight = yCol->size[0];
      yCol->size[0] = (int)npoints;
      emxEnsureCapacity_creal32_T(yCol, iheight);
      yCol_data = yCol->data;
      if ((int)npoints > 1) {
        iheight = yCol->size[0];
        yCol->size[0] = (int)npoints;
        emxEnsureCapacity_creal32_T(yCol, iheight);
        yCol_data = yCol->data;
        for (iheight = 0; iheight < len_tmp_tmp_tmp; iheight++) {
          yCol_data[iheight].re = 0.0F;
          yCol_data[iheight].im = 0.0F;
        }
      }
      if ((int)npoints != 1) {
        f_FFTImplementationCallback_doH(bin, yCol, (int)npoints, costab,
                                        sintab);
        yCol_data = yCol->data;
      } else {
        float nt_im;
        float nt_re;
        float nt_re_tmp;
        float twid_im;
        int istart;
        int nInt2;
        int nRowsD2;
        istart = (int)npoints - 2;
        nRowsD2 = (int)((unsigned int)(int)npoints >> 1);
        k = nRowsD2 / 2;
        yCol_data[0].re = bin;
        yCol_data[0].im = 0.0F;
        if ((int)npoints > 1) {
          for (i = 0; i <= istart; i += 2) {
            nt_re_tmp = yCol_data[i + 1].re;
            nt_im = yCol_data[i + 1].im;
            twid_im = yCol_data[i].re;
            nt_re = yCol_data[i].im;
            yCol_data[i + 1].re = twid_im - nt_re_tmp;
            yCol_data[i + 1].im = nt_re - nt_im;
            yCol_data[i].re = twid_im + nt_re_tmp;
            yCol_data[i].im = nt_re + nt_im;
          }
        }
        nInt2 = 2;
        iDelta2 = 4;
        iheight = ((k - 1) << 2) + 1;
        while (k > 0) {
          for (i = 0; i < iheight; i += iDelta2) {
            j = i + nInt2;
            nt_re = yCol_data[j].re;
            nt_im = yCol_data[j].im;
            yCol_data[j].re = yCol_data[i].re - nt_re;
            yCol_data[j].im = yCol_data[i].im - nt_im;
            yCol_data[i].re += nt_re;
            yCol_data[i].im += nt_im;
          }
          istart = 1;
          for (j = k; j < nRowsD2; j += k) {
            float twid_re;
            int nInt2m1;
            twid_re = costab_data[j];
            twid_im = sintab_data[j];
            i = istart;
            nInt2m1 = istart + iheight;
            while (i < nInt2m1) {
              int rt;
              rt = i + nInt2;
              nt_re_tmp = yCol_data[rt].im;
              nt_im = yCol_data[rt].re;
              nt_re = twid_re * nt_im - twid_im * nt_re_tmp;
              nt_im = twid_re * nt_re_tmp + twid_im * nt_im;
              yCol_data[rt].re = yCol_data[i].re - nt_re;
              yCol_data[rt].im = yCol_data[i].im - nt_im;
              yCol_data[i].re += nt_re;
              yCol_data[i].im += nt_im;
              i += iDelta2;
            }
            istart++;
          }
          k /= 2;
          nInt2 = iDelta2;
          iDelta2 += iDelta2;
          iheight -= nInt2;
        }
      }
    } else {
      float nt_im;
      int istart;
      emxInit_creal32_T(&wwc, 1);
      if (((int)npoints != 1) && (((int)npoints & 1) == 0)) {
        int nInt2;
        int nInt2m1;
        int rt;
        j = (int)((unsigned int)(int)npoints >> 1);
        nInt2m1 = (j + j) - 1;
        iheight = wwc->size[0];
        wwc->size[0] = nInt2m1;
        emxEnsureCapacity_creal32_T(wwc, iheight);
        wwc_data = wwc->data;
        rt = 0;
        wwc_data[j - 1].re = 1.0F;
        wwc_data[j - 1].im = 0.0F;
        nInt2 = j << 1;
        for (k = 0; k <= j - 2; k++) {
          istart = ((k + 1) << 1) - 1;
          if (nInt2 - rt <= istart) {
            rt += istart - nInt2;
          } else {
            rt += istart;
          }
          nt_im = -3.14159274F * (float)rt / (float)j;
          iheight = (j - k) - 2;
          wwc_data[iheight].re = cosf(nt_im);
          wwc_data[iheight].im = -sinf(nt_im);
        }
        iheight = nInt2m1 - 1;
        for (k = iheight; k >= j; k--) {
          wwc_data[k] = wwc_data[(nInt2m1 - k) - 1];
        }
      } else {
        int nInt2;
        int nInt2m1;
        int rt;
        nInt2m1 = ((int)npoints + (int)npoints) - 1;
        iheight = wwc->size[0];
        wwc->size[0] = nInt2m1;
        emxEnsureCapacity_creal32_T(wwc, iheight);
        wwc_data = wwc->data;
        rt = 0;
        wwc_data[(int)npoints - 1].re = 1.0F;
        wwc_data[(int)npoints - 1].im = 0.0F;
        nInt2 = (int)npoints << 1;
        for (k = 0; k <= len_tmp_tmp_tmp - 2; k++) {
          istart = ((k + 1) << 1) - 1;
          if (nInt2 - rt <= istart) {
            rt += istart - nInt2;
          } else {
            rt += istart;
          }
          nt_im = -3.14159274F * (float)rt / (float)(int)npoints;
          iheight = ((int)npoints - k) - 2;
          wwc_data[iheight].re = cosf(nt_im);
          wwc_data[iheight].im = -sinf(nt_im);
        }
        iheight = nInt2m1 - 1;
        for (k = iheight; k >= len_tmp_tmp_tmp; k--) {
          wwc_data[k] = wwc_data[(nInt2m1 - k) - 1];
        }
      }
      iheight = yCol->size[0];
      yCol->size[0] = (int)npoints;
      emxEnsureCapacity_creal32_T(yCol, iheight);
      yCol_data = yCol->data;
      if ((int)npoints > 1) {
        iheight = yCol->size[0];
        yCol->size[0] = (int)npoints;
        emxEnsureCapacity_creal32_T(yCol, iheight);
        yCol_data = yCol->data;
        for (iheight = 0; iheight < len_tmp_tmp_tmp; iheight++) {
          yCol_data[iheight].re = 0.0F;
          yCol_data[iheight].im = 0.0F;
        }
      }
      emxInit_creal32_T(&fv, 1);
      emxInit_creal32_T(&b_fv, 1);
      if ((iDelta2 != 1) && (((int)npoints & 1) == 0)) {
        g_FFTImplementationCallback_doH(bin, yCol, (int)npoints, iDelta2, wwc,
                                        costab, sintab, costab, sintabinv);
        yCol_data = yCol->data;
      } else {
        float nt_re;
        float twid_im;
        float twid_re;
        yCol_data[0].re = wwc_data[(int)npoints - 1].re * bin;
        yCol_data[0].im = wwc_data[(int)npoints - 1].im * -bin;
        for (k = 2; k <= len_tmp_tmp_tmp; k++) {
          yCol_data[k - 1].re = 0.0F;
          yCol_data[k - 1].im = 0.0F;
        }
        d_FFTImplementationCallback_r2b(yCol, iDelta2, costab, sintab, fv);
        fv_data = fv->data;
        d_FFTImplementationCallback_r2b(wwc, iDelta2, costab, sintab, b_fv);
        iheight = b_fv->size[0];
        b_fv->size[0] = fv->size[0];
        emxEnsureCapacity_creal32_T(b_fv, iheight);
        b_fv_data = b_fv->data;
        istart = fv->size[0];
        for (iheight = 0; iheight < istart; iheight++) {
          twid_im = fv_data[iheight].re;
          nt_im = b_fv_data[iheight].im;
          nt_re = fv_data[iheight].im;
          twid_re = b_fv_data[iheight].re;
          b_fv_data[iheight].re = twid_im * twid_re - nt_re * nt_im;
          b_fv_data[iheight].im = twid_im * nt_im + nt_re * twid_re;
        }
        d_FFTImplementationCallback_r2b(b_fv, iDelta2, costab, sintabinv, fv);
        fv_data = fv->data;
        if (fv->size[0] > 1) {
          nt_im = 1.0F / (float)fv->size[0];
          istart = fv->size[0];
          for (iheight = 0; iheight < istart; iheight++) {
            fv_data[iheight].re *= nt_im;
            fv_data[iheight].im *= nt_im;
          }
        }
        iheight = (int)(float)(int)npoints;
        istart = wwc->size[0];
        for (k = iheight; k <= istart; k++) {
          nt_im = wwc_data[k - 1].re;
          nt_re = fv_data[k - 1].im;
          twid_re = wwc_data[k - 1].im;
          twid_im = fv_data[k - 1].re;
          j = k - (int)(float)(int)npoints;
          yCol_data[j].re = nt_im * twid_im + twid_re * nt_re;
          yCol_data[j].im = nt_im * nt_re - twid_re * twid_im;
        }
      }
      emxFree_creal32_T(&b_fv);
      emxFree_creal32_T(&fv);
      emxFree_creal32_T(&wwc);
    }
    emxFree_real32_T(&sintabinv);
    emxFree_real32_T(&sintab);
    emxFree_real32_T(&costab);
    iheight = h->size[0];
    h->size[0] = (int)npoints;
    emxEnsureCapacity_creal32_T(h, iheight);
    wwc_data = h->data;
    for (iheight = 0; iheight < len_tmp_tmp_tmp; iheight++) {
      wwc_data[iheight] = yCol_data[iheight];
    }
    emxFree_creal32_T(&yCol);
  }
}

/*
 * File trailer for freqz.c
 *
 * [EOF]
 */
