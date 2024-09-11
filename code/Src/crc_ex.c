#include "crc_ex.h"
/**
  * @brief  This function performs CRC calculation on sz bytes from input data buffer pbuf.
  * @param  pbuf pointer to buffer to be processed for CRC calculation
  * @param  sz Nb of bytes to be processed for CRC calculation
  * @retval 8-bit CRC value computed on input data buffer
  */
uint16_t crc_calc(uint8_t *pbuf, uint32_t sz)
{
  register uint32_t data = 0;
  register uint32_t index = 0;

  /* Compute the CRC of Data Buffer array*/
  for (index = 0; index < (sz / 4); index++)
  {
    data = (uint32_t)((pbuf[4 * index] << 24) | (pbuf[4 * index + 1] << 16) | (pbuf[4 * index + 2] << 8) | pbuf[4 * index + 3]);
    LL_CRC_FeedData32(CRC, data);
  }
  
  /* Last bytes specific handling */
  if ((sz % 4) != 0)
  {
    if  (sz % 4 == 1)
    {
      LL_CRC_FeedData8(CRC, pbuf[4 * index]);
    }
    if  (sz % 4 == 2)
    {
      LL_CRC_FeedData16(CRC, (uint16_t)((pbuf[4 * index]<<8) | pbuf[4 * index + 1]));
    }
    if  (sz % 4 == 3)
    {
      LL_CRC_FeedData16(CRC, (uint16_t)((pbuf[4 * index]<<8) | pbuf[4 * index + 1]));
      LL_CRC_FeedData8(CRC, pbuf[4 * index + 2]);
    }
  }

  /* Return computed CRC value */
  return(LL_CRC_ReadData16(CRC));
}