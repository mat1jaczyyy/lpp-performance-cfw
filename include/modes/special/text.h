#ifndef TEXT_H
#define TEXT_H

#include "app.h"
#include "modes/mode.h"

#include "string.h"
#include "sysex/messages.h"

const u8 text_bitmap[96][6], text_ticks[7], text_frame_empty[10];

u8 text_port, text_color, text_loop, text_bytes[323], text_speed, text_elapsed, text_subcounter, text_frame[10], text_done, text_palette;
u16 text_counter;

void text_init();
void text_timer_event();
void text_surface_event(u8 p, u8 v, u8 x, u8 y);
void text_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);

#endif