#include "sysex/sysex.h"

const syx_declare(device_inquiry) = {0x7E, 0x7F, 0x06, 0x01, 0xF7};
const syx_declare(device_inquiry_response) = {0x7E,
                                              0x00,                                                  // Device ID
                                              0x06, 0x02, 0x00, 0x20, 0x29, 0x51, 0x00, 0x00, 0x00,  // Constant
                                              0x00, 0x63, 0x66, 0x79};                               // Firmware rev. (4 bytes) - CFY (CFW++++)

syx_declare_len(response_buffer, 319) = {};

const syx_declare_len(novation_header, 5) = {0x00, 0x20, 0x29, 0x02, 0x10};
const syx_declare_len(palette_header, 6) = {0x52, 0x45, 0x54, 0x49, 0x4E, 0x41};
const syx_declare_len(custom_header, 6) = {0x43, 0x55, 0x53, 0x54, 0x4F, 0x4D};

#define uploading_start 0x7B
#define uploading_write 0x3D
#define uploading_end 0x7D

void fast_led(u8 p, u8 r, u8 g, u8 b) {
	if (mode == mode_performance) performance_led_rgb(0xF, p, r, g, b, 1);
	else if (mode == mode_programmer) programmer_led_rgb(0x0, p, r, g, b, 1);
	else rgb_led(p, r, g, b);
}

void fast_flash(u8 p, u8 r, u8 g, u8 b) {
	if (mode == mode_performance) performance_led_rgb(0xB, p, r, g, b, 1);
	else if (mode == mode_programmer) programmer_led_rgb(0x1, p, r, g, b, 1);
	else flash_rgb_led(p, r, g, b);
}

void fast_pulse(u8 p, u8 r, u8 g, u8 b) {
	if (mode == mode_performance) performance_led_rgb(0xC, p, r, g, b, 1);
	else if (mode == mode_programmer) programmer_led_rgb(0x2, p, r, g, b, 1);
	else pulse_rgb_led(p, r, g, b);
}

void regular_led(u8 p, u8 v) {
	if (mode == mode_performance) performance_led(0xF, p, v, 1);
	else if (mode == mode_programmer) programmer_led(0x0, p, v, 1);
	else novation_led(p, v);
}

u8 palette_modifying = 0;
u8 custom_modifying = 0;

void handle_sysex(u8 port, u8* d, u16 l) {
	#define advance(x) d += x; l -= x

	if (*d != 0xF0) return;
	u8* end = d + l - 1;

	advance(1);

	// Light LEDs using SysEx (for Heaven)
	if (d[0] == 0x5F) {
		if (active_port != port || mode >= mode_normal) return;

		/*
		Stock firmware uses the format PP RR GG BB repeatable in a single SysEx message. It's ok but doesn't reach maximum efficiency.
		Max SysEx message size on Pro is 320 bytes, meaning it'd be impossible to pack a full screen update (as is the case on stock firmware too).

		I propose the following format for CFY:
		RR GG BB NN {XX XX XX... (NN times)}

		RR GG BB is the color.
		NN is the number of LEDs that we are updating to that color.
		XX XX XX... are the pitches of all the LEDs that we want to update to the same color.
		1-99 control normal grid, 99 is mode light. 0 sets the entire grid of LEDs.
		100-109 sets an entire row, while 110-119 sets an entire column.

		If RR, GG, or BB have 7th bits set, read them out for up to 7 pitch entries.
		Otherwise, read NN.

		This set is repeatable in a single message, and I believe to be the best *feasible* compression format that we can do.
		Other stuff just requires too much CPU on Apollo's side to work.
		*/

		for (u8* i = d + 1; i < end;) {
			u8 r = *i++;
			u8 g = *i++;
			u8 b = *i++;

			u8 n = ((r & 0x40) >> 4) | ((g & 0x40) >> 5) | ((b & 0x40) >> 6);
			if (n == 0) n = *i++;

			r &= 0x3F;
			g &= 0x3F;
			b &= 0x3F;

			for (u8 j = 0; j < n; j++) {
				u8 x = *i++;

				if (x == 0)
					for (u8 k = 0; k < 99; k++)
						fast_led(k, r, g, b);

				else if (x <= 99)
					fast_led(x, r, g, b);

				else if (x <= 109) {
					x = (x - 100) * 10 + 1;

					for (u8 k = x; k < x + 8; k++)
						fast_led(k, r, g, b);

				} else if (x <= 119) {
					x -= 100;

					for (u8 k = x; k < 90; k += 10)
						fast_led(k, r, g, b);
				}
			}
		}

	// Light LED using SysEx (RGB mode) - custom fast message
	} else if (d[0] == 0x6F) {
		if (active_port != port || mode >= mode_normal) return;

		for (u8* i = d + 1; i < end; i += 4)
			fast_led(i[0], i[1], i[2], i[3]);

	// Flash LED using SysEx (RGB mode) - custom fast message
	} else if (d[0] == 0x6B) {
		if (active_port != port || mode >= mode_normal) return;

		for (u8* i = d + 1; i < end; i += 4)
			fast_flash(i[0], i[1], i[2], i[3]);

	// Pulse LED using SysEx (RGB mode) - custom fast message
	} else if (d[0] == 0x6C) {
		if (active_port != port || mode >= mode_normal) return;

		for (u8* i = d + 1; i < end; i += 4)
			fast_pulse(i[0], i[1], i[2], i[3]);

	// Device Inquiry - Read information about the connected device
	} else if (syx_match(device_inquiry)) {
		syx_resp_cpy(device_inquiry_response);
		syx_response_buffer[sizeof(syx_device_inquiry_response) + 1] = 0xF7;
		syx_send(port, sizeof(syx_device_inquiry_response) + 2);

	// Novation stock SysEx messages
	} else if (syx_match(novation_header)) {
		advance(sizeof(syx_novation_header));

		// Challenge from the Control Surface
		if (d[0] == 0x40) {
			if (port != USBMIDI) return;

			if (l == 6) {
				u32 result = (*         // wtf?
					(u32 (*)(u32))      // cast to function pointer
					*(u32*)0x080000EC   // grab pointer to challenge function from table
				)(                      // call
					*(u32*)(d + 1)      // grab Live's challenge value
				);                      // :b1:

				syx_resp_cpy(novation_header);
				syx_response_buffer[sizeof(syx_novation_header) + 1] = 0x40;
				syx_response_buffer[sizeof(syx_novation_header) + 2] = result & 0x7F;
				syx_response_buffer[sizeof(syx_novation_header) + 3] = (result >> 8) & 0x7F;
				syx_response_buffer[sizeof(syx_novation_header) + 4] = 0xF7;
				syx_send(USBMIDI, sizeof(syx_novation_header) + 5);

			} else if (l == 2) // Also Live Quit Message!
				mode_default_update(mode_performance);

		// Mode selection - return the status
		} else if (d[0] == 0x21) {
			u8 new = d[1] & 1;

			syx_resp_cpy(novation_header);
			syx_response_buffer[sizeof(syx_novation_header) + 1] = 0x2D;
			syx_response_buffer[sizeof(syx_novation_header) + 2] = new;
			syx_response_buffer[sizeof(syx_novation_header) + 3] = 0xF7;
			syx_send(port, sizeof(syx_novation_header) + 4);

			if (!(mode != mode_ableton && new))
				mode_default_update(mode_ableton - new); // This will interrupt boot animation!

		// Live layout selection - return the status
		} else if (d[0] == 0x22) {
			ableton_layout = d[1];

			syx_resp_cpy(novation_header);
			syx_response_buffer[sizeof(syx_novation_header) + 1] = 0x2E;
			syx_response_buffer[sizeof(syx_novation_header) + 2] = d[1];
			syx_response_buffer[sizeof(syx_novation_header) + 3] = 0xF7;
			syx_send(port, sizeof(syx_novation_header) + 4);

			if (mode != mode_ableton) return;

			if (ableton_layout == ableton_layout_user)
				for (u8 i = 0; i < 100; i++)
					ableton_screen[i][1] = 0;

			if (ableton_layout == ableton_layout_note_blank)
				for (u8 x = 10; x < 90; x += 10)
					for (u8 y = 1; y < 9; y++)
						ableton_screen[x + y][1] = 0;

			mode_refresh();

		// Standalone layout selection - return the status
		} else if (d[0] == 0x2C) {
			if (mode_default == mode_ableton) return; // If not in Ableton mode

			syx_resp_cpy(novation_header);
			syx_response_buffer[sizeof(syx_novation_header) + 1] = 0x2F;
			syx_response_buffer[sizeof(syx_novation_header) + 2] = d[1];
			syx_response_buffer[sizeof(syx_novation_header) + 3] = 0xF7;
			syx_send(port, sizeof(syx_novation_header) + 4);

			mode_default_update(d[1] < 4? d[1] + mode_note : mode_performance); // 4 for Performance mode (unavailable on stock)

		// Create fader control
		} else if (d[0] == 0x2B) {
			if ((mode == mode_fader && port == USBSTANDALONE) || (
				mode == mode_ableton && port == USBMIDI && (
					ableton_layout == ableton_layout_fader_device ||
					ableton_layout == ableton_layout_fader_volume ||
					ableton_layout == ableton_layout_fader_pan ||
					ableton_layout == ableton_layout_fader_sends
				)
			))
				for (u8 i = 1; i <= (s16)l - 5 && i <= 29; i += 4) {
					u8 y = d[i] & 0x07;

					fader_counter[fader_mode][y] = 0;
					fader_type[fader_mode][y] = d[i + 1] & 1;
					fader_color[fader_mode][y] = d[i + 2];
					faders[fader_mode][y] = d[i + 3];

					fader_draw(y);
				}

		} else {
			if (active_port != port || mode >= mode_normal) return;

			// Light LED using SysEx
			if (d[0] == 0x0A)
				for (u8 i = 1; i <= (s16)l - 3 && i <= 193; i += 2)
					regular_led(d[i], d[i + 1]);

			// Flash LED using SysEx
			else if (d[0] == 0x23) {
				for (u8 i = 1; i <= (s16)l - 3 && i <= 193; i += 2) {
					if (mode == mode_performance) performance_led(0xB, d[i], d[i + 1], 1);
					else if (mode == mode_programmer) programmer_led(0x1, d[i], d[i + 1], 1);
					else flash_led(d[i], d[i + 1]);
				}

			// Pulse LED using SysEx
			} else if (d[0] == 0x28) {
				for (u8 i = 1; i <= (s16)l - 3 && i <= 193; i += 2) {
					if (mode == mode_performance) performance_led(0xC, d[i], d[i + 1], 1);
					else if (mode == mode_programmer) programmer_led(0x2, d[i], d[i + 1], 1);
					else pulse_led(d[i], d[i + 1]);
				}

			// Light a column of LEDs using SysEx
			} else if (d[0] == 0x0C)
				for (u8 i = 2; i <= (s16)l - 2 && i <= 11; i++)
					regular_led((i - 8) * 10 + d[1], d[i]);

			// Light a row of LEDs using SysEx
			else if (d[0] == 0x0D) {
				u8 x = d[1] * 10;

				for (u8 i = 2; i <= (s16)l - 2 && i <= 11; i++)
					regular_led(x + i - 8, d[i]);

			// Light all LEDs using SysEx
			} else if (d[0] == 0x0E)
				for (u8 p = 1; p < 99; p++)
					regular_led(p, d[1]);

			// Light LED using SysEx (RGB mode)
			else if (d[0] == 0x0B)
				for (u16 i = 1; i <= (s16)l - 5 && i <= 309; i += 4)
					fast_led(d[i], d[i + 1], d[i + 2], d[i + 3]);

			// Light LED grid using SysEx (RGB mode)
			else if (d[0] == 0x0F) {
				u16 ceil = d[1]? 193 : 299;
				u8 p = d[1]? 11 : 0;

				for (u16 i = 2; i <= (s16)l - 4 && i <= ceil; i += 3) {
					fast_led(p, d[i], d[i + 1], d[i + 2]);

					if (++p % 10 == 9 && d[1])
						p += 2;
				}

			// Text scrolling
			} else if (d[0] == 0x14) {
				if (l <= 4) { // Empty message
					if (mode == mode_text && port == text_port)
						mode_update(mode_default); // Stops the text scrolling

				} else if ((mode_default == mode_ableton && port == USBMIDI) || (mode_default != mode_ableton && port != USBMIDI)) { // Valid message
					text_port = port;
					text_color = d[1] & 0x7F;
					text_loop = d[2] != 0;

					u16 bp = 0;

					for (u16 i = 3; i < l - 1; i++)
						if ((1 <= d[i] && d[i] <= 7) || (32 <= d[i] && d[i] <= 127))
							text_bytes[bp++] = d[i];

					text_size = bp; // Stop reading bytes at this offset

					for (; bp < sizeof(text_bytes); bp++)
						text_bytes[bp] = 0; // Clear rest of array

					mode_update(mode_text);
				}
			}
		}

	// Retina palette download
	} else {
		if (mode >= mode_normal) return;

		// Retina palette download
		if (syx_match(palette_header)) {
			advance(sizeof(syx_palette_header));

			// Start
			if (d[0] == uploading_start)
				palette_modifying = 1;

			else {
				if (!palette_modifying) return;

				// Write
				if (d[0] == uploading_write) {
					for (u16 i = 1; i <= (s16)l - 5 && i <= 305; i += 4) {
						dirty = 1;

						for (u8 k = 0; k < 3; k++)
							editor_palette[k][d[i] & 0x7F] = d[i + k + 1] & 0x3F;
					}

				// End
				} else if (d[0] == uploading_end) {
					palette_modifying = 0;

					if (d[1] <= 3)
						dirty_palette = d[1];

					mode_update(mode_default);
				}
			}

		// Custom mode download
		} else if (syx_match(custom_header)) {
			advance(sizeof(syx_custom_header));

			// Start
			if (d[0] == uploading_start) {
				if (d[1] >= 8) return;

				custom_modifying = 1;
				custom_upload_start(d[1]);

			} else {
				if (!custom_modifying) return;

				// Write
				if (d[0] == uploading_write)
					custom_upload_push(d + 1);

				// End
				else if (d[0] == uploading_end) {
					custom_upload_end();

					custom_modifying = 0;
					mode_refresh();
				}
			}
		}
	}
}
