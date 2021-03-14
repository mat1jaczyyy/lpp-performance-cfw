#ifndef WRITE_H
#define WRITE_H

#include "app.h"

#include "stm32f10x_imports.h"

#define FLASH_PAGE_PALETTE        0
#define FLASH_PAGE_CHORD_BANKS    2
#define FLASH_PAGE_CUSTOM_MODES   3
#define FLASH_PAGE_SETTINGS      11

// Stock ROM reaches 0x0801BC80, so the minimum possible start is 0x0801C000
#define FLASH_START 0x0801CC00
#define FLASH_PAGE_SIZE 0x0400  // 1kB
#define FLASH_PAGES 12

#define FLASH_ADDR(page) (FLASH_START + FLASH_PAGE_SIZE * (page))

void flash_direct_write(const u8 page, const u16* const data, u16 length);

void flash_direct_settings_write();
void flash_direct_palette_write(const u8 page, const u8* const lo, const u8* const hi);

#endif