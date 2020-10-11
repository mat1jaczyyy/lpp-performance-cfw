#include "modes/normal/chord.h"

void chord_init() {
	if (mode == mode_chord) rgb_led(99, mode_chord_r, mode_chord_g, mode_chord_b); // Chord mode LED

	active_port = USBSTANDALONE;
}

void chord_timer_event() {}

void chord_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(mode_setup);
	}
}

void chord_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
	if (port == USBSTANDALONE && ch == channels[5]) {
		u8 x = p / 10;
		u8 y = p % 10;

		switch (t) {
			case 0x8:
				v = 0;

			case 0x9:
				break;

			case 0xB:
				break;
		}
	}
}

void chord_aftertouch_event(u8 v) {
	aftertouch_send(USBSTANDALONE, 0xD0 | channels[5], v);
}

void chord_poly_event(u8 p, u8 v) {
	
}
