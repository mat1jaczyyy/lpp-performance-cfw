#include "modes/normal/chord.h"

#define chord_octave_start 4

const u8 chord_octave_colors[5][3] = {{63, 0, 63}, {20, 0, 63}, {0, 0, 63}, {0, 0, 31}, {0, 0, 7}};

u8 chord_octave = chord_octave_start;

u8 chord_shift = 0;
u8 chord_nav_pressed[2] = {};

void chord_single(u8 p[], u8 l, u8 r, u8 g, u8 b) {
	for (u8 i = 0; i < l; i++) { // Used to update LEDs on buttons that trigger the same actual note
		rgb_led(p[i], r, g, b);
	}
}

s8 chord_press(u8 x, u8 y, u8 v, s8 out_p) {
	return 1;
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
	for (u8 x = 1; x < 9; x++) { // Regular notes
		for (u8 y = 1; y < 6; y++) {
			chord_press(x, y, 0, -1);
		}
	}

	for (u8 i = 0; i < 128; i++) { // Turn off all notes
		send_midi(USBSTANDALONE, 0x80 | channels[5], i, 0);
	}

	chord_draw_navigation();
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

	} else if (1 <= x && x <= 8 && 1 <= y && y <= 5) { // Main grid
		s8 n = chord_press(x, y, v, -1);
		if (n >= 0) send_midi(USBSTANDALONE, (v? 0x90 : 0x80) | channels[5], n, v);
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
	
}
