#ifndef EDITOR_H
#define EDITOR_H

#include "app_defs.h"
#include "modes/mode.h"
#include "sysex/messages.h"

void editor_init();
void editor_timer_event();
void editor_surface_event(u8 p, u8 v, u8 x, u8 y);
void editor_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);
void editor_aftertouch_event(u8 v);

#endif