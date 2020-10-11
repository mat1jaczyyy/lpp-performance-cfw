#ifndef CHORD_H
#define CHORD_H

#include "app.h"
#include "modes/mode.h"
#include "send/send.h"

#define mode_chord_r 0
#define mode_chord_g 0
#define mode_chord_b 63

void chord_init();
void chord_timer_event();
void chord_surface_event(u8 p, u8 v, u8 x, u8 y);
void chord_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);
void chord_aftertouch_event(u8 v);
void chord_poly_event(u8 p, u8 v);

#endif
