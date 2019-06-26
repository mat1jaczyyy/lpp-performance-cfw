#include "modes/normal/programmer.h"

u8 programmer_screen[100] = {};

void programmer_led(u8 p, u8 v, u8 s) {
	novation_led(p, v);
	if (s) programmer_screen[p] = v;
}

void programmer_init() {
	for (u8 i = 0; i < 100; i++) {
		programmer_led(i, programmer_screen[i], 0);
	}

	rgb_led(99, mode_programmer_r, mode_programmer_g, mode_programmer_b); // Programmer mode LED

	active_port = USBSTANDALONE;
}

void programmer_timer_event() {}

void programmer_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(mode_setup);
	
	} else {
		send_midi(USBSTANDALONE, (x == 0 || x == 9 || y == 0 || y == 9)? 0xB0 : ((v == 0)? 0x80 : 0x90), p, v);
	}
}

void programmer_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
	if (port == USBSTANDALONE && ch == 0x0) { // Channel 1 REQUIRED for FL Studio support
		if (t == 0x8) {
			v = 0; // Note off
			t = 0x9;
		}
		
		u8 x = p / 10;
		u8 y = p % 10;
		
		if ((t == 0xB && (x == 0 || x == 9 || y == 0 || y == 9)) || (t == 0x9 && 1 <= x && x <= 8 && 1 <= y && y <= 8)) {
			novation_led(p, v);
		}
	}
}

void programmer_aftertouch_event(u8 v) {
	aftertouch_send(USBSTANDALONE, 0xD0, v);
}

void programmer_poly_event(u8 p, u8 v) {
	send_midi(USBSTANDALONE, 0xA0, p, v);
}