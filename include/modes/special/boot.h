#ifndef BOOT_H
#define BOOT_H

#include "app_defs.h"
#include "modes/mode.h"

void boot_init();
void boot_timer_event();
void boot_surface_event(u8 p, u8 v, u8 x, u8 y);
void boot_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);
void boot_aftertouch_event(u8 v);
void boot_poly_event(u8 p, u8 v);

#endif