
/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_cfft_init_f32_extra.c
 * Description:  Initialization function for cfft f32 instance
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

#define FFTINIT(EXT,SIZE)                                           \
  S->bitRevLength = arm_cfft_sR_##EXT##_len##SIZE##_extra.bitRevLength;        \
  S->pBitRevTable = arm_cfft_sR_##EXT##_len##SIZE##_extra.pBitRevTable;         \
  S->pTwiddle = arm_cfft_sR_##EXT##_len##SIZE##_extra.pTwiddle;

/**
  @addtogroup ComplexFFT
  @{
 */

/**
  @brief         Initialization function for the cfft f32 function
  @param[in,out] S              points to an instance of the floating-point CFFT structure
  @param[in]     fftLen         fft length (number of complex samples)
  @return        execution status
                   - \ref ARM_MATH_SUCCESS        : Operation successful
                   - \ref ARM_MATH_ARGUMENT_ERROR : an error is detected

  @par          Use of this function is mandatory only for the MVE version of the FFT.
                Other versions can still initialize directly the data structure using 
                variables declared in arm_const_structs_extra.h
 */

#include "extra_ffts.h"
#include "arm_common_tables_extra.h"
#include "arm_const_structs_extra.h"

#if defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_vec_fft.h"
#include "arm_mve_tables.h"

arm_status arm_cfft_radix4by2_rearrange_twiddles_f32(arm_cfft_instance_f32_extra *S, int twidCoefModifier)
{
                                                                  
        switch (S->fftLen >> (twidCoefModifier - 1)) {  

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) \
            || defined(ARM_TABLE_TWIDDLECOEF_F32_16384)                                                                                                
        case 16384U:                                                                                
            S->rearranged_twiddle_tab_stride1_arr = rearranged_twiddle_tab_stride1_arr_16384_f32;
            S->rearranged_twiddle_stride1  =  rearranged_twiddle_stride1_16384_f32;     

            S->rearranged_twiddle_tab_stride2_arr = rearranged_twiddle_tab_stride2_arr_16384_f32;
            S->rearranged_twiddle_stride2  =  rearranged_twiddle_stride2_16384_f32;    

            S->rearranged_twiddle_tab_stride3_arr = rearranged_twiddle_tab_stride3_arr_16384_f32;
            S->rearranged_twiddle_stride3  =  rearranged_twiddle_stride3_16384_f32;                                                     
            break; 
#endif
                                                                         
                                                                                                   
        default:  
            return(ARM_MATH_ARGUMENT_ERROR);                                                                                 
            break;                                                                                 
            /* invalid sizes already filtered */                                                   
        }                                                                                          

        return(ARM_MATH_SUCCESS);

}

arm_status arm_cfft_init_f32_extra(
  arm_cfft_instance_f32_extra * S,
  uint16_t fftLen)
{

        /*  Initialise the default arm status */                                
        arm_status status = ARM_MATH_SUCCESS;                                   
                                                                                
        /*  Initialise the FFT length */                                        
        S->fftLen = fftLen;                                                     
                                                                                
        /*  Initialise the Twiddle coefficient pointer */                       
        S->pTwiddle = NULL;                         
                                                                                
                                                                                
        /*  Initializations of Instance structure depending on the FFT length */
        switch (S->fftLen) {   
                                                 
            /*  Initializations of structure parameters for 32768 point FFT */   
#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || (defined(ARM_TABLE_BITREVIDX_FXT_32768) && defined(ARM_TABLE_TWIDDLECOEF_F32_32768))                     
        case 32768U:  
            /*  Initialise the bit reversal table modifier */                   
            S->bitRevLength = ARMBITREVINDEXTABLE_FIXED_32768_TABLE_LENGTH_EXTRA;      
            S->pBitRevTable = (uint16_t *)armBitRevIndexTable_fixed_32768;   
            S->pTwiddle = (float32_t *)twiddleCoefExtra_32768;       
            status=arm_cfft_radix4by2_rearrange_twiddles_f32(S, 2);               
            break;                                                              
#endif 
                                                 
            /*  Initializations of structure parameters for 16384 point FFT */   
#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || (defined(ARM_TABLE_BITREVIDX_FXT_16384) && defined(ARM_TABLE_TWIDDLECOEF_F32_16384))                     
        case 16384U:  
            /*  Initialise the bit reversal table modifier */                   
            S->bitRevLength = ARMBITREVINDEXTABLE_FIXED_16384_TABLE_LENGTH_EXTRA;      
            S->pBitRevTable = (uint16_t *)armBitRevIndexTable_fixed_16384;   
            S->pTwiddle = (float32_t *)twiddleCoefExtra_16384;       
            status=arm_cfft_radix4by2_rearrange_twiddles_f32(S, 1);               
            break;                                                              
#endif 
                                                   
        default:                                                                
            /*  Reporting argument error if fftSize is not valid value */       
            status = ARM_MATH_ARGUMENT_ERROR;                                   
            break;                                                              
        }                                                                       
                                                                                
                                                                                
        return (status);     
}
#else
arm_status arm_cfft_init_f32_extra(
  arm_cfft_instance_f32_extra * S,
  uint16_t fftLen)
{
        /*  Initialise the default arm status */
        arm_status status = ARM_MATH_SUCCESS;

        /*  Initialise the FFT length */
        S->fftLen = fftLen;

        /*  Initialise the Twiddle coefficient pointer */
        S->pTwiddle = NULL;


        /*  Initializations of Instance structure depending on the FFT length */
        switch (S->fftLen) {

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || (defined(ARM_TABLE_TWIDDLECOEF_F32_32768) && defined(ARM_TABLE_BITREVIDX_FLT_32768))
            /*  Initializations of structure parameters for 32768 point FFT */
        case 32768U:
            /*  Initialise the bit reversal table modifier */
            FFTINIT(f32,32768);
            break;
#endif

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || (defined(ARM_TABLE_TWIDDLECOEF_F32_16384) && defined(ARM_TABLE_BITREVIDX_FLT_16384))
            /*  Initializations of structure parameters for 16384 point FFT */
        case 16384U:
            /*  Initialise the bit reversal table modifier */
            FFTINIT(f32,16384);
            break;
#endif


        default:
            /*  Reporting argument error if fftSize is not valid value */
            status = ARM_MATH_ARGUMENT_ERROR;
            break;
        }


        return (status);
}
#endif /* defined(ARM_MATH_MVEF) && !defined(ARM_MATH_AUTOVECTORIZE) */

/**
  @} end of ComplexFFT group
 */

