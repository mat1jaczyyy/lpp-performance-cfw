#include "modes/normal/note.h"

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

const u8 note_octave_colors[10][3] = {{63, 0, 63}, {20, 0, 63}, {0, 0, 63}, {0, 0, 31}, {0, 0, 7}, {0, 0, 31}, {0, 0, 63}, {20, 0, 63}, {40, 0, 63}, {63, 0, 63}};
const u8 note_transpose_colors[13][3] = {{0, 7, 0}, {0, 21, 0}, {0, 31, 0}, {0, 42, 0}, {0, 52, 0}, {0, 63, 0}, {15, 63, 0}, {23, 63, 0}, {31, 63, 0}, {39, 63, 0}, {47, 63, 0}, {55, 63, 0}, {63, 63, 0}};

const u8 scales[32][13] = {
	{5, 0, 1, 5, 7, 10}, // In Sen
	{5, 0, 2, 5, 7, 9}, // Yo scale
	{5, 0, 1, 5, 6, 10}, // Iwato
	{8, 0, 2, 3, 5, 6, 8, 9, 11}, // Whole Half
	{8, 0, 2, 3, 5, 7, 8, 10, 11}, // BeBop Minor
	{6, 0, 2, 3, 4, 7, 9}, // Major blues
	{5, 0, 2, 3, 7, 9}, // Kumoi
	{8, 0, 2, 4, 5, 7, 8, 9, 11}, // BeBop Major
	{7, 0, 2, 4, 6, 7, 9, 11}, // Lydian
	{7, 0, 1, 3, 5, 6, 8, 10}, // Locrian
	{5, 0, 2, 4, 7, 9}, // Major Pentatonic
	{7, 0, 1, 4, 5, 7, 8, 10}, // Phyrigian Dominant
	{8, 0, 1, 3, 4, 6, 7, 9, 10}, // Half-Whole Diminished
	{8, 0, 2, 4, 5, 7, 9, 10, 11}, // Mixolydian BeBop
	{7, 0, 1, 3, 4, 6, 8, 10}, // Super Locrian
	{5, 0, 2, 3, 6, 7}, // Hirajoshi
	{6, 0, 3, 5, 6, 7, 10}, // Blues
	{5, 0, 3, 5, 7, 10}, // Minor Pentatonic
	{7, 0, 2, 3, 6, 7, 8, 11}, // Hungarian Minor
	{7, 0, 2, 3, 6, 7, 9, 10}, // Ukrainian Dorian
	{7, 0, 1, 4, 6, 7, 9, 11}, // Marva
	{7, 0, 1, 3, 5, 6, 7, 11}, // Todi
	{6, 0, 2, 4, 6, 8, 10}, // Whole Tone
	{12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, // Chromatic
	{7, 0, 2, 3, 5, 7, 8, 10}, // Minor
	{7, 0, 2, 4, 5, 7, 9, 11}, // Major
	{7, 0, 2, 3, 5, 7, 9, 10}, // Dorian
	{7, 0, 1, 3, 5, 7, 8, 10}, // Phrygian
	{7, 0, 2, 4, 5, 7, 9, 10}, // Mixolydian
	{7, 0, 2, 3, 5, 7, 9, 11}, // Melodic Minor (ascending)
	{7, 0, 2, 3, 5, 7, 8, 11}, // Harmonic Minor
	{8, 0, 2, 3, 4, 5, 7, 9, 10} // BeBop Dorian
};
const u8 scale_keys[12] = {51, 62, 52, 63, 53, 54, 65, 55, 66, 56, 67, 57};

s8 note_octave = 3;
s8 note_transpose = 0;
u8 note_shift = 0;

u8 scale_enabled = 0;
u8 translate_enabled = 1;
u8 scale_segment = 7;
u8 scale_selected = 25;
u8 scale_root = 0;

void note_single(u8 *p, u8 l, u8 r, u8 g, u8 b) {
	for (u8 i = 0; i < l; i++) { // Used to update LEDs on buttons that trigger the same actual note
		rgb_led(*(p + i), r, g, b);
	}
}

s8 note_press(u8 x, u8 y, u8 v, s8 out_p) {
	u8 length = (scale_enabled)? scales[scale_selected][0] : note_length;
	u8 segment = (scale_enabled)? scale_segment : note_segment;

	u8 offset = (x - 1) * segment + (y - 1); // Note pressed in relation to lowest
	u8 up = offset / length; // Octaves above lowest

	offset %= length; // Note pressed in relation to its octave
	if (scale_enabled) offset = scales[scale_selected][1 + offset]; // Translate for Scale mode

	s8 c = (note_octave + up) * 12 + offset; // Note in relation to C
	s8 n = c + note_transpose + ((scale_enabled)? scale_root : 0); // Actual note (transposition applied)

	if (n == out_p || out_p < 0) {
		u8 p[8] = {x * 10 + y}; // Pitches to update on the Launchpad
		u8 l = 1;

		for (u8 i = 0; i < 7; i++) { // Add pitches above
			s8 e = x + i;
			s8 f = y - segment * i;

			if (1 <= e && e <= 8 && 1 <= f && f <= 8) {
				p[l++] = e * 10 + f; // Append to array
			} else {
				break;
			}
		}

		for (u8 i = 0; i < 7; i++) { // Add pitches below
			s8 e = x - i;
			s8 f = y + segment * i;

			if (1 <= e && e <= 8 && 1 <= f && f <= 8) {
				p[l++] = e * 10 + f;
			} else {
				break;
			}
		}

		if (n < 0) { // Invalid note - also affects notes larger than 127 due to overflow!
			note_single(&p[0], l, note_color_invalid_r, note_color_invalid_g, note_color_invalid_b);

		} else { // Valid note
			if (v == 0) { // Note released
				u8 m = modulo(c, 12); // Note without octave

				if (scale_enabled) {
					if (m == 0) {
						note_single(&p[0], l, note_color_scale_base_r, note_color_scale_base_g, note_color_scale_base_b);
					} else {
						note_single(&p[0], l, note_color_scale_r, note_color_scale_g, note_color_scale_b);
					}

				} else {

					if (translate_enabled) {
						// Color notes in relation to SCALE, not absolute!
						u8 local_m = modulo(12 + m - scale_root, 12); // Move relative to root note
						if (local_m == 0) {
							note_single(&p[0], l, note_color_base_r, note_color_base_g, note_color_base_b);
						} else {
							u8 out_of_scale = 1;
							for(u8 i = 1; i <= scales[scale_selected][0]; ++i) {
								if(local_m == scales[scale_selected][i]) {
									// White note
									note_single(&p[0], l, note_color_white_r, note_color_white_g, note_color_white_b);
									out_of_scale = 0;
									break;
								} else if(local_m < scales[scale_selected][i]) {
									break;
								}
							}
							if (out_of_scale) {
								// Black note
								note_single(&p[0], l, note_color_black_r, note_color_black_g, note_color_black_b);
							}
						}
					} else {
						// Previous mode, simple C major colors
						switch (m) {
							case 0: // C base note
								note_single(&p[0], l, note_color_base_r, note_color_base_g, note_color_base_b);
								break;

							case 2:
							case 4:
							case 5:
							case 7:
							case 9:
							case 11: // White note
								note_single(&p[0], l, note_color_white_r, note_color_white_g, note_color_white_b);
								break;

							default: // Black note
								note_single(&p[0], l, note_color_black_r, note_color_black_g, note_color_black_b);
								break;
						}
					}

					u8 b = modulo(note_transpose, length); // Base note of scale
					if (b != 0 && b == m) note_single(&p[0], l, note_color_transposed_r, note_color_transposed_g, note_color_transposed_b); // Scale base note
				}

			} else { // Note pressed
				if (out_p < 0) {
					note_single(&p[0], l, note_color_pressed_r, note_color_pressed_g, note_color_pressed_b);
				} else {
					note_single(&p[0], l, palette_value(palette_novation, v, 0), palette_value(palette_novation, v, 1), palette_value(palette_novation, v, 2));
				}
			}
		}
	}

	return n;
}

void note_scale_button() {
	if (note_shift) { // Shift button pressed
		rgb_led(80, 63, 63, 63);
		rgb_led(96, 7, 7, 7);

	} else { // Shift button released
		rgb_led(80, 7, 7, 7);
		if (mode_default == mode_ableton) {
			rgb_led(96, 0, 63, 63);
		} else if (mode_default == mode_note) {
			rgb_led(96, 0, 0, 0);
		}
	}
}

void note_draw() {
	for (u8 x = 1; x < 9; x++) { // Regular notes
		for (u8 y = 1; y < 9; y++) {
			note_press(x, y, 0, -1);
		}
	}

	for (u8 i = 0; i < 128; i++) { // Turn off all notes
		send_midi(2 - mode_default, 0x80, i, 0);
	}

	u8 o = note_octave + 1; // Octave navigation
	if (o < 5) {
		rgb_led(91, note_octave_colors[4][0], note_octave_colors[4][1], note_octave_colors[4][2]);
		rgb_led(92, note_octave_colors[o][0], note_octave_colors[o][1], note_octave_colors[o][2]);
	} else {
		rgb_led(91, note_octave_colors[o][0], note_octave_colors[o][1], note_octave_colors[o][2]);
		rgb_led(92, note_octave_colors[4][0], note_octave_colors[4][1], note_octave_colors[4][2]);
	}

	if (note_transpose > 0) { // Transpose navigation
		rgb_led(93, note_transpose_colors[0][0], note_transpose_colors[0][1], note_transpose_colors[0][2]);
		rgb_led(94, note_transpose_colors[note_transpose][0], note_transpose_colors[note_transpose][1], note_transpose_colors[note_transpose][2]);
	} else {
		rgb_led(93, note_transpose_colors[-note_transpose][0], note_transpose_colors[-note_transpose][1], note_transpose_colors[-note_transpose][2]);
		rgb_led(94, note_transpose_colors[0][0], note_transpose_colors[0][1], note_transpose_colors[0][2]);
	}
}

void note_init() {
	note_draw();
	note_scale_button();
	if (mode == mode_note) rgb_led(99, mode_note_r, mode_note_g, mode_note_b); // Note mode LED

	active_port = USBSTANDALONE;
}

void note_timer_event() {}

void note_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(mode_setup);

	} else if (p == 96 && note_shift && v != 0) { // Shift+Note button
		mode_update(mode_scale_setup); // Enter Setup for Scale mode
		rgb_led(p, 63, 63, 63);

	} else if (p == 96 && v == 0) {
		note_scale_button();

	} else if (x == 0 || y == 9 || (y == 0 && x < 8) || (x == 9 && y > 4)) { // Unused side buttons
		send_midi(2 - mode_default, 0xB0, p, v);
		rgb_led(p, 0, (v == 0)? 0 : 63, 0);

	} else if (p == 80) { // Shift button
		send_midi(2 - mode_default, 0xB0, p, v);
		note_shift = v;
		note_scale_button();

	} else if (x == 9 && y < 5) { // Navigation buttons
		if (v != 0) {
			if (scale_enabled && note_shift) {
				switch (p) {
					case 93: // Segment down
						if (scale_segment > 1) scale_segment--;
						break;

					case 94: // Segment up
						if (scale_segment < 7) scale_segment++;
						break;
				}
			} else {
				switch (p) {
					case 91: // Octave up
						if (note_octave < 8) note_octave++;
						break;

					case 92: // Octave down
						if (note_octave > -1) note_octave--;
						break;

					case 93: // Transpose down
						if (note_transpose > -12) note_transpose--;
						break;

					case 94: // Transpose up
						if (note_transpose < 12) note_transpose++;
						break;
				}
			}
			note_draw();
		}

	} else { // Main grid
		s8 n = note_press(x, y, v, -1);
		if (n >= 0) send_midi(2 - mode_default, (v == 0)? 0x80 : 0x90, n, v);
	}
}

void note_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
	if (port == 2 - mode_default && ch == 0x0) {
		u8 x = p / 10;
		u8 y = p % 10;

		switch (t) {
			case 0x8:
				v = 0;

			case 0x9:
				for (u8 x = 1; x < 9; x++) { // I'm lazy
					for (u8 y = 1; y < 9; y++) {
						note_press(x, y, v, p);
					}
				}
				break;

			case 0xB:
				if (x == 0 || (x == 9 && y > 4) || y == 0 || y == 9) {
					novation_led(p, v);
				}
				break;
		}
	}
}

void note_aftertouch_event(u8 v) {
	aftertouch_send(USBSTANDALONE, 0xD0, v);
}

void note_poly_event(u8 p, u8 v) {
	s8 n = note_press(p / 10, p % 10, v, -1);
	if (n >= 0) send_midi(USBSTANDALONE, 0xA0, n, v);
}
