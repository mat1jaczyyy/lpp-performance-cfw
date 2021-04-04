#include "flash/flash.h"

u8 dirty = 0;
u8 dirty_palette = 0xFF;

void flash_read() {
	settings_init();
	
	const u32* const addr = (const u32*)FLASH_ADDR(FLASH_PAGE_SETTINGS);

	if (addr[0] != settings_header) return;
	if (addr[2] != settings_crc(addr + 3, addr[1])) return;

	memcpy(&settings, addr + 3, sizeof(settings));

	settings_validate();
}

void flash_write() {
	if (dirty) {
		flash_direct_settings_write();
		dirty = 0;
	}

	if (dirty_palette < palette_custom) {
		flash_direct_palette_write(
			dirty_palette >> 1,
			(dirty_palette & 1)? palette_get(dirty_palette - 1) : (const u8*)editor_palette,
			(dirty_palette & 1)? (const u8*)editor_palette : palette_get(dirty_palette + 1)
		);

		dirty_palette = 0xFF;
	}
}

void flash_write_custom(u8 index, const u8* buffer) {
	flash_direct_write(FLASH_PAGE_CUSTOM_MODES + index, (const u16*)buffer, 1024);
}

const u8* const chord_banks = (const u8*)FLASH_ADDR(FLASH_PAGE_CHORD_BANKS);

void flash_write_chord_bank(u8 index, const u8* buffer) {
	u8 temp[14 * 9];

	memcpy(temp, chord_banks, 14 * 9);
	memcpy(temp + 9 * index, buffer, 9);
	
	flash_direct_write(FLASH_PAGE_CHORD_BANKS, (const u16*)temp, 14 * 9);
}