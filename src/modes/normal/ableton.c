#include "modes/normal/ableton.h"

#define user_r 20
#define user_g 0
#define user_b 63

u8 ableton_screen[100][2] = {};
u8 ableton_layout = 0x0;

void ableton_led(u8 ch, u8 p, u8 v, u8 s) {
	if (ch == 0x5) ch = 0x0;
	if (ch >= 3) return;

	if (mode == mode_ableton) {
		switch (ch) {
			case 0x0: // Normal LED
				novation_led(p, v);
				break;
			
			case 0x1: // Flashing
				flash_led(p, v);
				break;
			
			case 0x2: // Pulsing
				pulse_led(p, v);
				break;
		}
	}

	if (s) {
		ableton_screen[p][0] = ch;
		ableton_screen[p][1] = v;
	}
}

void ableton_init() {
	rgb_led(99, mode_ableton_r, mode_ableton_g, mode_ableton_b); // Live mode LED
	
	for (u8 i = 0; i < 100; i++) {
		ableton_led(ableton_screen[i][0], i, ableton_screen[i][1], 0);
	}
	
	if (ableton_layout == ableton_layout_note_chromatic) note_init();
	if (ableton_layout == ableton_layout_fader_device || ableton_layout == ableton_layout_fader_volume || ableton_layout == ableton_layout_fader_pan || ableton_layout == ableton_layout_fader_sends) fader_init();
	if (ableton_layout == ableton_layout_user) rgb_led(98, user_r, user_g, user_b); // User LED

	active_port = USBMIDI;
}

void ableton_timer_event() {
	if (ableton_layout == ableton_layout_fader_device || ableton_layout == ableton_layout_fader_volume || ableton_layout == ableton_layout_fader_pan || ableton_layout == ableton_layout_fader_sends) {
		fader_timer_event();
	}
}

void ableton_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(mode_setup);	
	} else {
		switch (ableton_layout) {
			case ableton_layout_session: // Session mode
			case ableton_layout_session_arm: // Session mode - Record Arm
			case ableton_layout_session_track: // Session mode - Track Select
			case ableton_layout_session_mute: // Session mode - Mute
			case ableton_layout_session_solo: // Session mode - Solo
			case ableton_layout_session_stop: // Session mode - Stop Clip
				if (x == 0 || x == 9 || y == 0 || y == 9) {
					send_midi(USBMIDI, 0xB0, p, v);
				} else {
					send_midi(USBMIDI, (v == 0)? 0x80 : 0x90, p, v);
				}
				break;
			
			case ableton_layout_note_drum: // Note mode - Drum Rack layout
				if (x == 0 || x == 9 || y == 0 || y == 9) {
					send_midi(USBMIDI, 0xB0, p, v);
				} else if (mode != mode_scale_setup) {
					send_midi(USBMIDI, (v == 0)? 0x80 : 0x90, xy_dr[p], v);
				}
				break;
			
			case ableton_layout_note_chromatic: // Note mode - Chromatic layout
				if (x == 0 || (x == 9 && (y == 5 || y > 6)) || (y == 0 && x < 8) || y == 9) {
					send_midi(USBMIDI, 0xB0, p, v);
				} else {
					note_surface_event(p, v, x, y);
				}
				break;
			
			case ableton_layout_note_blank: // Note mode - Blank layout (for Audio track)
				if (x == 0 || x == 9 || y == 0 || y == 9) {
					send_midi(USBMIDI, 0xB0, p, v);
				}
				break;
			
			case ableton_layout_fader_device: // Fader - Device mode
			case ableton_layout_fader_volume: // Fader - Volume
			case ableton_layout_fader_pan: // Fader - Pan
			case ableton_layout_fader_sends: // Fader - Sends
				if (x == 0 || x == 9 || y == 0 || y == 9) {
					send_midi(USBMIDI, 0xB0, p, v);
				} else {
					fader_surface_event(p, v, x, y);
				}
				break;
			
			case ableton_layout_user: // User mode
				if (x == 9) {
					send_midi(USBMIDI, 0xB5, p, v);
				} else {
					send_midi(USBMIDI, (v == 0)? 0x85 : 0x95, xy_dr[p], v);
				}
				break;
		}
	}
}

void ableton_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
	if (port == USBMIDI) {
		u8 x = p / 10;
		u8 y = p % 10;
		
		if (ableton_layout == ableton_layout_note_chromatic && mode != mode_scale_setup && t != 0xB) {
			note_midi_event(port, t, ch, p, v);
			return;
		}
		
		if ((ableton_layout == ableton_layout_fader_device || ableton_layout == ableton_layout_fader_volume || ableton_layout == ableton_layout_fader_pan || ableton_layout == ableton_layout_fader_sends) && !(x == 0 || x == 9 || y == 0 || y == 9)) {
			fader_midi_event(port, t, ch, p, v);
			return;
		}
		
		switch (t) {
			case 0x8:
				v = 0;
			
			case 0x9:
				if (ableton_layout == ableton_layout_note_drum && (ch == 0x0 || ch == 0x1 || ch == 0x2) && 36 <= p && p <= 99) {
					ableton_led(ch, dr_xy[p], v, 1);
					break;
				}
				if (ableton_layout == ableton_layout_user && (ch == 0x5 || ch == 0x1 || ch == 0x2)) {
					if (36 <= p && p <= 123) ableton_led(ch, dr_xy[p], v, 1);
					break;
				}
				if (ableton_layout == ableton_layout_note_blank && 1 <= x && x <= 8 && 1 <= y && y <= 8) {
					break;
				}
			
			case 0xB:
				if (ableton_layout == ableton_layout_user) {
					if (ch == 0x5) ableton_led(ch, p, v, 1);
				} else {
					if (ch == 0x0 || ch == 0x1 || ch == 0x2) ableton_led(ch, p, v, 1);
				}
				break;
		}
	}
}

void ableton_aftertouch_event(u8 v) {
	aftertouch_send(USBMIDI, (ableton_layout == ableton_layout_user)? 0xD5 : 0xD0, v);
}

void ableton_poly_event(u8 p, u8 v) {
	send_midi(USBMIDI, (ableton_layout == ableton_layout_user)? 0xA5 : 0xA0, p, v);
}