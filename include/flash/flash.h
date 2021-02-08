#ifndef FLASH_H
#define FLASH_H

#include "app.h"

#include "flash/settings.h"
#include "flash/write.h"
#include "led/led.h"

u8 dirty;
const u8* chord_banks;

void flash_read();
void flash_write();
void flash_write_custom(u8 index, const u8* buffer);
void flash_write_chord_bank(u8 index, const u8* buffer);

#endif