#include "modes/special/setup.h"

#define setup_custom_r 47
#define setup_custom_g 63
#define setup_custom_b 63

#define setup_preset_r 47
#define setup_preset_g 63
#define setup_preset_b 47

#define setup_velocity_r 31
#define setup_velocity_g 63
#define setup_velocity_b 63

#define setup_top_pro_r 63
#define setup_top_pro_g 47
#define setup_top_pro_b 63

#define setup_top_mk2_r 47
#define setup_top_mk2_g 31
#define setup_top_mk2_b 63

#define setup_performance_xy_r 63
#define setup_performance_xy_g 31
#define setup_performance_xy_b 31

#define setup_aftertouch_r 31
#define setup_aftertouch_g 63
#define setup_aftertouch_b 31

#define setup_direct_r 63
#define setup_direct_g 47
#define setup_direct_b 31

#define setup_idle_r 31
#define setup_idle_g 31
#define setup_idle_b 63

#define setup_brightness_selected_r 63
#define setup_brightness_selected_g 63
#define setup_brightness_selected_b 63

#define setup_brightness_r 7
#define setup_brightness_g 7
#define setup_brightness_b 23

#define setup_channel_deselected_r 0
#define setup_channel_deselected_g 21
#define setup_channel_deselected_b 21

#define setup_channel_selected_r 0
#define setup_channel_selected_g 0
#define setup_channel_selected_b 63

#define setup_custom_on_r 0
#define setup_custom_on_g 63
#define setup_custom_on_b 37

#define setup_custom_off_r 10
#define setup_custom_off_g 0
#define setup_custom_off_b 0

#define konami_r 63
#define konami_g 0
#define konami_b 0

#define setup_tick 33
#define setup_rainbow_length 48
#define konami_length 8

u8 setup_rainbow(u8 i, u8 c) {
	if (c == 0) {
		if (i < 9) return 63;
		if (i < 16) return 127 - 8 * i;
		if (i < 33) return 0;
		if (i < 40) return 8 * i - 257;
		return 63;

	} else if (c == 1) {
		if (i == 0) return 0;
		if (i < 8) return 8 * i - 1;
		if (i < 25) return 63;
		if (i < 32) return 255 - 8 * i;
		return 0;

	} else if (c == 2) {
		if (i < 17) return 0;
		if (i < 24) return 8 * i - 129;
		if (i < 41) return 63;
		if (i < 48) return 383 - 8 * i;
		return 0;
	}

	return 0;
}

const u8 konami[konami_length] = {91, 91, 92, 92, 93, 94, 93, 94};

u8 setup_elapsed = setup_tick;
u8 setup_mode_counter = 0;
u8 setup_editor_counter = 0;
u8 setup_jump = 0;
u8 konami_counter = 0;

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

		if (!performance_xy_enabled) {
			rgb_led(78, setup_performance_xy_r >> 2, setup_performance_xy_g >> 2, setup_performance_xy_b >> 2); // Drum Rack layout selected
		} else {
			rgb_led(78, setup_performance_xy_r, setup_performance_xy_g, setup_performance_xy_b); // XY layout selected
		}
	}

	if (mode_default == mode_ableton) {
		for (u8 x = 0; x < 2; x++)
			for (u8 y = 0; y < 3; y++) {
				u8 set = ableton_channel == 5 + 8 * x + y;
				rgb_led(
					(2 - x) * 10 + 6 + y,
					set? setup_channel_selected_r : setup_channel_deselected_r,
					set? setup_channel_selected_g : setup_channel_deselected_g, 
					set? setup_channel_selected_b : setup_channel_deselected_b
				);
			}
	}

	if (mode_note <= mode_default && mode_default <= mode_piano) {
		for (u8 x = 0; x < 4; x++)
			for (u8 y = 0; y < 4; y++) {
				u8 set = channels[mode_default - mode_note] == 4 * x + y;
				rgb_led(
					(4 - x) * 10 + 5 + y,
					set? setup_channel_selected_r : setup_channel_deselected_r,
					set? setup_channel_selected_g : setup_channel_deselected_g, 
					set? setup_channel_selected_b : setup_channel_deselected_b
				);
			}
	}

	if (mode_default == mode_custom) {
		if (custom_surface_led) {
			rgb_led(15, setup_custom_on_r, setup_custom_on_g, setup_custom_on_b);
		} else {
			rgb_led(15, setup_custom_off_r, setup_custom_off_g, setup_custom_off_b);
		}

		if (custom_midi_led) {
			rgb_led(16, setup_custom_on_r, setup_custom_on_g, setup_custom_on_b);
		} else {
			rgb_led(16, setup_custom_off_r, setup_custom_off_g, setup_custom_off_b);
		}
		
		if (custom_fader_vel_sensitive) {
			rgb_led(18, setup_custom_on_r, setup_custom_on_g, setup_custom_on_b);
		} else {
			rgb_led(18, setup_custom_off_r, setup_custom_off_g, setup_custom_off_b);
		}
	}

	if (idle_enabled) {
		rgb_led(41, setup_idle_r, setup_idle_g, setup_idle_b); // Idle animation enabled
	} else {
		rgb_led(41, setup_idle_r >> 2, setup_idle_g >> 2, setup_idle_b >> 2); // Idle animation disabled
	}

	if (vel_sensitive) {
		rgb_led(21, setup_velocity_r, setup_velocity_g, setup_velocity_b); // Velocity sensitivity enabled
	} else {
		rgb_led(21, setup_velocity_r >> 2, setup_velocity_g >> 2, setup_velocity_b >> 2); // Velocity sensitivity disabled
	}

	if (mode_default != mode_fader) {
		for (u8 i = 11; i < 14; i++) {
			rgb_led(i, setup_aftertouch_r >> 2, setup_aftertouch_g >> 2, setup_aftertouch_b >> 2); // Aftertouch
		}

		rgb_led(aftertouch_enabled + 11, setup_aftertouch_r, setup_aftertouch_g, setup_aftertouch_b); // Aftertouch selected
	}

	if (mode_default != mode_ableton) {
		if (direct_enabled) {
			rgb_led(22, setup_direct_r, setup_direct_g, setup_direct_b); // Direct MIDI enabled
		} else {
			rgb_led(22, setup_direct_r >> 2, setup_direct_g >> 2, setup_direct_b >> 2); // Direct MIDI disabled
		}
	}

	for (int i = 0; i < 8; i++) { 
		if (led_brightness == i) { 
			rgb_led(10 * i + 19, setup_brightness_selected_r, setup_brightness_selected_g, setup_brightness_selected_b); // Selected LED brightness
		} else {
			rgb_led(10 * i + 19, setup_brightness_r, setup_brightness_g, setup_brightness_b); // LED Brightness slider
		}
	}

	rgb_led(81, mode_performance_r >> 2, mode_performance_g >> 2, mode_performance_b >> 2); // Performance mode
	rgb_led(82, mode_ableton_r >> 2, mode_ableton_g >> 2, mode_ableton_b >> 2); // Ableton mode
	rgb_led(83, mode_note_r >> 2, mode_note_g >> 2, mode_note_b >> 2); // Note mode
	rgb_led(71, mode_drum_r >> 2, mode_drum_g >> 2, mode_drum_b >> 2); // Drum mode
	rgb_led(72, mode_fader_r >> 2, mode_fader_g >> 2, mode_fader_b >> 2); // Fader mode
	rgb_led(73, mode_programmer_r >> 2, mode_programmer_g >> 2, mode_programmer_b >> 2); // Programmer mode
	rgb_led(61, mode_piano_r >> 2, mode_piano_g >> 2, mode_piano_b >> 2); // Piano mode
	rgb_led(62, mode_custom_r >> 2, mode_custom_g >> 2, mode_custom_b >> 2); // Custom mode

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

		case 6:
			rgb_led(61, mode_piano_r, mode_piano_g, mode_piano_b); // Piano mode selected
			break;

		case 7:
			rgb_led(62, mode_custom_r, mode_custom_g, mode_custom_b); // Custom mode selected
			break;
	}
	
	setup_elapsed = setup_tick;
	konami_counter = 0;
}

void setup_timer_event() {
	if (++setup_elapsed >= setup_tick) {
		rgb_led(99, setup_rainbow(setup_mode_counter, 0), setup_rainbow(setup_mode_counter, 1), setup_rainbow(setup_mode_counter, 2)); // Mode LED indicator animation
		setup_mode_counter++; setup_mode_counter %= setup_rainbow_length;
		
		if (mode_default == mode_performance && palette_selected < palette_custom) {
			rgb_led(25, setup_rainbow(setup_mode_counter, 0), setup_rainbow(setup_mode_counter, 1), setup_rainbow(setup_mode_counter, 2));  // Enter palette editor button animation
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
		
		} else if (p == 41) { // Toggle idle animation
			idle_enabled = (idle_enabled)? 0 : 1;
			dirty = 1;
			mode_refresh();
		
		} else if (p == 21) { // Toggle velocity sensitivity
			vel_sensitive = (vel_sensitive)? 0 : 1;
			dirty = 1;
			mode_refresh();

		} else if (mode_default != mode_fader && 11 <= p && p <= 13) { // Toggle aftertouch
			aftertouch_enabled = p - 11;
			dirty = 1;
			mode_refresh();

		} else if (mode_default != mode_ableton && p == 22) { // Toggle direct MIDI
			direct_enabled = (direct_enabled)? 0 : 1;
			dirty = 1;
			mode_refresh();

		} else if (1 <= x && x <= 8 && y == 9) { // LED brightness adjust
			led_brightness = x - 1;
			dirty = 1;
			mode_refresh();

		} else if (81 <= p && p <= 83) { // Switch default mode
			mode_default = p - 81;
			mode_refresh();
		
		} else if (71 <= p && p <= 73) {
			mode_default = p - 68;
			mode_refresh();
		
		} else if (61 <= p && p <= 62) {
			mode_default = p - 55;
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
			
			} else if (p == 78) { // Change DR or XY layout
				performance_xy_enabled = (performance_xy_enabled)? 0 : 1;
				dirty = 1;
				mode_refresh();
			}
		
		} else if (mode_default == mode_ableton) {
			for (u8 i = 2; i >= 1; i--)
				if (i * 10 + 6 <= p && p <= i * 10 + 8) {
					ableton_channel = p + 15 - 18 * i;
					dirty = 1;
					mode_refresh();
					break;
				}
		
		} else if (mode_note <= mode_default && mode_default <= mode_piano) {
			for (u8 i = 4; i >= 1; i--)
				if (i * 10 + 5 <= p && p <= i * 10 + 8) {
					channels[mode_default - mode_note] = p + 11 - 14 * i;
					dirty = 1;
					mode_refresh();
					break;
				}
		
		} else if (mode_default == mode_custom) {
			if (p == 15) {
				custom_surface_led = (custom_surface_led)? 0 : 1;
				dirty = 1;
				mode_refresh();

			} else if (p == 16) {
				custom_midi_led = (custom_midi_led)? 0 : 1;
				dirty = 1;
				mode_refresh();

			} else if (p == 18) {
				custom_fader_vel_sensitive = (custom_fader_vel_sensitive)? 0 : 1;
				dirty = 1;
				mode_refresh();
			}
		}

		if (p == konami[konami_counter]) {
			if (++konami_counter == konami_length) {
				mode_update(mode_puyo);
				setup_jump = 0;
			}
			rgb_led(p, konami_r, konami_g, konami_b);

		} else {
			konami_counter = 0;
		}
	
	} else { // Note released
		if (p == 0 && setup_jump) { // Quickly jump back to main mode
			mode_update(mode_default);
			setup_jump = 0;
		
		} else if (91 <= p && p <= 94) {
			rgb_led(p, 0, 0, 0);
		}
	}
}

void setup_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {}

void setup_aftertouch_event(u8 v) {}

void setup_poly_event(u8 p, u8 v) {}