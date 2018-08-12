#ifndef SCALE_H
#define SCALE_H

#include "app.h"
#include "modes/mode.h"

#define scale_setup_color_enabled_r 20
#define scale_setup_color_enabled_g 63
#define scale_setup_color_enabled_b 0

#define scale_setup_color_segment_r 63
#define scale_setup_color_segment_g 20
#define scale_setup_color_segment_b 0

#define scale_setup_color_notes_r 0
#define scale_setup_color_notes_g 63
#define scale_setup_color_notes_b 63

#define scale_setup_color_root_r 0
#define scale_setup_color_root_g 0
#define scale_setup_color_root_b 63

#define scale_setup_color_scale_r 63
#define scale_setup_color_scale_g 0
#define scale_setup_color_scale_b 63

#define scale_setup_color_scale_selected_r 20
#define scale_setup_color_scale_selected_g 0
#define scale_setup_color_scale_selected_b 63

void scale_setup_init();
void scale_setup_timer_event();
void scale_setup_surface_event(u8 p, u8 v, u8 x, u8 y);
void scale_setup_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);

#endif