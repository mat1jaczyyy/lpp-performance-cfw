#ifndef PUYO_H
#define PUYO_H

#include "app.h"
#include "modes/mode.h"

#include "string.h"

void puyo_init();
void puyo_timer_event();
void puyo_surface_event(u8 p, u8 v, u8 x, u8 y);
void puyo_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);
void puyo_aftertouch_event(u8 v);
void puyo_poly_event(u8 p, u8 v);

#endif