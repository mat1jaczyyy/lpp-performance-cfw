#include "modes/normal/performance.h"

u8 performance_screen[100][4] = {};

void performance_led(u8 ch, u8 p, u8 v, u8 s) {
	switch (ch) {
		case 0xB:
			flash_led(p, v);
			break;

		case 0xC:
			pulse_led(p, v);
			break;

		case 0xF:
			palette_led(p, v);
			break;
	}

	if ((ch == 0xB || ch == 0xC || ch == 0xF) && s) {
		performance_screen[p][0] = ch;
		for (int i = 0; i < 3; i++) {
			performance_screen[p][i + 1] = palette_value(palette_selected, v, i);
		}
	}
}

void performance_led_rgb(u8 ch, u8 p, u8 r, u8 g, u8 b, u8 s) {
	switch (ch) {
		case 0xB:
			flash_rgb_led(p, r, g, b);
			break;

		case 0xC:
			pulse_rgb_led(p, r, g, b);
			break;

		case 0xF:
			rgb_led(p, r, g, b);
			break;
	}

	if ((ch == 0xB || ch == 0xC || ch == 0xF) && s) {
		performance_screen[p][0] = ch;
		performance_screen[p][1] = r;
		performance_screen[p][2] = g;
		performance_screen[p][3] = b;
	}
}

void performance_init() {
	for (u8 i = 0; i < 100; i++) {
		performance_led_rgb(performance_screen[i][0], i, performance_screen[i][1], performance_screen[i][2], performance_screen[i][3], 0);
	}
	
	if (!performance_screen[98][1] && !performance_screen[98][2] && !performance_screen[98][3]) rgb_led(98, mode_performance_r, mode_performance_g, mode_performance_b); // Performance User LED

	send_midi(USBSTANDALONE, 0xB0, 121, 0); // Reset All Controllers message, triggers Multi Reset

	active_port = USBSTANDALONE;
}

void performance_timer_event() {}

void performance_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(mode_setup);
				
	} else { // Send MIDI input to DAW
		send_midi(USBSTANDALONE, (v == 0)? 0x8F : 0x9F, (performance_xy_enabled)? p : xy_dr[p], v);
	}
}

void performance_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
	if(t != 0x9 && t != 0x8) return; // Checks for Note
	
	if (port == USBSTANDALONE && ch >= 0xA)  // Deleted the Switch and replaced it
	{
		if(t == 0x8) v = 0; // We cannot assume a note off will come with velocity 0. Note, there is no break statement here!
			
		if (performance_xy_enabled) { // XY layout
			performance_led(ch, p, v, 1);

		} else { // Drum Rack layout
			if (top_lights_config != 0) {
				if (108 <= p && p <= 115) { // Conversion of MK2 Top Lights
					p += -80;
				}
						
				if (top_lights_config > 1) { // Display additional LEDs
					if (100 <= p && p <= 107) {
						performance_led(ch, dr_xy[(top_lights_config == 2)? (215 - p) : (16 + p)], v, 1);
					} else if (28 <= p && p <= 35) { // p has been changed from the earlier if statement, so we must check for [28, 35] now!
						performance_led(ch, dr_xy[(top_lights_config == 2)? (151 - p) : (80 + p)], v, 1);
					}
				}
			}
					
			performance_led(ch, dr_xy[p], v, 1);
		}
	}
}

void performance_aftertouch_event(u8 v) {
	aftertouch_send(USBSTANDALONE, 0xDF, v);
}

void performance_poly_event(u8 p, u8 v) {
	send_midi(USBSTANDALONE, 0xAF, (performance_xy_enabled)? p : xy_dr[p], v);
}