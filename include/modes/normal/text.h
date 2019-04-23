#ifndef TEXT_H
#define TEXT_H

#include "app.h"
#include "modes/mode.h"

#include "string.h"
#include "sysex/messages.h"

u8 text_port, text_color, text_loop, text_bytes[323], text_palette;

void text_init();
void text_timer_event();
void text_surface_event(u8 p, u8 v, u8 x, u8 y);
void text_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);
void text_aftertouch_event(u8 v);

#endif