#ifndef STM32F10X_IMPORTS_H
#define STM32F10X_IMPORTS_H

#include "app_defs.h"

// From STM32F10x Standard Peripherals Library
// http://stm32.kosyak.info/doc/stm32f10x__crc_8h.html

typedef enum {
  FLASH_BUSY = 1,
  FLASH_ERROR_PG,
  FLASH_ERROR_WRP,
  FLASH_COMPLETE,
  FLASH_TIMEOUT
} FLASH_Status;

void FLASH_Unlock(void); // Unlocks the FLASH Program Erase Controller.
void FLASH_ClearFlag(u32 FLASH_FLAG); // Clears the FLASH's pending flags.
FLASH_Status FLASH_ErasePage(u32 Page_Address); // Erases a specified FLASH page.
FLASH_Status FLASH_ProgramHalfWord(u32 Address, u16 Data); // Programs a half word at a specified address.
void FLASH_Lock(void); // Locks the FLASH Program Erase Controller.

void CRC_ResetDR(void); // Resets the CRC Data register (DR).
u32 CRC_CalcBlockCRC(u32 pBuffer[], u32 BufferLength); // Computes the 32-bit CRC of a given buffer of data word(32-bit).

#endif