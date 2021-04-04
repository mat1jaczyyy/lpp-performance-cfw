#include "flash/write.h"

inline void flash_erase(const u32 addr) {
	if (FLASH_START <= addr && addr < FLASH_ADDR(FLASH_PAGES))
		FLASH_ErasePage(addr);
}

inline void flash_program(const u32 addr, const u16 data) {
	if (FLASH_START <= addr && addr < FLASH_ADDR(FLASH_PAGES))
		FLASH_ProgramHalfWord(addr, data);
}

inline void flash_u32(const u32 addr, const u32 data) {
	flash_program(addr, data & 0xFFFF);
	flash_program(addr + 2, data >> 16);
}

inline void flash_program_buffer(const u32 addr, const u16* const data, const u16 length) {
	for (u16 i = 0; i < length; i += 2)
		flash_program(addr + i, data[i >> 1]);
}

void flash_direct_write(const u8 page, const u16* const data, const u16 length) {
	if (page >= FLASH_PAGES) return;

	u8 end = page + (length + FLASH_PAGE_SIZE - 1) / FLASH_PAGE_SIZE; // ceiling division

	if (end >= FLASH_PAGES)
		end = FLASH_PAGES;

	FLASH_Unlock();
	FLASH_ClearFlag(0x35);

	for (u8 i = page; i < end; i++)
		flash_erase(FLASH_ADDR(i));

	flash_program_buffer(FLASH_ADDR(page), data, length);

	FLASH_Lock();
}

void flash_direct_settings_write() {
	FLASH_Unlock();
	FLASH_ClearFlag(0x35);

	const u32 addr = FLASH_ADDR(FLASH_PAGE_SETTINGS);
	flash_erase(addr);

	flash_u32(addr, settings_header);
	flash_u32(addr + sizeof(settings_header), sizeof(settings));
	flash_u32(addr + sizeof(settings_header) + sizeof(u32), settings_crc(&settings, sizeof(settings)));

	flash_program_buffer(addr + sizeof(settings_header) + 2 * sizeof(u32), (const u16*)&settings, sizeof(settings));

	FLASH_Lock();
}

void flash_direct_palette_write(const u8 page, const u8* const lo, const u8* const hi) {
	if (page >= FLASH_PAGES) return;

	u8 lo_buf[384], hi_buf[384];

	memcpy(lo_buf, lo, sizeof(lo_buf));
	memcpy(hi_buf, hi, sizeof(hi_buf));

	FLASH_Unlock();
	FLASH_ClearFlag(0x35);

	const u32 addr = FLASH_ADDR(page);
	flash_erase(addr);

	flash_program_buffer(addr, (const u16*)lo_buf, 384);
	flash_program_buffer(addr + palette_rom_size, (const u16*)hi_buf, 384);

	FLASH_Lock();
}