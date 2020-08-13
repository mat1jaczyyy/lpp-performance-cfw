#ifndef CUSTOM_H
#define CUSTOM_H

#include "app.h"
#include "modes/mode.h"
#include "send/send.h"

#include "aftertouch/aftertouch.h"

#define mode_custom_r 63
#define mode_custom_g 63
#define mode_custom_b 63

u8 custom_prev_active_slot;

void custom_init();
void custom_timer_event();
void custom_surface_event(u8 p, u8 v, u8 x, u8 y);
void custom_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);
void custom_aftertouch_event(u8 v);
void custom_poly_event(u8 p, u8 v);

#endif