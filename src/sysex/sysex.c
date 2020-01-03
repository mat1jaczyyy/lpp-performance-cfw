#include "sysex/sysex.h"

void handle_sysex(u8 port, u8 * d, u16 l) {
    // Device Inquiry - Read information about the connected device
	if (!memcmp(d, &syx_device_inquiry[0], syx_device_inquiry_length)) {
		hal_send_sysex(port, &syx_device_inquiry_response[0], syx_device_inquiry_response_length);
		return;
	}
	
	// Challenge from the Control Surface
	if (!memcmp(d, &syx_challenge[0], syx_challenge_length)) {
		if (port == USBMIDI) {
			if (l == 12) {
				u32 result = (*         // wtf?
				    (u32 (*)(u32))      // cast to function pointer
				    *(u32*)0x080000EC   // grab pointer to challenge function from table
				)(                      // call
				    *(u32*)(d + 7)      // grab Live's challenge value
				);                      // :b1:

				syx_challenge_response[7] = result & 0x7F;
				syx_challenge_response[8] = (result >> 8) & 0x7F;
				
				hal_send_sysex(USBMIDI, &syx_challenge_response[0], syx_challenge_response_length);
			
			} else if (l == 8) { // Live Quit Message
				mode_default_update(mode_performance);
			}
		}
		return;
	}
	
	// Mode selection - return the status
	if (!memcmp(d, &syx_mode_selection[0], syx_mode_selection_length)) {
		u8 new;
		syx_mode_selection_response[7] = new = *(d + 7) & 1;
		
		hal_send_sysex(port, &syx_mode_selection_response[0], syx_mode_selection_response_length);
		
		if (!(mode != mode_ableton && new)) mode_default_update(mode_ableton - new); // This will interrupt boot animation!
		
		return;
	}
	
	// Live layout selection - return the status
	if (!memcmp(d, &syx_live_layout_selection[0], syx_live_layout_selection_length)) {
		syx_live_layout_selection_response[7] = ableton_layout = *(d + 7);
		
		hal_send_sysex(port, &syx_live_layout_selection_response[0], syx_live_layout_selection_response_length);

		if (mode == mode_ableton) {
			if (ableton_layout == ableton_layout_user) {
				for (u8 i = 0; i < 100; i++) ableton_screen[i][1] = 0;
			}
			
			if (ableton_layout == ableton_layout_note_blank) {
				for (u8 x = 10; x < 90; x += 10) {
					for (u8 y = 1; y < 9; y++) ableton_screen[x + y][1] = 0;
				}
			}
			
			mode_refresh();
		}
		return;
	}
	
	// Standalone layout selection - return the status
	if (!memcmp(d, &syx_standalone_layout_selection[0], syx_standalone_layout_selection_length)) {
		if (mode_default != 1) { // If not in Ableton mode
			u8 new; 
			syx_standalone_layout_selection_response[7] = new = *(d + 7);
			
			hal_send_sysex(port, &syx_standalone_layout_selection_response[0], syx_standalone_layout_selection_response_length);
			
			mode_default_update((new < 4)? (new + mode_note) : mode_performance); // 4 for Performance mode (unavailable on stock)
		}
		return;
	}
	
	// Light LED using SysEx
	if (!memcmp(d, &syx_led_single[0], syx_led_single_length)) {
		if (active_port != port) return;

		if (mode < mode_normal) {
			for (u8 i = 7; i <= l - 3 && i <= 199; i += 2) {
				if (mode == mode_performance) {
					performance_led(0xF, *(d + i), *(d + i + 1), 1);
				} else if (mode == mode_programmer) {
					programmer_led(0x0, *(d + i), *(d + i + 1), 1);
				} else {
					novation_led(*(d + i), *(d + i + 1));
				}
			}
		}
		return;
	}
	
	// Light a column of LEDs using SysEx
	if (!memcmp(d, &syx_led_column[0], syx_led_column_length)) {
		if (active_port != port) return;

		if (mode < mode_normal) {
			u8 y = *(d + 7);
			for (u8 i = 8; i <= l - 2 && i <= 17; i++) {
				if (mode == mode_performance) {
					performance_led(0xF, (i - 8) * 10 + y, *(d + i), 1);
				} else if (mode == mode_programmer) {
					programmer_led(0x0, (i - 8) * 10 + y, *(d + i), 1);
				} else {
					novation_led((i - 8) * 10 + y, *(d + i));
				}
			}
		}
		return;
	}
	
	// Light a row of LEDs using SysEx
	if (!memcmp(d, &syx_led_row[0], syx_led_row_length)) {
		if (active_port != port) return;

		if (mode < mode_normal) {
			u8 x = *(d + 7) * 10;
			for (u8 i = 8; i <= l - 2 && i <= 17; i++) {
				if (mode == mode_performance) {
					performance_led(0xF, x + i - 8, *(d + i), 1);
				} else if (mode == mode_programmer) {
					programmer_led(0x0, x + i - 8, *(d + i), 1);
				} else {
					novation_led(x + i - 8, *(d + i));
				}
			}
		}
		return;
	}
	
	// Light all LEDs using SysEx
	if (!memcmp(d, &syx_led_all[0], syx_led_all_length)) {
		if (active_port != port) return;

		if (mode < mode_normal) {
			u8 v = *(d + 7);
			for (u8 p = 1; p < 99; p++)	{
				if (mode == mode_performance) {
					performance_led(0xF, p, v, 1);
				} else if (mode == mode_programmer) {
					programmer_led(0x0, p, v, 1);
				} else {
					novation_led(p, v);
				}
			}
		}
		return;
	}
	
	// Flash LED using SysEx
	if (!memcmp(d, &syx_led_flash[0], syx_led_flash_length)) {
		if (active_port != port) return;

		if (mode < mode_normal) {
			for (u8 i = 7; i <= l - 3 && i <= 199; i += 2) {
				if (mode == mode_performance) {
					performance_led(0xB, *(d + i), *(d + i + 1), 1);
				} else if (mode == mode_programmer) {
					programmer_led(0x1, *(d + i), *(d + i + 1), 1);
				} else {
					flash_led(*(d + i), *(d + i + 1));
				}
			}
		}
		return;
	}

	// Pulse LED using SysEx
	if (!memcmp(d, &syx_led_pulse[0], syx_led_pulse_length)) {
		if (active_port != port) return;

		if (mode < mode_normal) {
			for (u8 i = 7; i <= l - 3 && i <= 199; i += 2) {
				if (mode == mode_performance) {
					performance_led(0xC, *(d + i), *(d + i + 1), 1);
				} else if (mode == mode_programmer) {
					programmer_led(0x2, *(d + i), *(d + i + 1), 1);
				} else {
					pulse_led(*(d + i), *(d + i + 1));
				}
			}
		}
		return;
	}

	// Light LED using SysEx (RGB mode)
	if (!memcmp(d, &syx_led_rgb[0], syx_led_rgb_length)) {
		if (active_port != port) return;

		if (mode < mode_normal) {
			for (u16 i = 7; i <= l - 5 && i <= 315; i += 4) {
				if (mode == mode_performance) {
					performance_led_rgb(0xF, *(d + i), *(d + i + 1), *(d + i + 2), *(d + i + 3), 1);
				} else if (mode == mode_programmer) {
					programmer_led_rgb(0x0, *(d + i), *(d + i + 1), *(d + i + 2), *(d + i + 3), 1);
				} else {
					rgb_led(*(d + i), *(d + i + 1), *(d + i + 2), *(d + i + 3));
				}
			}
		}
		return;
	}

	// Light LED using SysEx (RGB mode) - custom fast message
	if (!memcmp(d, &syx_led_rgb_fast[0], syx_led_rgb_fast_length)) {
		if (active_port != port) return;

		if (mode < mode_normal) {
			if (mode == mode_performance) {
				performance_led_rgb(0xF, *(d + 2), *(d + 3), *(d + 4), *(d + 5), 1);
			} else if (mode == mode_programmer) {
				programmer_led_rgb(0x0, *(d + 2), *(d + 3), *(d + 4), *(d + 5), 1);
			} else {
				rgb_led(*(d + 2), *(d + 3), *(d + 4), *(d + 5));
			}
		}
		return;
	}
	
	// Light LED grid using SysEx (RGB mode)
	if (!memcmp(d, &syx_led_grid[0], syx_led_grid_length)) {
		if (active_port != port) return;

		if (mode < mode_normal) {
			u8 t = *(d + 7);

			u16 ceil; u8 p;
			if (t) {
				ceil = 197;
				p = 11;
			} else {
				ceil = 305;
				p = 0;
			}

			for (u16 i = 8; i <= l - 4 && i <= ceil; i += 3) {
				if (mode == mode_performance) {
					performance_led_rgb(0xF, p, *(d + i), *(d + i + 1), *(d + i + 2), 1);
				} else if (mode == mode_programmer) {
					programmer_led_rgb(0x0, p, *(d + i), *(d + i + 1), *(d + i + 2), 1);
				} else {
					rgb_led(p, *(d + i), *(d + i + 1), *(d + i + 2));
				}

				if (++p % 10 == 9 && t) {
					p += 2;
				}
			}
		}
		return;
	}
	
	// Text scrolling
	if (!memcmp(d, &syx_text[0], syx_text_length)) {
		if (active_port != port) return;

		if (mode < mode_normal) {
			if (l <= 10) { // Empty message
				if (mode == mode_text && port == text_port && !text_palette) mode_update(mode_default); // Stops the text scrolling

			} else if ((mode_default == mode_ableton && port == USBMIDI) || (mode_default != mode_ableton && port != USBMIDI)) { // Valid message
				text_port = port;
				text_color = *(d + 7) & 127;
				text_loop = *(d + 8) != 0;

				u16 bp = 1;

				for (u16 i = 9; i < l - 1; i++) {
					u8 v = *(d + i);
					if ((1 <= v && v <= 7) || (32 <= v && v <= 127)) {
						text_bytes[bp++] = v;
					}
				}

				for (u16 i = bp; i < text_bytes[0]; i++) text_bytes[i] = 0; // Clear rest of array

				text_bytes[0] = bp; // Stop reading bytes at this offset

				mode_update(mode_text);
			}
		}
		return;
	}
	
	// Retina palette download start
	if (!memcmp(d, &syx_palette_start[0], syx_palette_start_length)) {
		if (mode < mode_normal) {
			if (!text_palette) {
				text_palette = 1;

				app_sysex_event(port, &syx_palette_text[0], syx_palette_text_length);
			}
		}
		return;
	}
	
	// Retina palette download write
	if (!memcmp(d, &syx_palette_write[0], syx_palette_write_length)) {
		if (mode < mode_normal) {
			if (text_palette) {
				u8 j = *(d + 8);

				if (j < 3) {
					for (u16 i = 9; i <= l - 5 && i <= 313; i += 4) {
						dirty = 1;

						for (u8 k = 0; k < 3; k++) {
							palette[j][k][*(d + i)] = *(d + i + k + 1);
						}
					}
				}
			}
		}
		return;
	}
	
	// Retina palette download end
	if (!memcmp(d, &syx_palette_end[0], syx_palette_end_length)) {
		if (mode < mode_normal) {
			if (text_palette) {
				text_palette = 0;
				mode_update(mode_default);
			}
		}
		return;
	}
	
	// Create fader control
	if (!memcmp(d, &syx_fader[0], syx_fader_length)) {
		if ((mode == mode_fader && port == USBSTANDALONE) || (mode == mode_ableton && port == USBMIDI && (ableton_layout == ableton_layout_fader_device || ableton_layout == ableton_layout_fader_volume || ableton_layout == ableton_layout_fader_pan || ableton_layout == ableton_layout_fader_sends))) {
			for (u8 i = 7; i <= l - 5 && i <= 35; i += 4) {
				u8 y = *(d + i) & 7;
				
				fader_counter[fader_mode][y] = 0;
				fader_type[fader_mode][y] = *(d + i + 1) & 1;
				fader_color[fader_mode][y] = *(d + i + 2);
				faders[fader_mode][y] = *(d + i + 3);
				
				fader_draw(y);
			}
		}
		return;
	}
}
