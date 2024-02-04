#include "flash/stm32f10x_imports.h"

// From STM32F10x Standard Peripherals Library
// http://stm32.kosyak.info/doc/stm32f10x__crc_8h.html

#define __IO   volatile

typedef struct
{
  __IO u32 DR;
  __IO u8  IDR;
  u8   RESERVED0;
  u16  RESERVED1;
  __IO u32 CR;
} CRC_TypeDef;

#define PERIPH_BASE           ((u32)0x40000000) 
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)
#define CRC_BASE              (AHBPERIPH_BASE + 0x3000)

#define CRC                 ((CRC_TypeDef *) CRC_BASE)

/********************  Bit definition for CRC_CR register  ********************/
#define  CRC_CR_RESET                        ((u8)0x01) 

void CRC_ResetDR(void)
{
  /* Reset CRC generator */
  CRC->CR = CRC_CR_RESET;
}

u32 CRC_CalcBlockCRC(u32 pBuffer[], u32 BufferLength)
{
  u32 index = 0;
  
  for(index = 0; index < BufferLength; index++)
  {
    CRC->DR = pBuffer[index];
  }
  return (CRC->DR);
}
