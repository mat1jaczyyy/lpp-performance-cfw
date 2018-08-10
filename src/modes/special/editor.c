#include "modes/special/editor.h"
#include "modes/mode.h"
#include "led/led.h"
#include "led/palettes.h"
#include "flash/flash.h"
#include "flash/settings.h"

u8 editor_selected = 1;

u8 editor_xy_v(u8 xy) {
	return 64 - (xy / 10 * 8) + (xy - 1) % 10 + (editor_selected >> 6) * 64;
}

u8 editor_v_xy(u8 v) {
	return 81 - 10 * ((v % 64) >> 3) + v % 8;
}

void editor_refresh() {
	palette_led(editor_v_xy(editor_selected), editor_selected);
	
	display_u8(editor_selected, 0, 9, 63, 63, 63);
	
	display_u6(palette[palette_selected][0][editor_selected], 1, 0, 63, 0, 0);
	display_u6(palette[palette_selected][1][editor_selected], 0, 0, 0, 63, 0);
	display_u6(palette[palette_selected][2][editor_selected], 1, 9, 0, 0, 63);
}

void editor_draw() {
	for (u8 x = 1; x < 9; x++) {
		for (u8 y = 1; y < 9; y++) {
			u8 xy = x * 10 + y;
			palette_led(xy, editor_xy_v(xy));
		}
	}
	editor_refresh();
}

void editor_select_xy(u8 xy) {
	if (xy != 81 || (editor_selected >> 6) != 0) {
		editor_selected = editor_xy_v(xy);
		editor_refresh();
	} else {
		palette[palette_selected][0][0] = 0;
		palette[palette_selected][1][0] = 0;
		palette[palette_selected][2][0] = 0;
		editor_draw();
	}
}

void editor_select_v(u8 v) {
	if (v != 0) {
		editor_selected = v;
		editor_refresh();
	} else {
		palette[palette_selected][0][0] = 0;
		palette[palette_selected][1][0] = 0;
		palette[palette_selected][2][0] = 0;
		editor_draw();
	}
}

void editor_select_flip(u8 i) {
	u8 v = editor_selected ^ (1 << i);
	
	if (v != 0) {
		editor_selected = v;
		if (i == 6) {
			editor_draw();
		} else {
			editor_refresh();
		}
	} else {
		palette[palette_selected][0][0] = 0;
		palette[palette_selected][1][0] = 0;
		palette[palette_selected][2][0] = 0;
		editor_draw();
	}
}

void editor_init() {
	editor_selected = 1;
	editor_draw();
}

void editor_timer_event() {}

void editor_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (v != 0) {
		if (p == 0) { // Enter Setup mode
			mode_update(mode_setup);
		
		} else if (2 <= x && x <= 7 && y == 0) { // Modify red bit
			palette[palette_selected][0][editor_selected] ^= 1 << (x - 2);
			editor_refresh();
			dirty = 1;
		
		} else if (2 <= p && p <= 7) { // Modify green bit
			palette[palette_selected][1][editor_selected] ^= 1 << (7 - p);
			editor_refresh();
			dirty = 1;
		
		} else if (2 <= x && x <= 7 && y == 9) { // Modify blue bit
			palette[palette_selected][2][editor_selected] ^= 1 << (x - 2);
			editor_refresh();
			dirty = 1;
		
		} else if (92 <= p && p <= 98) { // Modify velocity bit
			editor_select_flip(98 - p);
		
		} else if (p != 1 && p != 8 && p != 10 && p != 19 && p != 80 && p != 89 && p != 91) { // Select velocity on grid
			editor_select_xy(p);
		}
	}
}

void editor_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {}