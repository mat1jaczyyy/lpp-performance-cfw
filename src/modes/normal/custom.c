#include "modes/normal/custom.h"

#define inactive_slot_r 5
#define inactive_slot_g 5
#define inactive_slot_b 5

#define active_slot_r 21
#define active_slot_g 63
#define active_slot_b 27

#define invalid_slot_r 63
#define invalid_slot_g 0
#define invalid_slot_b 0

u8 custom_valid = 0;
u8 custom_loaded = 0, custom_active_slot = 0;

u8 custom_upload_index = 0;
u8 custom_upload_buffer[1024] = {};
u16 custom_upload_offset = 0;

void custom_upload_start(u8 i) {
	custom_upload_index = i;
	custom_upload_offset = 0;
}

void custom_upload_push(const u8* d) {
	for (; *d != 0xF7 && custom_upload_offset < 1024; d++)
		custom_upload_buffer[custom_upload_offset++] = *d;
}

void custom_upload_end() {
	if (custom_upload_offset < 1024)
		custom_upload_buffer[custom_upload_offset++] = 0xF7;

	while (custom_upload_offset < 1024)
		custom_upload_buffer[custom_upload_offset++] = 0;
	
	custom_loaded = 0;

	flash_write_custom(custom_upload_index, custom_upload_buffer);
}

u8 custom_export = 0;
u8 custom_export_slot = 255;

#define custom_rom_start 0x0801D800
#define custom_rom_size 0x400

typedef struct {
	u8 ch, kind, p, trig, v_on, v_off, bg;
} custom_blob;

typedef struct {
	u8 xy;
	custom_blob blob;
} custom_bin_blob;

const u8* custom_data(u8 i) {
	return (const u8*)custom_rom_start + custom_rom_size * i;
}

u8 custom_external_feedback = 0;
u8 custom_on[8] = {};

u8 custom_held_slot = 255;
u8 custom_delete_counter = 0;
u16 custom_delete_blink = 0;

u8 map_screen[8][8][8];

void custom_screen_led(u8 s, u8 x, u8 y, u8 c) {
	if (c >> 7) c = map_screen[s][x][y];
	else map_screen[s][x][y] = c;

	if (s == custom_active_slot)
		novation_led((x + 1) * 10 + y + 1, c);
}

const custom_blob* map[8][8][8] = {};
u8 map_state[8][8];

u8* const map_state_i = &map_state[0][0];
u16 map_state_ptr[8][8][8];

u16 outputting;

const u8 custom_fader_stops[2][8][4] = {
	{ // Linear
		{0, 4, 8, 12},
		{17, 21, 25, 29},
		{34, 38, 42, 47},
		{52, 56, 60, 65},
		{70, 74, 79, 84},
		{89, 93, 98, 103},
		{108, 112, 117, 122},
		{127}
	},
	{ // Pan
		{0},
		{21, 16, 11, 6},
		{42, 37, 32, 27},
		{63, 58, 53, 48},
		{64, 69, 74, 79},
		{85, 90, 95, 100},
		{106, 111, 116, 121},
		{127}
	}
};

typedef struct {
	u16 tick, elapsed;
	u8 value, final, counter;
	s8 change;
} custom_fader_anim;

typedef struct {
	const custom_blob* blob;
	u8 anim_s, anim_p, type, color;
} custom_fader;

custom_fader custom_faders[8][8];
custom_fader_anim custom_fader_anims[8][8];

u8 custom_fader_orientation = 0;

#define grab_fader_slot(s, i) \
	custom_fader* fader = &custom_faders[s][i]; \
	custom_fader_anim* anim = &custom_fader_anims[fader->anim_s][fader->anim_p];

#define grab_fader(i) grab_fader_slot(custom_active_slot, i)

void custom_fader_led(u8 x, u8 y, u8 v, u8 f) {
	u8 orientation = (custom_fader_orientation >> custom_active_slot) & 1;

	rgb_led(
		((orientation? y : x) + 1) * 10 + (orientation? x : y) + 1,
		palette_value(palette_novation, v, 0) >> f,
		palette_value(palette_novation, v, 1) >> f,
		palette_value(palette_novation, v, 2) >> f
	);
}

void custom_fader_draw(u8 i) {
	grab_fader(i)
	if (!fader->blob) return;

	if (fader->type) {   // Pan
		if (anim->value < custom_fader_stops[fader->type][3][0] + (anim->value != anim->final))
			for (s8 x = 7; x >= 0; x--) {
				u8 f = 0;

				if (x >= 4) f = 7;
				else if (x == 0) f = anim->value == custom_fader_stops[fader->type][0][0]? 0 : 7;
				else if (anim->value <= custom_fader_stops[fader->type][x - 1][0]) f = 0;
				else if (anim->value > custom_fader_stops[fader->type][x][0]) f = 7;
				else for (u8 j = 0; j < 3; j++)
					if (anim->value > custom_fader_stops[fader->type][x][j + 1]) {
						f = 3 - j;
						break;
					}
				
				custom_fader_led(x, i, fader->color, f);
			}

		else if (anim->value > custom_fader_stops[fader->type][4][0] - (anim->value != anim->final))
			for (u8 x = 0; x < 8; x++) {
				u8 f = 0;

				if (x <= 3) f = 7;
				else if (x == 7) f = anim->value == custom_fader_stops[fader->type][7][0]? 0 : 7;
				else if (anim->value >= custom_fader_stops[fader->type][x + 1][0]) f = 0;
				else if (anim->value < custom_fader_stops[fader->type][x][0]) f = 7;
				else for (u8 j = 0; j < 3; j++)
					if (anim->value < custom_fader_stops[fader->type][x][j + 1]) {
						f = 3 - j;
						break;
					}
				
				custom_fader_led(x, i, fader->color, f);
			}

		else
			for (u8 x = 0; x < 8; x++)
				custom_fader_led(x, i, x == 3 || x == 4? fader->color : 0, 0);

	} else   // Linear
		for (u8 x = 0; x < 8; x++) {
			u8 f = 0;

			if (x == 7) f = anim->value == custom_fader_stops[fader->type][7][0]? 0 : 7;
			else if (anim->value >= custom_fader_stops[fader->type][x + 1][0]) f = 0;
			else if (anim->value < custom_fader_stops[fader->type][x][0]) f = 7;
			else for (u8 j = 0; j < 3; j++)
				if (anim->value < custom_fader_stops[fader->type][x][j + 1]) {
					f = 3 - j;
					break;
				}

			custom_fader_led(x, i, fader->color, f);
		}
}

const u16 custom_fader_times_a[12] = {
	8062, 6450, 5374, 4607, 4031, 3583, 3224, 2932, 2688, 2481, 2303, 2150
};

const u16 custom_fader_times_b[16] = {
	2016, 1896, 1792, 1696, 1613, 1536, 1467, 1403, 1344, 1291, 1240, 1192, 1152, 1112, 1076, 1040
};

u32 custom_fader_time(u8 v) {
	if (v < 48) return custom_fader_times_a[v >> 2];
	if (v < 112) return custom_fader_times_b[v & 0x0F] >> ((v >> 4) - 3);

	return 126;
}

inline void custom_fader_send(custom_fader* fader) {
	u8 ch = fader->blob->ch <= 0xF? fader->blob->ch : 0x0;
	send_midi(USBSTANDALONE, 0xB0 | ch, fader->blob->p, custom_fader_anims[fader->anim_s][fader->anim_p].value);
}

void custom_fader_trigger(u8 x, u8 y, u8 v) {
	if (!v) return;

	u8 orientation = (custom_fader_orientation >> custom_active_slot) & 1;
	u8 i = orientation? x : y;
	u8 c = orientation? y : x;

	grab_fader(i)
	if (!fader->blob) return;

	v = custom_fader_vel_sensitive? v : 127;
	
	anim->final = custom_fader_stops[fader->type][c][0]; // Save final value of the line

	if ((fader->type == 0 && c != 7) || (fader->type == 1 && c != 0 && c != 7)) // Retrigger small steps
		for (u8 j = 0; j < 3; j++)
			if (custom_fader_stops[fader->type][c][j] == anim->value) {
				anim->final = custom_fader_stops[fader->type][c][j + 1];
				break;
			}
	
	s8 direction = 2 * (anim->value < anim->final) - 1; // Direction of line - {-1} = down, {1} = up
	u16 diff = (direction > 0)? (anim->final - anim->value) : (anim->value - anim->final); // Difference between current value and new value
	
	anim->elapsed = anim->counter = 0; // Stop current line

	if (diff == 1)
		anim->value = anim->final;
	
	if (diff <= 1) {
		custom_fader_send(fader);

		for (u8 i = 0; i < 8; i++)
			custom_fader_draw(i);

		return;
	}

	u16 time = custom_fader_time(v) * (diff - 1) / 126; // Time it takes to do the line

	if (time >= diff) { // Enough time to do line smoothly
		anim->tick = time / diff;
		anim->counter = diff;
		anim->change = direction;
		
	} else { // Not enough time - compensate with smaller steps
		anim->tick = 1;
		anim->counter = time;
		anim->change = direction * (diff / time);
	}
}

void custom_load() {
	custom_loaded = 1;
	custom_valid = 0;

	custom_external_feedback = 0;
	memset(custom_on, 0, sizeof(custom_on));
	memset(map_screen, 0, sizeof(map_screen));
	memset(map, 0, sizeof(map));
	memset(map_state, 0, sizeof(map_state));
	memset(map_state_ptr, 0, sizeof(map_state_ptr));
	outputting = 0;
	memset(custom_faders, 0, sizeof(custom_faders));
	memset(custom_fader_anims, 0, sizeof(custom_fader_anims));
	custom_fader_orientation = 0;

	for (u8 s = 0; s < 8; s++) {
		const u8* data = custom_data(s);
		const void* top = custom_data(s + 1);

		while (*data != 0x7F)  // Skip web-editor region (name and object metadata)
			if (*data > 0x7F || (void*)data++ >= top) goto next;

		custom_external_feedback |= (data[2] & 1) << s;
		custom_on[s] = data[3];

		for (const custom_bin_blob* blob = (const custom_bin_blob*)(data + 4); blob->xy != 0xF7; blob++) {
			if (blob->xy > 0x7F || (void*)blob >= top) goto next;

			if (blob->blob.kind) {   // Regular pad
				u8 x = blob->xy / 10 - 1;
				u8 y = blob->xy % 10 - 1;
				map[s][x][y] = &blob->blob;

				u8 map_ch = map[s][x][y]->ch <= 0xF? map[s][x][y]->ch : 0x0;
				map_state_ptr[s][x][y] = s * 64 + x * 8 + y;

				for (u8 t = 0; t <= s; t++) {
					for (u8 x2 = 0; x2 < 8; x2++) {
						for (u8 y2 = 0; y2 < 8; y2++) {
							if (!map[t][x2][y2]) continue;

							u8 t_ch = map[t][x2][y2]->ch <= 0xF? map[t][x2][y2]->ch : 0x0;

							if (map[s][x][y]->kind == map[t][x2][y2]->kind && map_ch == t_ch && map[s][x][y]->p == map[t][x2][y2]->p) {
								map_state_ptr[s][x][y] = t * 64 + x2 * 8 + y2;
								goto done_map;
							}
						}
					}
				}
				done_map:;
			
			} else if (blob->xy < 8) {   // Fader
				u8 p = blob->xy;
				
				u8 orientation = (blob->blob.trig >> 1) & 1;
				custom_fader_orientation |= orientation << s;

				if (orientation)
					p = 7 - p;

				for (u8 i = 0; i < 8; i++) {
					if (blob->blob.trig < 2)
						map[s][i][p] = &blob->blob;

					else map[s][p][i] = &blob->blob;
				}

				custom_faders[s][p].anim_s = s;
				custom_faders[s][p].anim_p = p;

				for (u8 t = 0; t <= s; t++) {
					for (u8 i = 0; i < 8; i++) {
						if (custom_faders[t][i].blob && custom_faders[t][i].blob->ch == blob->blob.ch && custom_faders[t][i].blob->p == blob->blob.p) {
							custom_faders[s][p].anim_s = custom_faders[t][i].anim_s;
							custom_faders[s][p].anim_p = custom_faders[t][i].anim_p;

							goto done_fader;
						}
					}
				}
				
				done_fader:
				custom_faders[s][p].blob = &blob->blob;
				custom_faders[s][p].type = blob->blob.trig & 1;
				custom_faders[s][p].color = blob->blob.bg;

				custom_fader_anim* anim = &custom_fader_anims[custom_faders[s][p].anim_s][custom_faders[s][p].anim_p];
				anim->value = anim->final = 127;
			}
		}

		custom_valid |= 1 << s;

		next:;
	}
}

void custom_init() {
	rgb_led(99, mode_custom_r, mode_custom_g, mode_custom_b); // Custom mode LED

	active_port = USBSTANDALONE;

	custom_held_slot = 255;
	custom_delete_counter = 0;
	custom_delete_blink = 0;

	u8 valid = (custom_valid >> custom_active_slot) & 1;

	if (!custom_loaded) {
		custom_load();
		valid = (custom_valid >> custom_active_slot) & 1;

		for (u8 s = 0; s < 8; s++)
			if ((custom_valid >> s) & 1 && !((custom_external_feedback >> s) & 1))
				for (u8 x = 0; x < 8; x++)
					for (u8 y = 0; y < 8; y++)
						if (map[s][x][y] && map[s][x][y]->kind)
							custom_screen_led(s, x, y, map[s][x][y]->bg & 0x7F);
	
	} else if (valid) {
		for (u8 x = 0; x < 8; x++)
			for (u8 y = 0; y < 8; y++)
				custom_screen_led(custom_active_slot, x, y, 255);
	}

	if (valid)
		for (u8 i = 0; i < 8; i++)
			custom_fader_draw(i);
	
	for (u8 i = 0; i < 8; i++)
		if (custom_active_slot == i) {
			if (valid) rgb_led(89 - i * 10, active_slot_r, active_slot_g, active_slot_b);
			else rgb_led(89 - i * 10, invalid_slot_r, invalid_slot_g, invalid_slot_b);

		} else rgb_led(89 - i * 10, inactive_slot_r, inactive_slot_g, inactive_slot_b);
}

void custom_timer_event() {
	if (!custom_loaded) return;

	if (custom_export_slot < 8 && custom_export++ % 7 == 0) {
		u8 p = custom_export / 7;
		
		const u8* c = custom_data(custom_export_slot);

		u16 l = 0;
		for (; *c != 0xF7; c++)
			l++;

		c -= l;

		syx_resp_cpy(custom_header);
		u16 i = sizeof(syx_custom_header) + 1;

		if (p == 0) {
			syx_response_buffer[i++] = l >> 7;
			syx_response_buffer[i++] = l & 0x7F;
		}

		for (u16 j = p == 0? 0 : 310 * p - 2; i < sizeof(syx_response_buffer) - 1 && c[j] != 0xF7; j++)
			syx_response_buffer[i++] = c[j];
		
		syx_response_buffer[i++] = 0xF7;
		syx_send(USBSTANDALONE, i);

		if (310 * (p + 1) - 2 >= l) custom_export_slot = 255;
	}

	u8 anim_processed[8] = {};

	for (u8 s = 0; s < 8; s++) {
		if (!((custom_valid >> s) & 1)) continue;

		for (u8 i = 0; i < 8; i++) {
			grab_fader_slot(s, i)

			if (!fader->blob) continue;

			if (!((anim_processed[fader->anim_s] << fader->anim_p) & 1)) {
				if (anim->counter && ++anim->elapsed >= anim->tick) {
					anim->value += anim->change; // Update fader line
					
					anim->counter--;
					if (!anim->counter)
						anim->value = anim->final; // Set fader to supposed final value
					
					custom_fader_send(fader);
					anim->elapsed = 0;
				}

				anim_processed[fader->anim_s] |= 1 << fader->anim_p;
			}
			
			if (mode == mode_custom && custom_active_slot == s)
				custom_fader_draw(i);
		}
	}

	if (mode != mode_custom || !((custom_valid >> custom_active_slot) & 1)) return;

	if (custom_held_slot < 8 && custom_delete_blink) {
		if (++custom_delete_blink > 400) custom_delete_blink = 1;
		
		rgb_led(
			89 - custom_held_slot * 10,
			custom_delete_blink > 200? 0 : invalid_slot_r,
			custom_delete_blink > 200? 0 : invalid_slot_g,
			custom_delete_blink > 200? 0 : invalid_slot_b
		);
	}
}

void custom_led(u8 ch, u8 s, u8 x, u8 y, u8 v, u8 c) {
	u8 p = (x + 1) * 10 + y + 1;

	if ((custom_external_feedback >> custom_active_slot) & 1) {
		if (s == custom_active_slot) {
			if (ch == 0x0) novation_led(p, v);
			else if (ch == 0x1) flash_led(p, v);
			else if (ch == 0x2) pulse_led(p, v);
		}
		return;
	}

	custom_screen_led(s, x, y, c & 0x7F);
}

void custom_highlight(u8 t, u8 ch, u8 p, u8 v, u8 s) {
	if (s && !custom_midi_led) return;

	u8 ext = (custom_external_feedback >> custom_active_slot) & 1;
	if (!s && ext) return;

	u8 k;
	if (t == 0x9) k = 0x01;
	else if (t == 0xB && (!s || ext)) k = 0x02;
	else return;

	u8 e = s? 1 : custom_surface_led;
	
	for (u8 slot = 0; slot < 8; slot++) {
		for (u8 x = 0; x < 8; x++) {
			for (u8 y = 0; y < 8; y++) {
				u8 map_ch = ext? ch : (map[slot][x][y]->ch <= 0xF? map[slot][x][y]->ch : 0x0);
				
				if (map[slot][x][y]->kind == k && map_ch == ch && map[slot][x][y]->p == p) {
					if (k == 0x01) {
						if (map[slot][x][y]->trig == 0x01) {
							if (!s || ext)
								custom_led(ch, slot, x, y, v, v == 127? custom_on[slot] : map[slot][x][y]->bg);

						} else if (e) custom_led(ch, slot, x, y, v, v? custom_on[slot] : map[slot][x][y]->bg);

					} else if (k == 0x02 && (map[slot][x][y]->trig == 0x01 || (map[slot][x][y]->trig == 0x00 && e)))
						custom_led(ch, slot, x, y, v, v == map[slot][x][y]->v_on? custom_on[slot] : map[slot][x][y]->bg);
				}
			}
		}
	}
}

void custom_send(u8 t, u8 ch, u8 p, u8 v) {
	if (!((custom_valid >> custom_active_slot) & 1))return;

	send_midi(USBSTANDALONE, (t << 4) | (ch & 0xF), p, v);
	custom_highlight(t, ch, p, v, 0);
}

void custom_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(mode_setup);
	
	} else if (y == 9 && 1 <= x && x <= 8) {
		u8 t = 8 - x;

		if (v) {
			if (custom_active_slot != t) {
				custom_active_slot = t;
				mode_refresh();
			}

			custom_held_slot = t;
		
		} else if (custom_held_slot == t) {
			if ((custom_valid >> custom_active_slot) & 1) rgb_led(89 - custom_held_slot * 10, active_slot_r, active_slot_g, active_slot_b);

			custom_held_slot = 255;
			custom_delete_counter = 0;
			custom_delete_blink = 0;
		}
	
	} else {
		if (x == 0 || x == 9 || y == 0) { // Unused side buttons
			send_midi(USBSTANDALONE, 0xB0, p, v);  // TODO: Define channel somehow?
			rgb_led(p, 0, (v == 0)? 0 : 63, 0);
		}

		if ((custom_valid >> custom_active_slot) & 1) {
			if (p == 91) { // Export mode
				if (v) {
					custom_export = 0;
					custom_export_slot = custom_active_slot;
				}

			} else if (p == 50) { // Delete mode
				if (v) {
					if (custom_held_slot < 8) {
						rgb_led(50, invalid_slot_r, invalid_slot_g, invalid_slot_b);

						if (++custom_delete_counter == 1)
							rgb_led(
								89 - custom_held_slot * 10,
								(active_slot_r + invalid_slot_r) / 2,
								(active_slot_g + invalid_slot_g) / 2,
								(active_slot_b + invalid_slot_b) / 2
							);
						
						else if (custom_delete_counter == 2)
							custom_delete_blink = 1;

						if (custom_delete_counter >= 3) {
							for (u16 i = 0; i < 1024; i++)
								custom_upload_buffer[i] = 0xFF;

							custom_loaded = 0;
							flash_write_custom(custom_held_slot, custom_upload_buffer);

							mode_refresh();
						}
					}

				} else rgb_led(50, 0, 0, 0);

			} else if (1 <= x && x <= 8 && 1 <= y && y <= 8) {
				x--;
				y--;

				if (map[custom_active_slot][x][y]) {
					if (!map[custom_active_slot][x][y]->kind) {
						custom_fader_trigger(x, y, v);
						return;
					}

					u8 ch = map[custom_active_slot][x][y]->ch <= 0xF? map[custom_active_slot][x][y]->ch : 0x0;
					
					switch (map[custom_active_slot][x][y]->kind) {
						case 0x01: // MIDI note
							if (map[custom_active_slot][x][y]->trig == 0x01) {
								if (v) {
									u8 x2 = map_state_ptr[custom_active_slot][x][y] >> 3;
									u8 y2 = map_state_ptr[custom_active_slot][x][y] & 0x7;

									map_state_i[x2] ^= 1ULL << y2;
									custom_send(0x9, ch, map[custom_active_slot][x][y]->p, ((map_state_i[x2] >> y2) & 1)? v : 0);
								}
								
							} else {
								outputting ^= (((outputting >> ch) ^ (v > 0)) & 1) << ch; // set bit at ch to be v > 0
								custom_send(0x9, ch, map[custom_active_slot][x][y]->p, v);
							}
							break;
						
						case 0x02: // Control Change
							if (map[custom_active_slot][x][y]->trig == 0x01) {
								if (v) {
									u8 x2 = map_state_ptr[custom_active_slot][x][y] >> 3;
									u8 y2 = map_state_ptr[custom_active_slot][x][y] & 0x7;

									map_state_i[x2] ^= 1ULL << y;
									custom_send(0xB, ch, map[custom_active_slot][x][y]->p, ((map_state_i[x2] >> y2) & 1)? map[custom_active_slot][x][y]->v_on : map[custom_active_slot][x][y]->v_off);
								}

							} else if (map[custom_active_slot][x][y]->trig == 0x02) {
								if (v) custom_send(0xB, ch, map[custom_active_slot][x][y]->p, map[custom_active_slot][x][y]->v_on);
								
							} else custom_send(0xB, ch, map[custom_active_slot][x][y]->p, v? map[custom_active_slot][x][y]->v_on : map[custom_active_slot][x][y]->v_off);
							break;
						
						case 0x03: // Program Change
							if (v) custom_send(0xC, ch, map[custom_active_slot][x][y]->p, 0);
							break;
					}
				}
			}
		}
	}
}

void custom_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
	if ((custom_valid >> custom_active_slot) & 1 && port == USBSTANDALONE) {
		if (t == 0x8) {
			v = 0; // Note off
			t = 0x9;
		}
		
		custom_highlight(t, ch, p, v, 1);
	}
}

void custom_aftertouch_event(u8 v) {
	if (!((custom_valid >> custom_active_slot) & 1)) return;

	for (u8 i = 0; i < 16; i++)
		if ((outputting << i) & 1)
			aftertouch_send(USBSTANDALONE, 0xD0 | i, v);
}

void custom_poly_event(u8 p, u8 v) {
	if (!((custom_valid >> custom_active_slot) & 1)) return;

	u8 x = p / 10 - 1;
	u8 y = p % 10 - 1;

	if (0 <= x && x <= 7 && 0 <= y && y <= 7 && map[custom_active_slot][x][y] && map[custom_active_slot][x][y]->kind == 0x01 && map[custom_active_slot][x][y]->trig != 0x01)
		custom_send(0xA, map[custom_active_slot][x][y]->ch, map[custom_active_slot][x][y]->p, v);
}