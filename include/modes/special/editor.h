#ifndef EDITOR_H
#define EDITOR_H

#include "app_defs.h"
#include "modes/mode.h"

u8 editor_xy_v(u8 xy);
u8 editor_v_xy(u8 v);
void editor_refresh();
void editor_draw();
void editor_select_xy(u8 xy);
void editor_select_v(u8 v);
void editor_select_flip(u8 i);

void editor_init();
void editor_timer_event();
void editor_surface_event(u8 p, u8 v, u8 x, u8 y);
void editor_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);

#endif