#ifndef PALETTES_H
#define PALETTES_H

#include "app_defs.h"

#include "flash/write.h"

#define palette_count 7
#define palette_custom 4
#define palette_novation 4

#define palette_rom_size 0x200

const u8* const palette_get(u8 i);
u8 palette_value(u8 i, u8 v, u8 c);

#endif