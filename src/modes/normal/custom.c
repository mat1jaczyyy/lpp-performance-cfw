#include "modes/normal/custom.h"

u8 active_slot = 0;

void custom_init() {
	rgb_led(99, mode_custom_r, mode_custom_g, mode_custom_b); // Custom mode LED

    for (u8 i = 0; i < 8; i++)
        novation_led(89 - i * 10, (active_slot == i)? 77 : 1);

	active_port = USBSTANDALONE;
}

void custom_timer_event() {}

void custom_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(mode_setup);
	
	} else {
		if (y == 9 && 1 <= x && x <= 8) {
            if (v != 0) {
                active_slot = 8 - x;
                custom_init();
            }
        }
	}
}

void custom_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
	if (port == USBSTANDALONE && ch == 0x4) {
		if (t == 0x8) {
			v = 0; // Note off
			t = 0x9;
		}
		
		u8 x = p / 10;
		u8 y = p % 10;
		
		
	}
}

void custom_aftertouch_event(u8 v) {}

void custom_poly_event(u8 p, u8 v) {}