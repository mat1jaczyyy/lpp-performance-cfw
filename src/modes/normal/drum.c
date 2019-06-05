#include "modes/normal/drum.h"

#define drum_offset_start 9

#define drum_nav_r 0
#define drum_nav_g 0
#define drum_nav_b 63

#define drum_color_pressed_r 0
#define drum_color_pressed_g 63
#define drum_color_pressed_b 0

const u8 drum_colors[9][3] = {{21, 0, 21}, {0, 0, 63}, {63, 15, 0}, {63, 63, 0}, {56, 20, 15}, {0, 41, 63}, {24, 63, 7}, {33, 18, 63}, {21, 0, 7}};
const u8 drum_align[17] = {1, 1, 1, 1, 5, 5, 5, 5, 9, 9, 9, 9, 13, 13, 13, 13, 13};

u8 drum_offset = drum_offset_start;
u8 drum_nav_pressed[4] = {};

s8 drum_press(u8 x, u8 y, u8 v, s8 out_p) {
	u8 n = (drum_offset + x - 1) * 4 + (y - 1) % 4;
	if (y >= 5) n += 32;
	
	if (n == out_p || out_p < 0) {
		u8 p = x * 10 + y;
		
		if (v == 0) { // Note released
			u8 i = (n + 12) / 16;
			rgb_led(p, drum_colors[i][0], drum_colors[i][1], drum_colors[i][2]);
		
		} else { // Note pressed
			rgb_led(p, drum_color_pressed_r, drum_color_pressed_g, drum_color_pressed_b);
		}
	}
	
	return n;
}

void drum_init() {
	for (u8 x = 1; x < 9; x++) { // Regular notes
		for (u8 y = 1; y < 9; y++) {
			drum_press(x, y, 0, -1);
		}
	}
	
	for (u8 i = 0; i < 128; i++) { // Turn off all notes
		hal_send_midi(USBSTANDALONE, 0x81, i, 0);
	}
	
	rgb_led(91, drum_nav_r, drum_nav_g, (drum_offset < 13)? (drum_nav_b >> (1 - drum_nav_pressed[0])) : 0); // Navigation buttons
	rgb_led(92, drum_nav_r, drum_nav_g, (drum_offset > 3)? (drum_nav_b >> (1 - drum_nav_pressed[1])) : 0);
	rgb_led(93, drum_nav_r, drum_nav_g, (drum_offset < 16)? (drum_nav_b >> (1 - drum_nav_pressed[2])) : 0);
	rgb_led(94, drum_nav_r, drum_nav_g, (drum_offset > 0)? (drum_nav_b >> (1 - drum_nav_pressed[3])) : 0);
	
	rgb_led(99, mode_drum_r, mode_drum_g, mode_drum_b); // Drum mode LED
}

void drum_timer_event() {}

void drum_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(mode_setup);
	
	} else if (x == 0 || (x == 9 && y >= 5) || y == 0 || y == 9) { // Unused side buttons
		hal_send_midi(USBSTANDALONE, 0xB1, p, v);
		rgb_led(p, 0, (v == 0)? 0 : 63, 0);
	
	} else if (x == 9 && y <= 4) { // Navigation buttons
		if (v != 0) {
			switch (p) {
				case 91:
					drum_offset += 4 * (drum_offset < 13);
					break;
				
				case 92:
					drum_offset -= 4 * (drum_offset > 3);
					break;
				
				case 93:
					drum_offset += drum_offset < 16;
					break;
				
				case 94:
					drum_offset -= drum_offset > 0;
					break;
			}
		}
		drum_nav_pressed[p - 91] = (v)? 1 : 0;
		
		if (drum_nav_pressed[0] && drum_nav_pressed[1]) { // Reset offset. Note: Undocumented in Programmer's reference
			drum_offset = drum_offset_start;
		} else if (drum_nav_pressed[2] && drum_nav_pressed[3]) { // Align offset
			drum_offset = drum_align[drum_offset];
		}
		drum_init(); // Redraw grid
	
	} else { // Main grid
		hal_send_midi(USBSTANDALONE, (v == 0)? 0x81 : 0x91, drum_press(x, y, v, -1), v);
	}
}

void drum_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
	if (port == USBSTANDALONE && ch == 0x1) {
		u8 x = p / 10;
		u8 y = p % 10;
		
		switch (t) {
			case 0x8:
				v = 0;
			
			case 0x9:
				for (u8 x = 1; x < 9; x++) { // I'm lazy
					for (u8 y = 1; y < 9; y++) {
						drum_press(x, y, v, p);
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

void drum_aftertouch_event(u8 v) {
	aftertouch_send(USBSTANDALONE, 0xD1, v);
}

void drum_poly_event(u8 p, u8 v) {
	poly_send(USBSTANDALONE, 0xA1, drum_press(p / 10, p % 10, v, -1), v);
}