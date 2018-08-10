#include "modes/special/setup.h"
#include "modes/mode.h"
#include "led/led.h"
#include "led/palettes.h"
#include "flash/flash.h"
#include "flash/settings.h"

u8 setup_elapsed = setup_tick;
u8 setup_rainbow[setup_rainbow_length][3] = {{63, 0, 0}, {63, 7, 0}, {63, 15, 0}, {63, 23, 0}, {63, 31, 0}, {63, 39, 0}, {63, 47, 0}, {63, 55, 0}, {63, 63, 0}, {55, 63, 0}, {47, 63, 0}, {39, 63, 0}, {31, 63, 0}, {23, 63, 0}, {15, 63, 0}, {7, 63, 0}, {0, 63, 0}, {0, 63, 7}, {0, 63, 15}, {0, 63, 23}, {0, 63, 31}, {0, 63, 39}, {0, 63, 47}, {0, 63, 55}, {0, 63, 63}, {0, 55, 63}, {0, 47, 63}, {0, 39, 63}, {0, 31, 63}, {0, 23, 63}, {0, 15, 63}, {0, 7, 63}, {0, 0, 63}, {7, 0, 63}, {15, 0, 63}, {23, 0, 63}, {31, 0, 63}, {39, 0, 63}, {47, 0, 63}, {55, 0, 63}, {63, 0, 63}, {63, 0, 55}, {63, 0, 47}, {63, 0, 39}, {63, 0, 31}, {63, 0, 23}, {63, 0, 15}, {63, 0, 7}};
u8 setup_mode_counter = 0;
u8 setup_editor_counter = 0;
u8 setup_jump = 0;

void setup_init() {
	if (mode_default == mode_performance) {
		for (u8 i = 16; i < 19; i++) {
			rgb_led(i, setup_custom_r >> 2, setup_custom_g >> 2, setup_custom_b >> 2); // Select preset palette
		}

		for (u8 i = 26; i < 29; i++) {
			rgb_led(i, setup_preset_r >> 2, setup_preset_g >> 2, setup_preset_b >> 2); // Select flash (custom) palette
		}

		if (palette_selected < palette_custom) {
			rgb_led(palette_selected + 26, setup_custom_r, setup_custom_g, setup_custom_b); // Flash palette selected
		} else {
			rgb_led(palette_selected + 16 - palette_custom, setup_preset_r, setup_preset_g, setup_preset_b); // Preset palette selected
		}

		rgb_led(85, setup_top_pro_r >> 2, setup_top_pro_g >> 2, setup_top_pro_b >> 2); // PRO Top Lights
		for (u8 i = 86; i < 89; i++) {
			rgb_led(i, setup_top_mk2_r >> 2, setup_top_mk2_g >> 2, setup_top_mk2_b >> 2); // MK2 Top Lights
		}
	
		if (!top_lights_config) {
			rgb_led(85, setup_top_pro_r, setup_top_pro_g, setup_top_pro_b); // PRO Top Lights selected
		} else {
			rgb_led(top_lights_config + 85, setup_top_mk2_r, setup_top_mk2_g, setup_top_mk2_b); // MK2 Top Lights selected
		}
	}

	if (vel_sensitive) {
		rgb_led(11, setup_velocity_r, setup_velocity_g, setup_velocity_b); // Velocity sensitivity enabled
	} else {
		rgb_led(11, setup_velocity_r >> 2, setup_velocity_g >> 2, setup_velocity_b >> 2); // Velocity sensitivity disabled
	}

	rgb_led(81, mode_performance_r >> 2, mode_performance_g >> 2, mode_performance_b >> 2); // Performance mode
	rgb_led(82, mode_ableton_r >> 2, mode_ableton_g >> 2, mode_ableton_b >> 2); // Ableton mode
	rgb_led(83, mode_note_r >> 2, mode_note_g >> 2, mode_note_b >> 2); // Note mode
	rgb_led(71, mode_drum_r >> 2, mode_drum_g >> 2, mode_drum_b >> 2); // Drum mode
	rgb_led(72, mode_fader_r >> 2, mode_fader_g >> 2, mode_fader_b >> 2); // Fader mode
	rgb_led(73, mode_programmer_r >> 2, mode_programmer_g >> 2, mode_programmer_b >> 2); // Programmer mode
	
	switch (mode_default) {
		case 0:
			rgb_led(81, mode_performance_r, mode_performance_g, mode_performance_b); // Performance mode selected
			break;
		
		case 1:
			rgb_led(82, mode_ableton_r, mode_ableton_g, mode_ableton_b); // Ableton mode selected
			break;
		
		case 2:
			rgb_led(83, mode_note_r, mode_note_g, mode_note_b); // Note mode selected
			break;
		
		case 3:
			rgb_led(71, mode_drum_r, mode_drum_g, mode_drum_b); // Drum mode selected
			break;
		
		case 4:
			rgb_led(72, mode_fader_r, mode_fader_g, mode_fader_b); // Fader mode selected
			break;
		
		case 5:
			rgb_led(73, mode_programmer_r, mode_programmer_g, mode_programmer_b); // Programmer mode selected
			break;
	}
	
	setup_elapsed = setup_tick;
}

void setup_timer_event() {
	if (++setup_elapsed >= setup_tick) {
		rgb_led(99, setup_rainbow[setup_mode_counter][0], setup_rainbow[setup_mode_counter][1], setup_rainbow[setup_mode_counter][2]); // Mode LED indicator animation
		setup_mode_counter++; setup_mode_counter %= setup_rainbow_length;
		
		if (mode_default == mode_performance && palette_selected < palette_custom) {
			rgb_led(25, setup_rainbow[setup_editor_counter][0], setup_rainbow[setup_editor_counter][1], setup_rainbow[setup_editor_counter][2]);  // Enter palette editor button animation
			setup_editor_counter++; setup_editor_counter %= setup_rainbow_length;
		}
		
		setup_elapsed = 0;
	}
}

void setup_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (v) {
		setup_jump = 1;
		
		if (p == 0) { // Enter selected main mode
			mode_update(mode_default);
			setup_jump = 0;
		
		} else if (p == 11) { // Toggle velocity sensitivity
			vel_sensitive = (vel_sensitive)? 0 : 1;
			dirty = 1;
			mode_refresh();

		} else if (81 <= p && p <= 83) { // Switch default mode
			mode_default = p - 81;
			mode_refresh();
		
		} else if (71 <= p && p <= 73) {
			mode_default = p - 68;
			mode_refresh();
		
		} else if (mode_default == mode_performance) {
			if (1 <= x && x <= 2 && 6 <= y && y <= 8) { // Palette switch
				palette_selected = (2 - x) * 3 + y - 6;
				dirty = 1;
				mode_refresh();
		
			} else if (p == 25) {
				if (palette_selected < 3) { // Enter Palette editor mode
					mode_update(mode_editor);
					setup_jump = 0;
				}
		
			} else if (85 <= p && p <= 88) { // Change Top Lights configuration
				top_lights_config = p - 85;
				dirty = 1;
				mode_refresh();
			}
		}
	
	} else { // Note released
		if (p == 0 && setup_jump) { // Quickly jump back to main mode
			mode_update(mode_default);
			setup_jump = 0;
		}
	}
}

void setup_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {}