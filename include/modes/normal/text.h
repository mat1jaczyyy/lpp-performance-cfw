#ifndef TEXT_H
#define TEXT_H

#include "app.h"
#include "sysex/sysex.h"

#include "modes/mode.h"

u8 text_port, text_color, text_loop, text_bytes[322];
u16 text_end;

void text_init();
void text_timer_event();
void text_surface_event(u8 p, u8 v, u8 x, u8 y);
void text_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);
void text_aftertouch_event(u8 v);
void text_poly_event(u8 p, u8 v);

#endif