#ifndef FADER_H
#define FADER_H

#include "app.h"
#include "modes/mode.h"
#include "send/send.h"

#define mode_fader_r 63
#define mode_fader_g 0
#define mode_fader_b 63

u8 fader_mode, faders[2][8], fader_type[2][8], fader_color[2][8], fader_counter[2][8]; // u8 fader_final[2][8];

void fader_draw(u8 y);

void fader_init();
void fader_timer_event();
void fader_surface_event(u8 p, u8 v, u8 x, u8 y);
void fader_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);
void fader_aftertouch_event(u8 v);
void fader_poly_event(u8 p, u8 v);

#endif
