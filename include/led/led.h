#ifndef LED_H
#define LED_H

#include "app.h"

#include "led/palettes.h"
#include "flash/settings.h"

u8 flash_screen[100], pulse_screen[100];

void rgb_led(u8 p, u8 r, u8 g, u8 b);
void clear_led();
void palette_led(u8 p, u8 v);
void palette_channel_led(u8 p, u8 c, u8 v);
void novation_led(u8 p, u8 v);
void flash_led(u8 p, u8 v);
void pulse_led(u8 p, u8 v);

void display_u8(u8 v, u8 d, u8 x, u8 r, u8 g, u8 b);
void display_u6(u8 v, u8 d, u8 x, u8 r, u8 g, u8 b);

#endif
