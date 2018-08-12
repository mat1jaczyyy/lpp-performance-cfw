#ifndef NOTE_H
#define NOTE_H

#include "app.h"
#include "modes/mode.h"

#define mode_note_r 0
#define mode_note_g 63
#define mode_note_b 63

#define note_color_invalid_r 7
#define note_color_invalid_g 0
#define note_color_invalid_b 0

#define note_color_base_r 63
#define note_color_base_g 0
#define note_color_base_b 63

#define note_color_transposed_r 20
#define note_color_transposed_g 0
#define note_color_transposed_b 63

#define note_color_white_r 0
#define note_color_white_g 41
#define note_color_white_b 63

#define note_color_black_r 0
#define note_color_black_g 0
#define note_color_black_b 0

#define note_color_scale_base_r 20
#define note_color_scale_base_g 0
#define note_color_scale_base_b 63

#define note_color_scale_r 63
#define note_color_scale_g 0
#define note_color_scale_b 63

#define note_color_pressed_r 0
#define note_color_pressed_g 63
#define note_color_pressed_b 0

#define note_length 12
#define note_segment 5

s8 note_octave, note_transpose;
u8 note_shift, note_octave_colors[10][3], note_transpose_colors[13][3], scales[32][13], scale_keys[12];
u8 scale_enabled, scale_segment, scale_selected, scale_root;

void note_single(u8 *p, u8 l, u8 r, u8 g, u8 b);
s8 note_press(u8 x, u8 y, u8 v, s8 out_p);
void note_scale_button();
void note_draw();

void note_init();
void note_timer_event();
void note_surface_event(u8 p, u8 v, u8 x, u8 y);
void note_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);

#endif