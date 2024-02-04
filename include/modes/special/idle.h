#ifndef IDLE_H
#define IDLE_H

#include "app.h"
#include "modes/mode.h"

#define idle_timeout 600000

u8 idle_return;

void idle_init();
void idle_timer_event();
void idle_surface_event(u8 p, u8 v, u8 x, u8 y);
void idle_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);
void idle_aftertouch_event(u8 v);
void idle_poly_event(u8 p, u8 v);

#endif
