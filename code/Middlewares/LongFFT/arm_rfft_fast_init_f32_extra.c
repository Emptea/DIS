
/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_rfft_fast_init_f32_extra.c
 * Description:  Split Radix Decimation in Frequency CFFT Floating point processing function
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "extra_ffts.h"
#include "arm_common_tables_extra.h"

/**
  @ingroup groupTransforms
 */

/**
  @addtogroup RealFFT
  @{
 */

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || (defined(ARM_TABLE_TWIDDLECOEF_F32_8192) && (defined(ARM_TABLE_BITREVIDX_FLT_8192) || defined(ARM_TABLE_BITREVIDX_FXT_8192)) && defined(ARM_TABLE_TWIDDLECOEF_RFFT_F32_16384))

/**
  @private
  @brief         Initialization function for the 16384pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f32_extra structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */

static arm_status arm_rfft_16384_fast_init_f32( arm_rfft_fast_instance_f32_extra * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_f32_extra(&(S->Sint),8192);
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }

  S->fftLenRFFT = 16384U;
  S->pTwiddleRFFT    = (float32_t *) twiddleCoefExtra_rfft_16384;

  return ARM_MATH_SUCCESS;
}
#endif 

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || (defined(ARM_TABLE_TWIDDLECOEF_F32_16384) && (defined(ARM_TABLE_BITREVIDX_FLT_16384) || defined(ARM_TABLE_BITREVIDX_FXT_16384)) && defined(ARM_TABLE_TWIDDLECOEF_RFFT_F32_32768))

/**
  @private
  @brief         Initialization function for the 32768pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f32_extra structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */

static arm_status arm_rfft_32768_fast_init_f32( arm_rfft_fast_instance_f32_extra * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_f32_extra(&(S->Sint),16384);
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }

  S->fftLenRFFT = 32768U;
  S->pTwiddleRFFT    = (float32_t *) twiddleCoefExtra_rfft_32768;

  return ARM_MATH_SUCCESS;
}
#endif 

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || (defined(ARM_TABLE_TWIDDLECOEF_F32_32768) && (defined(ARM_TABLE_BITREVIDX_FLT_32768) || defined(ARM_TABLE_BITREVIDX_FXT_32768)) && defined(ARM_TABLE_TWIDDLECOEF_RFFT_F32_65536))

/**
  @private
  @brief         Initialization function for the 65536pt floating-point real FFT.
  @param[in,out] S  points to an arm_rfft_fast_instance_f32_extra structure
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected
 */

static arm_status arm_rfft_65536_fast_init_f32( arm_rfft_fast_instance_f32_extra * S ) {

  arm_status status;

  if( !S ) return ARM_MATH_ARGUMENT_ERROR;

  status=arm_cfft_init_f32_extra(&(S->Sint),32768);
  if (status != ARM_MATH_SUCCESS)
  {
    return(status);
  }

  S->fftLenRFFT = 65536U;
  S->pTwiddleRFFT    = (float32_t *) twiddleCoefExtra_rfft_65536;

  return ARM_MATH_SUCCESS;
}
#endif 


/**
  @brief         Initialization function for the floating-point real FFT.
  @param[in,out] S       points to an arm_rfft_fast_instance_f32_extra structure
  @param[in]     fftLen  length of the Real Sequence
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : <code>fftLen</code> is not a supported length

  @par           Description
                   The parameter <code>fftLen</code> specifies the length of RFFT/CIFFT process.
                   Supported FFT Lengths are 16384, 32768, 65536.
  @par
                   This Function also initializes Twiddle factor table pointer and Bit reversal table pointer.
 */

arm_status arm_rfft_fast_init_f32_extra(
  arm_rfft_fast_instance_f32_extra * S,
  uint16_t fftLen)
{
  typedef arm_status(*fft_init_ptr)( arm_rfft_fast_instance_f32_extra *);
  fft_init_ptr fptr = 0x0;

  switch (fftLen)
  {

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || (defined(ARM_TABLE_TWIDDLECOEF_F32_32768) && (defined(ARM_TABLE_BITREVIDX_FLT_32768) || defined(ARM_TABLE_BITREVIDX_FXT_32768)) && defined(ARM_TABLE_TWIDDLECOEF_RFFT_F32_65536))
  case 65536U:
    fptr = arm_rfft_65536_fast_init_f32;
    break;
#endif

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || (defined(ARM_TABLE_TWIDDLECOEF_F32_16384) && (defined(ARM_TABLE_BITREVIDX_FLT_16384) || defined(ARM_TABLE_BITREVIDX_FXT_16384)) && defined(ARM_TABLE_TWIDDLECOEF_RFFT_F32_32768))
  case 32768U:
    fptr = arm_rfft_32768_fast_init_f32;
    break;
#endif

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || (defined(ARM_TABLE_TWIDDLECOEF_F32_8192) && (defined(ARM_TABLE_BITREVIDX_FLT_8192) || defined(ARM_TABLE_BITREVIDX_FXT_8192)) && defined(ARM_TABLE_TWIDDLECOEF_RFFT_F32_16384))
  case 16384U:
    fptr = arm_rfft_16384_fast_init_f32;
    break;
#endif

  default:
    break;
  }

  if( ! fptr ) return ARM_MATH_ARGUMENT_ERROR;
  return fptr( S );

}

/**
  @} end of RealFFT group
 */

