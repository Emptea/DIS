#ifndef _ARM_EXTRA_TABLES_H
#define _ARM_EXTRA_TABLES_H

#include "arm_math_types.h"

#ifdef   __cplusplus
extern "C"
{
#endif

#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_FFT_ALLOW_TABLES)
  /* Double Precision Float CFFT twiddles */
  #if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || defined(ARM_TABLE_BITREV_8192)
    extern const uint16_t armBitRevTableExtra[8192];
  #endif /* !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) */
  
  #if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || defined(ARM_TABLE_BITREVIDX_FLT_8192)
    #define ARMBITREVINDEXTABLE_8192_TABLE_LENGTH_EXTRA ((uint16_t)14576)
    extern const uint32_t armBitRevIndexTableExtra8192[ARMBITREVINDEXTABLE_8192_TABLE_LENGTH_EXTRA];
  #endif /* !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) */
  
  #if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || defined(ARM_TABLE_BITREVIDX_FLT_16384)
    #define ARMBITREVINDEXTABLE_16384_TABLE_LENGTH_EXTRA ((uint16_t)31080)
    extern const uint32_t armBitRevIndexTableExtra16384[ARMBITREVINDEXTABLE_16384_TABLE_LENGTH_EXTRA];
  #endif /* !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) */
  
  #if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || defined(ARM_TABLE_BITREVIDX_FLT_32768)
    #define ARMBITREVINDEXTABLE_32768_TABLE_LENGTH_EXTRA ((uint16_t)32256)
    extern const uint32_t armBitRevIndexTableExtra32768[ARMBITREVINDEXTABLE_32768_TABLE_LENGTH_EXTRA];
  #endif /* !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) */
  
  #if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || defined(ARM_TABLE_TWIDDLECOEF_F32_8192)
    extern const float32_t twiddleCoefExtra_8192[16384];
  #endif /* !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) */
  
  #if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || defined(ARM_TABLE_TWIDDLECOEF_F32_16384)
    extern const float32_t twiddleCoefExtra_16384[32768];
  #endif /* !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) */
  
  #if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || defined(ARM_TABLE_TWIDDLECOEF_F32_32768)
    extern const float32_t twiddleCoefExtra_32768[65536];
  #endif /* !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) */
  
  #if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || defined(ARM_TABLE_TWIDDLECOEF_RFFT_F32_16384)
    extern const float32_t twiddleCoefExtra_rfft_16384[16384];
  #endif /* !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) */

  #if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || defined(ARM_TABLE_TWIDDLECOEF_RFFT_F32_32768)
    extern const float32_t twiddleCoefExtra_rfft_32768[32768];
  #endif /* !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) */


#endif /* if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_FAST_TABLES) */
#ifdef   __cplusplus
}
#endif

#endif /*  ARM_EXTRA_TABLES_H */