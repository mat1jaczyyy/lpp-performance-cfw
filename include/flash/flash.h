#ifndef FLASH_H
#define FLASH_H

#include "app.h"

#include "flash/settings.h"
#include "led/led.h"

#define flash_indicator_r 3
#define flash_indicator_g 0
#define flash_indicator_b 0

u8 dirty;

void flash_read();
void flash_write();

#endif