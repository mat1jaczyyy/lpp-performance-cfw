#ifndef ABLETON_H
#define ABLETON_H

#include "app_defs.h"

#define mode_ableton_r 0
#define mode_ableton_g 63
#define mode_ableton_b 0

#define ableton_layout_session 0x0
#define ableton_layout_note_drum 0x1
#define ableton_layout_note_chromatic 0x2
#define ableton_layout_user 0x3
#define ableton_layout_note_blank 0x4
#define ableton_layout_fader_device 0x5
#define ableton_layout_session_arm 0x6
#define ableton_layout_session_track 0x7
#define ableton_layout_session_mute 0x8
#define ableton_layout_session_solo 0x9
#define ableton_layout_fader_volume 0xA
#define ableton_layout_fader_pan 0xB
#define ableton_layout_fader_sends 0xC
#define ableton_layout_session_stop 0xD

#define user_r 20
#define user_g 0
#define user_b 63

u8 ableton_screen[100], ableton_layout;

void ableton_led(u8 ch, u8 p, u8 v, u8 s);

void ableton_init();
void ableton_timer_event();
void ableton_surface_event(u8 p, u8 v, u8 x, u8 y);
void ableton_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);

#endif