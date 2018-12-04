#ifndef PALETTES_H
#define PALETTES_H

#include "app_defs.h"

#define palette_count 6
#define palette_custom 3
#define palette_novation 3

u8 palette[palette_custom][3][128];
//const u8 palette_preset[palette_count - palette_custom][3][128];

u8 palette_value(u8 i, u8 v, u8 c);
u8 palette_value_reverse(u8 i, u8 v, u8 c);

#endif
