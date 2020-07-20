#include "flash/write.h"

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

#define FLASH_START 0x0801EC00
#define FLASH_SIZE 0x1000
#define FLASH_PAGE_SIZE 0x0400

const u8* flash_rom = (u8*)FLASH_START;

u8 flash_direct_read(u16 offset) {
	if (offset >= FLASH_SIZE) return 0xFF;

	return flash_rom[offset];
}

void flash_direct_write(u16 start, u8 *data, u16 length) {
	if (start + length >= FLASH_SIZE) length = FLASH_SIZE;

	FLASH_Unlock();
	FLASH_ClearFlag(0x35);

	for (u32 i = start + FLASH_START; i < start + FLASH_START + length; i += FLASH_PAGE_SIZE) {
		FLASH_ErasePage(i);
	}

	for (u32 i = 0; i < length; i += 2) {
		FLASH_ProgramHalfWord(
			FLASH_START + start + i,
			(u16)data[i] | ((u16)data[i + 1] << 8)
		);
	}

	FLASH_Lock();
}