#ifndef DRUM_H
#define DRUM_H

#include "app_defs.h"

#define mode_drum_r 63
#define mode_drum_g 63
#define mode_drum_b 0

#define drum_offset_start 9

#define drum_nav_r 0
#define drum_nav_g 0
#define drum_nav_b 63

#define drum_color_pressed_r 0
#define drum_color_pressed_g 63
#define drum_color_pressed_b 0

u8 drum_offset, drum_colors[9][3], drum_nav_pressed[4], drum_align[17];

s8 drum_press(u8 x, u8 y, u8 v, s8 out_p);

void drum_init();
void drum_timer_event();
void drum_surface_event(u8 p, u8 v, u8 x, u8 y);
void drum_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);

#endif