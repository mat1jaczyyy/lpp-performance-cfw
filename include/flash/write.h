#ifndef WRITE_H
#define WRITE_H

#include "app.h"

u8 flash_direct_read(u16 offset);
void flash_direct_write(u16 start, const u8 *data, u16 length);

#endif