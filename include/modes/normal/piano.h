#ifndef PIANO_H
#define PIANO_H

#include "app.h"
#include "modes/mode.h"

#include "aftertouch/aftertouch.h"
#include "aftertouch/poly.h"

#define mode_piano_r 7
#define mode_piano_g 0
#define mode_piano_b 63

s8 piano_octave;
u8 piano_shift;

void piano_init();
void piano_timer_event();
void piano_surface_event(u8 p, u8 v, u8 x, u8 y);
void piano_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);
void piano_aftertouch_event(u8 v);
void piano_poly_event(u8 p, u8 v);

#endif