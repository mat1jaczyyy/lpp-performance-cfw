#ifndef BOOT_H
#define BOOT_H

#include "app_defs.h"
#include "modes/mode.h"

#define boot_note_tick 72
#define boot_note_length 17
#define boot_notes_per_frame 6

#define boot_rotate_180 1

#define boot_fade_tick 4

#define boot_colors_length 252

#define boot_user_stop 188

const u8 boot_notes[boot_note_length][boot_notes_per_frame], boot_colors[boot_colors_length][3];

u8 boot_note_elapsed, boot_note_floor, boot_note_ceil, boot_fade_elapsed, boot_fade_counter[16];

void boot_init();
void boot_timer_event();
void boot_surface_event(u8 p, u8 v, u8 x, u8 y);
void boot_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);

#endif