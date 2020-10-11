#include "modes/special/scale.h"

#define scale_setup_color_enabled_r 20
#define scale_setup_color_enabled_g 63
#define scale_setup_color_enabled_b 0

#define scale_setup_color_translate_r 63
#define scale_setup_color_translate_g 63
#define scale_setup_color_translate_b 0

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

void scale_setup_init() {
	switch (mode_default) {
		case mode_ableton:
			rgb_led(99, mode_ableton_r, mode_ableton_g, mode_ableton_b); // Note mode LED
			break;

		case mode_note:
			rgb_led(99, mode_note_r, mode_note_g, mode_note_b); // Note mode LED
			break;

		case mode_chord:
			rgb_led(99, mode_chord_r, mode_chord_g, mode_chord_b); // Chord mode LED
			break;
	}

	if (scale_enabled) {
		rgb_led(88, scale_setup_color_enabled_r, scale_setup_color_enabled_g, scale_setup_color_enabled_b); // Scale mode enabled
	} else {
		rgb_led(88, scale_setup_color_enabled_r / 5, scale_setup_color_enabled_g / 5, scale_setup_color_enabled_b / 5); // Scale mode disabled
	}

	if (translate_enabled) {
		rgb_led(58, scale_setup_color_translate_r, scale_setup_color_translate_g, scale_setup_color_translate_b); // Translate mode enabled
	} else {
		rgb_led(58, scale_setup_color_translate_r / 5, scale_setup_color_translate_g / 5, scale_setup_color_translate_b / 5); // Translate mode disabled
	}

	for (u8 i = 81; i < 88; i++) {
		rgb_led(i, scale_setup_color_segment_r / 3, scale_setup_color_segment_g / 3, scale_setup_color_segment_b / 3); // Segment length options
	}
	
	if (scale_segment < 8)
		rgb_led(88 - scale_segment, scale_setup_color_segment_r, scale_setup_color_segment_g, scale_setup_color_segment_b); // Selected segment

	for (u8 i = 0; i < 12; i++) {
		rgb_led(scale_keys[i], scale_setup_color_notes_r >> 3, scale_setup_color_notes_g >> 3, scale_setup_color_notes_b >> 3); // Root note selector and scale preview
	}
	for (u8 i = 1; i < scales_length(scale_selected); i++) {
		rgb_led(scale_keys[modulo(scales(scale_selected, i) + scale_root, 12)], scale_setup_color_notes_r, scale_setup_color_notes_g, scale_setup_color_notes_b); // Notes in current scale
	}
	rgb_led(scale_keys[scale_root], scale_setup_color_root_r, scale_setup_color_root_g, scale_setup_color_root_b); // Scale root note

	for (u8 x = 1; x < 5; x++) {
		for (u8 y = 1; y < 9; y++) {
			rgb_led(x * 10 + y, scale_setup_color_scale_r, scale_setup_color_scale_g, scale_setup_color_scale_b); // Select scale
		}
	}
	rgb_led(((scale_selected >> 3) + 1) * 10 + (scale_selected % 8) + 1, scale_setup_color_scale_selected_r, scale_setup_color_scale_selected_g, scale_setup_color_scale_selected_b); // Selected scale
}

void scale_setup_timer_event() {}

void scale_setup_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(mode_setup);

	} else if (p == 96) {
		if (v == 0) {
			novation_led(96, 39);
		} else {
			mode_update(mode_default); // Enter Note/Live mode
			if (mode_default == mode_note) {
				rgb_led(96, 0, 63, 0);
			}
		}

	} else if (p == 80) { // Shift button
		send_midi(2 - mode_default, 0xB0, p, v);
		note_shift = v;
		chord_shift = v;

	} else if (v != 0) {
		if (p == 88) { // Enable or disable Scale mode
			scale_enabled = 1 - scale_enabled;
			scale_setup_init();

		} else if (p == 58) { // Enable or disable translate mode
			translate_enabled = 1 - translate_enabled;
			scale_setup_init();

		} else if (81 <= p && p <= 87) { // Select segment length
			u8 t = 88 - p;
			scale_segment = t == scale_segment? 8 : t;
			scale_setup_init();

		} else if (1 <= x && x <= 4 && 1 <= y && y <= 8) { // Select scale
			scale_selected = (x - 1) * 8 + (y - 1);
			scale_setup_init();

		} else {
			u8 i = 0;
			do {
				if (scale_keys[i] == p) break;
			} while (++i < 12);

			if (i < 12) {
				if (i != scale_root) {
					note_octave = 3;
				}

				scale_root = i;
				scale_setup_init();
			}
		}
	}
}

void scale_setup_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {}

void scale_setup_aftertouch_event(u8 v) {}

void scale_setup_poly_event(u8 p, u8 v) {}
