#include "modes/normal/custom.h"

#define inactive_slot_r 5
#define inactive_slot_g 5
#define inactive_slot_b 5

#define active_slot_r 21
#define active_slot_g 63
#define active_slot_b 27

#define custom_rom_start 0x0801D800
#define custom_rom_size 0x400

typedef struct {
	u8 ch, kind, p, trig, v_on, v_off, bg;
} custom_blob;

typedef struct {
	u8 xy;
	custom_blob blob;
} custom_bin_blob;

typedef struct {
	u8 state;
	const custom_blob* blob;
} custom_map_blob;

const u8* custom_data(u8 i) {
	return (const u8*)custom_rom_start + custom_rom_size * i;
}

u8 custom_prev_active_slot = 255, custom_active_slot = 0;
u8 custom_on = 21;

custom_map_blob map[8][8] = {};
s8 outputting[16] = {};

const u8 custom_fader_stops[2][8] = {
	{0, 17, 34, 52, 70, 89, 108, 127}, // Linear
	{0, 21, 42, 63, 64, 85, 106, 127}  // Pan
};

struct {
	u8 value, type, color;
	u16 tick, elapsed;
	u8 counter, final;
	s8 change;
} custom_faders[8];

u8 custom_fader_orientation = 0;

void custom_fader_led(u8 x, u8 y, u8 v) {
	novation_led(((custom_fader_orientation? y : x) + 1) * 10 + (custom_fader_orientation? x : y) + 1, v);
}

void custom_fader_draw(u8 i) {
	if (!custom_faders[i].color) return;

	const u8* stops = custom_fader_stops[custom_faders[i].type];

	if (custom_faders[i].type) {   // Pan
		if (custom_faders[i].value < stops[3])
			for (s8 x = 7; x >= 0; x--)
				custom_fader_led(x, i, x <= 3 && custom_faders[i].value <= stops[x]? custom_faders[i].color : 0);

		else if (custom_faders[i].value > stops[4])
			for (u8 x = 0; x < 8; x++)
				custom_fader_led(x, i, 4 <= x && custom_faders[i].value >= stops[x]? custom_faders[i].color : 0);

		else
			for (u8 x = 0; x < 8; x++)
				custom_fader_led(x, i, x == 3 || x == 4? custom_faders[i].color : 0);

	} else   // Linear
		for (u8 x = 0; x < 8; x++)
			custom_fader_led(x, i, custom_faders[i].value >= stops[x] && custom_faders[i].value? custom_faders[i].color : 0);
}

void custom_init() {
	rgb_led(99, mode_custom_r, mode_custom_g, mode_custom_b); // Custom mode LED

    for (u8 i = 0; i < 8; i++) {
		if (custom_active_slot == i)
       		rgb_led(89 - i * 10, active_slot_r, active_slot_g, active_slot_b);
		else rgb_led(89 - i * 10, inactive_slot_r, inactive_slot_g, inactive_slot_b);
	}

	active_port = USBSTANDALONE;

	if (custom_prev_active_slot != custom_active_slot) {
		custom_prev_active_slot = custom_active_slot;

		const u8* data = custom_data(custom_active_slot);
		while (*data != 0x7F) data++;  // Skip web-editor region (name and object metadata)
		custom_on = data[3];

		memset(map, 0, sizeof(map));
		memset(outputting, 0, sizeof(outputting));
		memset(custom_faders, 0, sizeof(custom_faders));

		for (const custom_bin_blob* blob = (const custom_bin_blob*)(data + 4); blob->xy != 0xF7; blob++) {
			if (blob->blob.kind) {   // Regular pad
				u8 x = blob->xy / 10 - 1;
				u8 y = blob->xy % 10 - 1;
				map[x][y].blob = &blob->blob;
			
			} else if (blob->xy < 8) {   // Fader
				u8 p = blob->xy;
				
				for (u8 i = 0; i < 7; i++) {
					if (blob->blob.trig < 2)
						map[i][p].blob = &blob->blob;

					else map[p][i].blob = &blob->blob;
				}

				if ((custom_fader_orientation = blob->blob.trig >> 1))
					p = 7 - p;

				custom_faders[p].type = blob->blob.trig & 1;
				custom_faders[p].color = blob->blob.bg;
				custom_faders[p].value = 127;
			}
		}
	}

	for (u8 x = 0; x < 8; x++)
		for (u8 y = 0; y < 8; y++)
			if (map[x][y].blob && map[x][y].blob->kind)
				novation_led((x + 1) * 10 + y + 1, map[x][y].blob->bg);

	for (u8 i = 0; i < 8; i++)
		custom_fader_draw(i);
}

void custom_timer_event() {}

void custom_highlight(u8 t, u8 ch, u8 p, u8 v, u8 s) {
	if (s && !custom_midi_led) return;

	u8 k;
	if (t == 0x9) k = 0x01;
	else if (t == 0xB && !s) k = 0x02;
	else return;

	u8 e = s? 1 : custom_surface_led;
	
	for (u8 x = 0; x < 8; x++) {
		for (u8 y = 0; y < 8; y++) {
			u8 map_ch = map[x][y].blob->ch <= 0xF? map[x][y].blob->ch : 0x0;
			
			if (map[x][y].blob->kind == k && map_ch == ch && map[x][y].blob->p == p) {
				u8 p = (x + 1) * 10 + y + 1;

				if (k == 1) {
					if (map[x][y].blob->trig == 0x01) {
						if (!s) novation_led(p, v == 127? custom_on : map[x][y].blob->bg);

					} else if (e) novation_led(p, v? custom_on : map[x][y].blob->bg);

				} else if (k == 0x02 && (map[x][y].blob->trig == 0x01 || (map[x][y].blob->trig == 0x00 && e)))
					novation_led(p, v == map[x][y].blob->v_on? custom_on : map[x][y].blob->bg);
			}
		}
	}
}

void custom_send(u8 t, u8 ch, u8 p, u8 v) {
	send_midi(USBSTANDALONE, (t << 4) | (ch & 0xF), p, v);
	custom_highlight(t, ch, p, v, 0);
}

void custom_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(mode_setup);
	
	} else {
		if (y == 9 && 1 <= x && x <= 8) {
            if (v != 0) {
                custom_active_slot = 8 - x;
                mode_refresh();
            }
        
		} else if (1 <= x && x <= 8 && 1 <= y && y <= 8) {
			x--;
			y--;

			if (map[x][y].blob) {
				u8 ch = map[x][y].blob->ch <= 0xF? map[x][y].blob->ch : 0x0;
				
				switch (map[x][y].blob->kind) {
					case 0x01: // MIDI note
						if (map[x][y].blob->trig == 0x01) {
							if (v) custom_send(0x9, ch, map[x][y].blob->p, (map[x][y].state = !map[x][y].state)? v : 0);
							
						} else {
							outputting[ch] = v? 1 : -1;
							if (outputting[ch] < 0) outputting[ch] = 0;

							custom_send(0x9, ch, map[x][y].blob->p, v);
						}
						break;
					
					case 0x02: // Control Change
						if (map[x][y].blob->trig == 0x01) {
							if (v) custom_send(0xB, ch, map[x][y].blob->p, (map[x][y].state = !map[x][y].state)? map[x][y].blob->v_on : map[x][y].blob->v_off);

						} else if (map[x][y].blob->trig == 0x02) {
							if (v) custom_send(0xB, ch, map[x][y].blob->p, map[x][y].blob->v_on);
							
						} else custom_send(0xB, ch, map[x][y].blob->p, v? map[x][y].blob->v_on : map[x][y].blob->v_off);
						break;
					
					case 0x03: // Program Change
						if (v) custom_send(0xC, ch, map[x][y].blob->p, 0);
						break;
				}
			}
		}
	}
}

void custom_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
	if (port == USBSTANDALONE) {
		if (t == 0x8) {
			v = 0; // Note off
			t = 0x9;
		}
		
		custom_highlight(t, ch, p, v, 1);
	}
}

void custom_aftertouch_event(u8 v) {
	for (u8 i = 0; i < 16; i++)
		if (outputting[i])
			aftertouch_send(USBSTANDALONE, 0xD0 | i, v);
}

void custom_poly_event(u8 p, u8 v) {
	u8 x = p / 10 - 1;
	u8 y = p % 10 - 1;

	if (0 <= x && x <= 7 && 0 <= y && y <= 7 && map[x][y].blob && map[x][y].blob->kind == 0x01 && map[x][y].blob->trig != 0x01)
		custom_send(0xA, map[x][y].blob->ch, map[x][y].blob->p, v);
}