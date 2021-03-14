#include "modes/special/editor.h"

u8 editor_palette[3][128];

u8 editor_selected = 1;

#define editor_export_speed 4
#define editor_export_max 0x80

u8 editor_export_do = 0;
u8 editor_export_counter = 0;

u8 editor_xy_v(u8 xy) {
	return 64 - (xy / 10 * 8) + (xy - 1) % 10 + (editor_selected >> 6) * 64;
}

u8 editor_v_xy(u8 v) {
	return 81 - 10 * ((v % 64) >> 3) + v % 8;
}

void editor_refresh() {
	rgb_led(editor_v_xy(editor_selected), editor_palette[0][editor_selected], editor_palette[1][editor_selected], editor_palette[2][editor_selected]);
	
	display_u8(editor_selected, 0, 9, 63, 63, 63);
	
	display_u6(palette_value(settings.palette_selected, editor_selected, 0), 1, 0, 63, 0, 0);
	display_u6(palette_value(settings.palette_selected, editor_selected, 1), 0, 0, 0, 63, 0);
	display_u6(palette_value(settings.palette_selected, editor_selected, 2), 1, 9, 0, 0, 63);
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
		editor_palette[0][0] = 0;
		editor_palette[1][0] = 0;
		editor_palette[2][0] = 0;
		editor_draw();
	}
}

void editor_select_v(u8 v) {
	if (v != 0) {
		editor_selected = v;
		editor_refresh();
	} else {
		editor_palette[0][0] = 0;
		editor_palette[1][0] = 0;
		editor_palette[2][0] = 0;
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
		editor_palette[0][0] = 0;
		editor_palette[1][0] = 0;
		editor_palette[2][0] = 0;
		editor_draw();
	}
}

void editor_export() {
	editor_export_do = 1;
	editor_export_counter = 0;
}

void editor_init() {
	memcpy(editor_palette, palette_get(settings.palette_selected), sizeof(editor_palette));
	editor_selected = 1;
	editor_draw();
}

void editor_timer_event() {
	if (editor_export_do) {
		syx_resp_cpy(palette_header);
		
		for (u8 i = 0; i < editor_export_speed; i++) {
			syx_response_buffer[sizeof(syx_palette_header) + 1] = editor_export_counter;

			for (u8 j = 0; j < 3; j++)
				syx_response_buffer[sizeof(syx_palette_header) + 2 + j] = editor_palette[j][editor_export_counter];

			syx_response_buffer[sizeof(syx_palette_header) + 5] = 0xF7;
			syx_send(USBSTANDALONE, sizeof(syx_palette_header) + 6);
			
			if (++editor_export_counter >= editor_export_max) {
				editor_export_do = 0;
				return;
			}
		}
	}
}

void editor_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (!editor_export_do) {
		if (v != 0) {
			if (p == 0) { // Enter Setup mode
				mode_update(mode_setup);
			
			} else if (2 <= x && x <= 7 && y == 0) { // Modify red bit
				editor_palette[0][editor_selected] ^= 1 << (x - 2);
				editor_refresh();
				dirty_palette = settings.palette_selected;
			
			} else if (2 <= p && p <= 7) { // Modify green bit
				editor_palette[1][editor_selected] ^= 1 << (7 - p);
				editor_refresh();
				dirty_palette = settings.palette_selected;
			
			} else if (2 <= x && x <= 7 && y == 9) { // Modify blue bit
				editor_palette[2][editor_selected] ^= 1 << (x - 2);
				editor_refresh();
				dirty_palette = settings.palette_selected;
			
			} else if (92 <= p && p <= 98) { // Modify velocity bit
				editor_select_flip(98 - p);
			
			} else if (p == 91) {
				editor_export();

			} else if (p != 1 && p != 8 && p != 10 && p != 19 && p != 80 && p != 89) { // Select velocity on grid
				editor_select_xy(p);
			}
		}
	}
}

void editor_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {}

void editor_aftertouch_event(u8 v) {}

void editor_poly_event(u8 p, u8 v) {}