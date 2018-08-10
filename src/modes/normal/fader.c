#include "modes/normal/fader.h"
#include "modes/mode.h"
#include "app.h"
#include "led/led.h"

u8 fader_mode = 0; // 0 = Standalone, 1 = Live
u8 faders[2][8] = {{}, {}};
u8 fader_stops[2][8] = {
	{0, 17, 34, 52, 70, 89, 108, 127}, // Linear
	{0, 21, 42, 63, 64, 85, 106, 127}  // Pan
};

u8 fader_type[2][8] = {{}, {}};
u8 fader_color[2][8] = {
	{53, 53, 53, 53, 53, 53, 53, 53},
	{0, 0, 0, 0, 0, 0, 0, 0}
};

u16 fader_tick[2][8] = {{}, {}};
u16 fader_elapsed[2][8] = {{}, {}};
u8 fader_counter[2][8] = {{}, {}};
u8 fader_final[2][8] = {{}, {}};
s8 fader_change[2][8] = {{}, {}};

void fader_draw(u8 y) {
	switch (fader_type[fader_mode][y]) {
		case fader_linear:
			for (u8 x = 0; x < 8; x++) {
				if (faders[fader_mode][y] >= fader_stops[fader_linear][x] && faders[fader_mode][y]) {
					novation_led((x + 1) * 10 + y + 1, fader_color[fader_mode][y]);
				} else {
					novation_led((x + 1) * 10 + y + 1, 0);
				}
			}
			break;
			
		case fader_pan:
			if (faders[fader_mode][y] < fader_stops[fader_pan][3]) {
				for (u8 x = 7; x > 3; x--) {
					novation_led((x + 1) * 10 + y + 1, 0);
				}
				for (s8 x = 3; x >= 0; x--) {
					if (faders[fader_mode][y] <= fader_stops[fader_pan][x]) {
						novation_led((x + 1) * 10 + y + 1, fader_color[fader_mode][y]);
					} else {
						novation_led((x + 1) * 10 + y + 1, 0);
					}
				}
			} else if (faders[fader_mode][y] > fader_stops[fader_pan][4]) {
				for (u8 x = 0; x < 4; x++) {
					novation_led((x + 1) * 10 + y + 1, 0);
				}
				for (u8 x = 4; x < 8; x++) {
					if (faders[fader_mode][y] >= fader_stops[fader_pan][x]) {
						novation_led((x + 1) * 10 + y + 1, fader_color[fader_mode][y]);
					} else {
						novation_led((x + 1) * 10 + y + 1, 0);
					}
				}
			} else {
				for (u8 x = 0; x < 8; x++) {
					if (x == 3 || x == 4) {
						novation_led((x + 1) * 10 + y + 1, fader_color[fader_mode][y]);
					} else {
						novation_led((x + 1) * 10 + y + 1, 0);
					}
				}
			}
			break;
	}
}

void fader_init() {
	fader_mode = (4 - mode_default) / 3;
	
	for (u8 i = 0; i < 8; i++) fader_counter[fader_mode][i] = 0;
	for (u8 y = 0; y < 8; y++) fader_draw(y);
	
	if (mode == mode_fader) rgb_led(99, mode_fader_r, mode_fader_g, mode_fader_b); // Fader mode LED
}

void fader_timer_event() {
	for (u8 y = 0; y < 8; y++) {
		if (fader_counter[fader_mode][y]) {
			if (++fader_elapsed[fader_mode][y] >= fader_tick[fader_mode][y]) {
				faders[fader_mode][y] += fader_change[fader_mode][y]; // Update fader line
				
				fader_counter[fader_mode][y]--;
				if (!fader_counter[fader_mode][y]) {
					faders[fader_mode][y] = fader_final[fader_mode][y]; // Set fader to supposed final value
				}
				
				hal_send_midi(fader_mode, 0xB0 + 2 * (1 - fader_mode), 21 + y, faders[fader_mode][y]); // Send fader
				fader_draw(y);
				
				fader_elapsed[fader_mode][y] = 0;
			}
		}
	}
}

void fader_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(mode_setup);
	
	} else {
		if (x == 0 || x == 9 || y == 0 || y == 9) { // Unused side buttons
			hal_send_midi(fader_mode, 0xB0 + 2 * (1 - fader_mode), p, v);
			rgb_led(p, 0, (v == 0)? 0 : 63, 0);
		
		} else if (v != 0) { // Main grid
			x--; y--;
			u16 time = (14110 - (110 * v)) / 7; // Time it takes to do the line
			
			fader_final[fader_mode][y] = fader_stops[fader_type[fader_mode][y]][x]; // Save final value of the line
			
			s8 direction = 2 * (faders[fader_mode][y] < fader_final[fader_mode][y]) - 1; // Direction of line - {-1} = down, {1} = up
			u16 diff = (direction > 0)? (fader_final[fader_mode][y] - faders[fader_mode][y]) : (faders[fader_mode][y] - fader_final[fader_mode][y]); // Difference between current value and new value
			
			fader_elapsed[fader_mode][y] = 0; // Stop current line
			
			if (diff == 0) {
				hal_send_midi(fader_mode, 0xB0 + 2 * (1 - fader_mode), 21 + y, faders[fader_mode][y]); // Resend fader
							
			} else if (time >= diff) { // Enough time to do line smoothly
				fader_tick[fader_mode][y] = time / diff;
				fader_counter[fader_mode][y] = diff;
				fader_change[fader_mode][y] = direction;
				
			} else { // Not enough time - compensate with smaller steps
				fader_tick[fader_mode][y] = 1;
				fader_counter[fader_mode][y] = time;
				fader_change[fader_mode][y] = direction * (diff / time);
			}
		}
	}
}

void fader_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
	if (port == fader_mode && t == 0xB && ch == (2 * (1 - fader_mode)) && 21 <= p && p <= 28) {
		p -= 21;
		
		fader_counter[fader_mode][p] = 0;
		faders[fader_mode][p] = v;
		
		fader_draw(p);
	}
}