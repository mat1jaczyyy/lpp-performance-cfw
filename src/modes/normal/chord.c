#include "modes/normal/chord.h"

#define chord_color_empty_r 0
#define chord_color_empty_g 0
#define chord_color_empty_b 0

#define chord_color_major_dominant_r 0
#define chord_color_major_dominant_g 0
#define chord_color_major_dominant_b 7

#define chord_color_minor_r 2
#define chord_color_minor_g 0
#define chord_color_minor_b 7

#define chord_color_diminished_r 0
#define chord_color_diminished_g 7
#define chord_color_diminished_b 2

#define chord_color_triad_r 7
#define chord_color_triad_g 2
#define chord_color_triad_b 0

#define chord_color_pressed_r 63
#define chord_color_pressed_g 63
#define chord_color_pressed_b 63

#define chord_octave_start 4

const u8 chord_octave_colors[5][3] = {{63, 0, 63}, {20, 0, 63}, {0, 0, 63}, {0, 0, 31}, {0, 0, 7}};
const u8 chord_map[5][5] = {
	{0, 4, 14, 11, 16},
	{0, 4, 10, 7, 12},
	{0, 4, 9, 6, 11},
	{0, 4, 8, 5, 10},
	{0, 4, 7, 3, 6}
};

s8 chord_octave = chord_octave_start;

u8 chord_shift = 0;
u8 chord_nav_pressed[2] = {};

u8 chord_kind[8] = {};
u8 chord_triads = 8;

s8 chord_value(u8 x, u8 y) {
	u8 l = scales_length(scale_selected);
	u8 i = chord_octave * l + (x - 1) + chord_map[l > 8? 0: 9 - l][y - 1];
	s8 n = 12 * (i / l) + scales(scale_selected, i % l) + scale_root;

	return n;
}

s8 chord_press_result[3] = {};

u8 chord_press(u8 x, u8 y, u8 v, s8 out_p) {
	u8 p = x * 10 + y;

	if (chord_kind[x - 1] == 0 || (y == 6 && x > chord_triads)) {
		rgb_led(p, chord_color_empty_r, chord_color_empty_g, chord_color_empty_b);
		return 0;
	}

	u8 root2 = scales_length(scale_selected) + 1;
	s8 n = 0;
	s8 r;

	if (y == 6) {
		u8 b = chord_value(1, 1) + 12;

		chord_press_result[0] = chord_value(x, 1) + 12;
		chord_press_result[1] = chord_value(x, 3);
		chord_press_result[2] = chord_value(x, 2) + 12;
		
		for (u8 i = 1; i < 3; i++) {
			while (chord_press_result[i] >= b + 12)
				chord_press_result[i] -= 12;
		}
		
		for (u8 i = 0; i < 3; i++) {
			if (chord_press_result[i] < 0) {
				rgb_led(p, chord_color_empty_r, chord_color_empty_g, chord_color_empty_b);
				return 0;
			}
		}
		
		n = 3;

	} else {
		r = chord_value(x, y);
		n = 1;

		if (out_p < 0)
			chord_press_result[0] = r;
	}
	
	#define brighten(x) x = 7 - ((7 - x) * 3 / 4)
	#define brighten_all if (x == 1 || x == root2) { brighten(r); brighten(g); brighten(b); }

	if (out_p < 0) {
		for (u8 i = 0; i < n; i++)
			for (u8 _x = 1; _x < 9; _x++) { // I'm lazy
				for (u8 _y = 1; _y < 6; _y++) {
					chord_press(_x, _y, v, chord_press_result[i]);
				}
			}
		
		if (y == 6) {
			if (v) rgb_led(p, chord_color_pressed_r, chord_color_pressed_g, chord_color_pressed_b);
			else {
				u8 r = chord_color_triad_r, g = chord_color_triad_g, b = chord_color_triad_b;

				brighten_all

				rgb_led(p, r, g, b);
			}
		}

	} else if (n == 1 && r == out_p) {
		if (v) {
			rgb_led(p, chord_color_pressed_r, chord_color_pressed_g, chord_color_pressed_b);

		} else {
			u8 r, g, b;

			switch (chord_kind[x - 1]) {
				case 1:
					r = chord_color_major_dominant_r; g = chord_color_major_dominant_g; b = chord_color_major_dominant_b;
					break;
				
				case 2:
					r = chord_color_minor_r; g = chord_color_minor_g; b = chord_color_minor_b;
					break;
				
				default:
					r = chord_color_diminished_r; g = chord_color_diminished_g; b = chord_color_diminished_b;
					break;
			}

			brighten_all

			rgb_led(p, r, g, b);
		}
	}

	return n;
}

void chord_draw_navigation() {
	u8 o = chord_octave > 4? 8 - chord_octave: chord_octave;
	u8 u = chord_octave > 4? o : 4;
	u8 d = chord_octave < 4? o : 4;

	rgb_led(91, chord_octave_colors[u][0], chord_octave_colors[u][1], chord_octave_colors[u][2]);
	rgb_led(92, chord_octave_colors[d][0], chord_octave_colors[d][1], chord_octave_colors[d][2]);
}

void chord_scale_button() {
	if (chord_shift) { // Shift button pressed
		rgb_led(80, 63, 63, 63);
		rgb_led(96, 7, 7, 7);

	} else { // Shift button released
		rgb_led(80, 7, 7, 7);
		rgb_led(96, 0, 0, 0);
	}
}

void chord_draw() {
	chord_triads = 8;

	for (u8 x = 1; x < 9; x++) { // Regular notes
		for (u8 y = 1; y < 6; y++) {
			s8 r = chord_value(x, y);
			if (y < 4) chord_press_result[y - 1] = r;

			if (chord_triads >= 8 && (s8)(r + 12) < 0)
				chord_triads = x - 1;

			if (r < 0) {
				chord_kind[x - 1] = 0;
				goto next;
			}
		}

		chord_press_result[2] -= 12;
		s8 f = chord_press_result[2] - chord_press_result[0];
		s8 s = chord_press_result[1] - chord_press_result[2];

		if (f == 4 && s == 3) // Major
			chord_kind[x - 1] = 1;

		else if (f == 3 && s == 4) // Minor
			chord_kind[x - 1] = 2;
		
		else chord_kind[x - 1] = 3; // Has to be Diminished

		next:
		for (u8 y = 1; y < 7; y++) {
			chord_press(x, y, 0, -1);
		}
	}

	for (u8 i = 0; i < 128; i++) { // Turn off all notes
		send_midi(USBSTANDALONE, 0x80 | channels[5], i, 0);
	}

	chord_draw_navigation();
}

void chord_send(u8 x, u8 y, u8 v, u8 t) {
	u8 n = chord_press(x, y, v, -1);

	for (u8 i = 0; i < n; i++)
		send_midi(USBSTANDALONE, t | channels[5], chord_press_result[i], v);
}

void chord_init() {
	chord_draw();
	chord_scale_button();
	if (mode == mode_chord) rgb_led(99, mode_chord_r, mode_chord_g, mode_chord_b); // Chord mode LED

	active_port = USBSTANDALONE;
}

void chord_timer_event() {}

void chord_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(mode_setup);
	
	} else if (p == 96 && chord_shift && v != 0) { // Shift+Note button
		mode_update(mode_scale_setup); // Enter Setup for Scale mode
		rgb_led(p, 63, 63, 63);

	} else if (p == 96 && v == 0) {
		chord_scale_button();

	} else if (p == 80) { // Shift button
		chord_shift = v;
		chord_scale_button();

	} else if (x == 9 && y < 5) { // Navigation buttons
		if (v != 0) {
			switch (p) {
				case 91: // Octave up
					if (chord_octave < 8) chord_octave++;
					break;

				case 92: // Octave down
					if (chord_octave > 0) chord_octave--;
					break;
			}
		}

		chord_nav_pressed[p - 91] = (v)? 1 : 0;
		
		if (chord_nav_pressed[0] && chord_nav_pressed[1]) { // Reset offset
			chord_octave = chord_octave_start;
		}

		chord_draw();

	} else if (1 <= x && x <= 8 && 1 <= y && y <= 6) { // Main grid
		chord_send(x, y, v, v? 0x90 : 0x80);
	}
}

void chord_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
	if (port == USBSTANDALONE && ch == channels[5]) {
		switch (t) {
			case 0x8:
				v = 0;

			case 0x9:
				for (u8 x = 1; x < 9; x++) { // I'm lazy
					for (u8 y = 1; y < 6; y++) {
						chord_press(x, y, v, p);
					}
				}
				break;
		}
	}
}

void chord_aftertouch_event(u8 v) {
	aftertouch_send(USBSTANDALONE, 0xD0 | channels[5], v);
}

void chord_poly_event(u8 p, u8 v) {
	chord_send(p / 10, p % 10, v, 0xA0);
}
