
#include "arm_math_types.h"
#include "arm_const_structs_extra.h"

/* Floating-point structs */
#if !defined(ARM_MATH_MVEF) || defined(ARM_MATH_AUTOVECTORIZE)


#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || (defined(ARM_TABLE_TWIDDLECOEF_F32_8192) && defined(ARM_TABLE_BITREVIDX_FLT_8192))
const arm_cfft_instance_f32_extra arm_cfft_sR_f32_len8192_extra = {
  8192, twiddleCoefExtra_8192, armBitRevIndexTableExtra8192, ARMBITREVINDEXTABLE_8192_TABLE_LENGTH_EXTRA
};
#endif



#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || (defined(ARM_TABLE_TWIDDLECOEF_F32_16384) && defined(ARM_TABLE_BITREVIDX_FLT_16384))
const arm_cfft_instance_f32_extra arm_cfft_sR_f32_len16384_extra = {
  16384, twiddleCoefExtra_16384, armBitRevIndexTableExtra16384, ARMBITREVINDEXTABLE_16384_TABLE_LENGTH_EXTRA
};
#endif



#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || (defined(ARM_TABLE_TWIDDLECOEF_F32_32768) && defined(ARM_TABLE_BITREVIDX_FLT_32768))
const arm_cfft_instance_f32_extra arm_cfft_sR_f32_len32768_extra = {
  32768, twiddleCoefExtra_32768, armBitRevIndexTableExtra32768, ARMBITREVINDEXTABLE_32768_TABLE_LENGTH_EXTRA
};
#endif



#if !defined(ARM_DSP_CONFIG_TABLES) || defined(ARM_ALL_FFT_TABLES) || (defined(ARM_TABLE_TWIDDLECOEF_F32_65536) && defined(ARM_TABLE_BITREVIDX_FLT_65536))
const arm_cfft_instance_f32_extra arm_cfft_sR_f32_len65536_extra = {
  65536, twiddleCoefExtra_65536, armBitRevIndexTableExtra65536, ARMBITREVINDEXTABLE_65536_TABLE_LENGTH_EXTRA
};
#endif


#endif /* !defined(ARM_MATH_MVEF) || defined(ARM_MATH_AUTOVECTORIZE) */
