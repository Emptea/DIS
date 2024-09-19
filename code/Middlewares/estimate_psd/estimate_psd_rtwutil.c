/*
 * File: estimate_psd_rtwutil.c
 *
 * MATLAB Coder version            : 5.6
 * C/C++ source code generated on  : 18-Sep-2024 13:56:03
 */

/* Include Files */
#include "estimate_psd_rtwutil.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>

/* Function Definitions */
/*
 * Arguments    : float u0
 *                float u1
 * Return Type  : float
 */
float rt_hypotf_snf(float u0, float u1)
{
  float a;
  float b;
  float y;
  a = fabsf(u0);
  b = fabsf(u1);
  if (a < b) {
    a /= b;
    y = b * sqrtf(a * a + 1.0F);
  } else if (a > b) {
    b /= a;
    y = a * sqrtf(b * b + 1.0F);
  } else if (rtIsNaNF(b)) {
    y = rtNaNF;
  } else {
    y = a * 1.41421354F;
  }
  return y;
}

/*
 * File trailer for estimate_psd_rtwutil.c
 *
 * [EOF]
 */
