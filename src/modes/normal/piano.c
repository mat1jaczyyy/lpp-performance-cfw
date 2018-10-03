#include "modes/normal/piano.h"

#define piano_color_invalid_r 7
#define piano_color_invalid_g 0
#define piano_color_invalid_b 0

#define piano_color_base_r 63
#define piano_color_base_g 0
#define piano_color_base_b 63

#define piano_color_white_r 0
#define piano_color_white_g 41
#define piano_color_white_b 63

#define piano_color_black_r 0
#define piano_color_black_g 10
#define piano_color_black_b 63

#define piano_color_empty_r 0
#define piano_color_empty_g 0
#define piano_color_empty_b 0

#define piano_color_pressed_r 0
#define piano_color_pressed_g 63
#define piano_color_pressed_b 0

const u8 piano_octave_colors[10][3] = {{63, 0, 63}, {20, 0, 63}, {0, 0, 63}, {0, 0, 31}, {0, 0, 7}, {0, 0, 31}, {0, 0, 63}, {20, 0, 63}, {40, 0, 63}, {63, 0, 63}};
const u8 piano_transpose_colors[8][3] = {{0, 7, 0}, {0, 21, 0}, {0, 42, 0}, {0, 63, 0}, {23, 63, 0}, {41, 63, 0}, {55, 63, 0}, {63, 63, 0}};

const s8 piano_map[2][8] = {
	{0, 2, 4, 5, 7, 9, 11, 12},
	{-1, 1, 3, -1, 6, 8, 10, -1}
};

s8 piano_octave = 1;
s8 piano_transpose = 0;

void piano_single(u8 *p, u8 l, u8 r, u8 g, u8 b) {
	for (u8 i = 0; i < l; i++) { // Used to update LEDs on buttons that trigger the same actual note
		rgb_led(*(p + i), r, g, b);
	}
}

s8 piano_press(u8 x, u8 y, u8 v, s8 out_p) {
	s8 a = y - 1 + piano_transpose;
	s8 c = piano_map[(x - 1) % 2][modulo(a, 7)];
	s8 n = -1; u8 e = 0;

	if (c == -1) {
		e = 1;
	} else {
		n = (piano_octave + ((x - 1) >> 1) + ((a + 7) / 7 - 1) + 2) * 12 + c; // Actual note (transposition applied)
	}

	if (n == out_p || out_p < 0) {
		u8 p[2] = {x * 10 + y}; // Pitches to update on the Launchpad
		u8 l = 1;

		if (y == 1 && !((x - 1) % 2) && x > 2) { 
			l = 2;
			p[1] = p[0] - 13;

		} else if (y == 8 && !((x - 1) % 2) && x < 7) { 
			l = 2;
			p[1] = p[0] + 13;
		}
		
		if (e) { // Unused note
			piano_single(&p[0], l, piano_color_empty_r, piano_color_empty_g, piano_color_empty_b);

		} else if (n < 0) { // Invalid note - also affects notes larger than 127 due to overflow!
			piano_single(&p[0], l, piano_color_invalid_r, piano_color_invalid_g, piano_color_invalid_b);
			
		} else { // Valid note
			if (v == 0) { // Note released
				u8 m = modulo(n, 12); // Note without octave
				
				switch (m) {
					case 0: // C base note
						piano_single(&p[0], l, piano_color_base_r, piano_color_base_g, piano_color_base_b);
						break;
					
					case 2:
					case 4:
					case 5:
					case 7:
					case 9:
					case 11: // White note
						piano_single(&p[0], l, piano_color_white_r, piano_color_white_g, piano_color_white_b);
						break;
					
					default: // Black note
						piano_single(&p[0], l, piano_color_black_r, piano_color_black_g, piano_color_black_b);
						break;
				}
			
			} else { // Note pressed
				if (out_p < 0) {
					piano_single(&p[0], l, piano_color_pressed_r, piano_color_pressed_g, piano_color_pressed_b);
				} else {
					piano_single(&p[0], l, palette_value(palette_novation, v, 0), palette_value(palette_novation, v, 1), palette_value(palette_novation, v, 2));
				}
			}
		}
	}
	
	return n;
}

void piano_draw() {
	for (u8 x = 1; x < 9; x++) { // Regular notes
		for (u8 y = 1; y < 9; y++) {
			piano_press(x, y, 0, -1);
		}
	}
	
	for (u8 i = 0; i < 128; i++) { // Turn off all notes
		hal_send_midi(2 - mode_default, 0x80, i, 0);
	}
	
	u8 o = piano_octave + 3; // Octave navigation
	if (o < 5) {
		rgb_led(91, piano_octave_colors[4][0], piano_octave_colors[4][1], piano_octave_colors[4][2]);
		rgb_led(92, piano_octave_colors[o][0], piano_octave_colors[o][1], piano_octave_colors[o][2]);
	} else {
		rgb_led(91, piano_octave_colors[o][0], piano_octave_colors[o][1], piano_octave_colors[o][2]);
		rgb_led(92, piano_octave_colors[4][0], piano_octave_colors[4][1], piano_octave_colors[4][2]);
	}
	
	if (piano_transpose > 0) { // Transpose navigation
		rgb_led(93, piano_transpose_colors[0][0], piano_transpose_colors[0][1], piano_transpose_colors[0][2]);
		rgb_led(94, piano_transpose_colors[piano_transpose][0], piano_transpose_colors[piano_transpose][1], piano_transpose_colors[piano_transpose][2]);
	} else {
		rgb_led(93, piano_transpose_colors[-piano_transpose][0], piano_transpose_colors[-piano_transpose][1], piano_transpose_colors[-piano_transpose][2]);
		rgb_led(94, piano_transpose_colors[0][0], piano_transpose_colors[0][1], piano_transpose_colors[0][2]);
	}
}

void piano_init() {
	piano_draw();
	if (mode == mode_piano) rgb_led(99, mode_piano_r, mode_piano_g, mode_piano_b); // Note mode LED
}

void piano_timer_event() {}

void piano_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(mode_setup);
	
	} else if (x == 0 || y == 9 || y == 0 || (x == 9 && y > 4)) { // Unused side buttons
		hal_send_midi(2 - mode_default, 0xB0, p, v);
		rgb_led(p, 0, (v == 0)? 0 : 63, 0);
	
	} else if (x == 9 && y < 5) { // Navigation buttons
		if (v != 0) {
			switch (p) {
				case 91: // Octave up
					if (piano_octave < 6) piano_octave++;
					break;
				
				case 92: // Octave down
					if (piano_octave > -3) piano_octave--;
					break;
				
				case 93: // Transpose down
					if (piano_transpose > -7) piano_transpose--;
					break;
				
				case 94: // Transpose up
					if (piano_transpose < 7) piano_transpose++;
					break;
			}
			piano_draw();
		}
	
	} else { // Main grid
		s8 n = piano_press(x, y, v, -1);
		if (n >= 0) hal_send_midi(USBSTANDALONE, (v == 0)? 0x83 : 0x93, n, v);
	}
}

void piano_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
	if (port == USBSTANDALONE && ch == 0x3) {
		u8 x = p / 10;
		u8 y = p % 10;
		
		switch (t) {
			case 0x8:
				v = 0;
			
			case 0x9:
				for (u8 x = 1; x < 9; x++) { // I'm lazy
					for (u8 y = 1; y < 9; y++) {
						piano_press(x, y, v, p);
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