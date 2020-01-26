#ifndef FLASH_H
#define FLASH_H

#include "app.h"

#include "flash/settings.h"
#include "flash/write.h"
#include "led/led.h"

u8 dirty;

void flash_read();
void flash_write();

#endif