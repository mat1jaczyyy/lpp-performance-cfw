#ifndef SETUP_H
#define SETUP_H

#include "app_defs.h"
#include "modes/mode.h"

#define setup_tick 33
#define setup_rainbow_length 48

#define setup_custom_r 47
#define setup_custom_g 63
#define setup_custom_b 63

#define setup_preset_r 47
#define setup_preset_g 63
#define setup_preset_b 47

#define setup_velocity_r 31
#define setup_velocity_g 63
#define setup_velocity_b 63

#define setup_top_pro_r 63
#define setup_top_pro_g 47
#define setup_top_pro_b 63

#define setup_top_mk2_r 47
#define setup_top_mk2_g 31
#define setup_top_mk2_b 63

const u8 setup_rainbow[setup_rainbow_length][3];

u8 setup_elapsed, setup_mode_counter, setup_editor_counter, setup_jump;

void setup_init();
void setup_timer_event();
void setup_surface_event(u8 p, u8 v, u8 x, u8 y);
void setup_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);

#endif