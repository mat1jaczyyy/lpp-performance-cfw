/******************************************************************************
 
  Copyright (c) 2015, Focusrite Audio Engineering Ltd.
  All rights reserved.
 
  Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 * Neither the name of Focusrite Audio Engineering Ltd., nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.
 
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
    Performance-optimized Launchpad Pro Firmware modification by mat1jaczyyy
 
 ******************************************************************************/

/*
  NOTE: TO USE THIS FIRMWARE WITH ABLETON, YOU NEED TO PATCH THE LAUNCHPPAD PRO
 CONTROL SURFACE. THIS IS DUE TO THE UMAC HASH CHECK (encrypt_challenge2) WHOSE
 PURPOSE IS TO ENSURE THE DEVICE IS A LEGITIMATE LAUNCHPAD PRO DEVICE. ONLY THE
 CLOSED-SOURCE STOCK FIRMWARE IS ABLE TO CORRECTLY RESPOND TO THIS MESSAGE.
                                                                              */

/*    Includes and helpers    */
/*----------------------------*/

#include <string.h> // array operations
#include "app.h"    // HAL interface

u8 math_pow(u8 x, u8 e) {
	u8 y = 1;
	for (u8 j = 0; j < e; j++) {
		y *= x;
	}
	return y;
}

#define arr_size(x) (sizeof(x) / sizeof((x)[0]))
#define syx_cmp(x, y, n) (arr_size(y) == n && memcmp(x, &y[0], n) == 0)

u8 xy_dr[128] = {0, 116, 117, 118, 119, 120, 121, 122, 123, 0, 115, 36, 37, 38, 39, 68, 69, 70, 71, 107, 114, 40, 41, 42, 43, 72, 73, 74, 75, 106, 113, 44, 45, 46, 47, 76, 77, 78, 79, 105, 112, 48, 49, 50, 51, 80, 81, 82, 83, 104, 111, 52, 53, 54, 55, 84, 85, 86, 87, 103, 110, 56, 57, 58, 59, 88, 89, 90, 91, 102, 109, 60, 61, 62, 63, 92, 93, 94, 95, 101, 108, 64, 65, 66, 67, 96, 97, 98, 99, 100, 0, 28, 29, 30, 31, 32, 33, 34, 35, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
u8 dr_xy[128] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 91, 92, 93, 94, 95, 96, 97, 98, 11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34, 41, 42, 43, 44, 51, 52, 53, 54, 61, 62, 63, 64, 71, 72, 73, 74, 81, 82, 83, 84, 15, 16, 17, 18, 25, 26, 27, 28, 35, 36, 37, 38, 45, 46, 47, 48, 55, 56, 57, 58, 65, 66, 67, 68, 75, 76, 77, 78, 85, 86, 87, 88, 89, 79, 69, 59, 49, 39, 29, 19, 80, 70, 60, 50, 40, 30, 20, 10, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0};

/*   Flash-backed variables   */
/*----------------------------*/

u8 palette[4][3][128] = {
	{ // User-defined flash-backed palette
		{0},
		{0},
		{0}
	},
	{ // Novation's default palette
		{0, 7, 31, 63, 63, 63, 21, 6, 63, 63, 21, 9, 63, 63, 21, 6, 33, 20, 7, 4, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 33, 20, 6, 3, 63, 63, 21, 6, 63, 63, 21, 8, 63, 37, 29, 16, 0, 0, 0, 0, 0, 9, 31, 7, 63, 46, 43, 24, 3, 0, 0, 0, 15, 30, 43, 15, 63, 33, 28, 0, 14, 21, 13, 22, 12, 33, 52, 63, 63, 45, 35, 32, 14, 4, 3, 5, 5, 25, 41, 54, 53, 63, 39, 25, 7, 54, 31, 38, 35, 15, 28, 55, 39, 13, 6, 1, 45, 15, 44, 18},
		{0, 7, 31, 63, 18, 0, 0, 0, 46, 20, 7, 6, 63, 63, 21, 6, 63, 63, 21, 10, 63, 63, 21, 6, 63, 63, 21, 6, 63, 63, 21, 7, 63, 63, 21, 6, 48, 41, 16, 3, 33, 21, 7, 1, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 5, 13, 20, 24, 14, 21, 20, 0, 17, 0, 31, 7, 0, 63, 58, 63, 34, 63, 41, 10, 0, 0, 6, 8, 18, 55, 63, 63, 63, 63, 63, 34, 20, 31, 7, 0, 31, 43, 63, 22, 10, 18, 19, 5, 7, 14, 0, 20, 26, 55, 55, 44, 7, 63, 63, 37, 25, 15, 28, 63, 0, 0, 51, 16, 43, 12, 23, 5},
		{0, 7, 31, 63, 18, 0, 0, 0, 26, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 18, 0, 0, 0, 23, 6, 3, 0, 33, 21, 7, 4, 45, 37, 13, 4, 63, 63, 20, 6, 63, 63, 21, 6, 63, 63, 21, 6, 63, 63, 24, 11, 63, 63, 21, 6, 33, 20, 7, 4, 0, 0, 0, 0, 0, 13, 31, 63, 19, 50, 31, 7, 0, 11, 1, 2, 0, 33, 63, 63, 63, 63, 30, 0, 0, 1, 5, 0, 9, 27, 50, 63, 48, 57, 63, 22, 0, 0, 0, 1, 0, 3, 13, 10, 22, 6, 2, 15, 6, 9, 11, 3, 11, 26, 46, 63, 63, 15, 28, 63, 0, 0, 0, 0, 0, 0, 0, 0}
	},
	{ // mat1jaczyyy's custom palette
		{0, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 11, 23, 35, 51, 47, 7, 15, 23, 39, 31, 3, 7, 11, 27, 15, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 3, 7, 11, 27, 15, 7, 15, 23, 39, 31, 11, 23, 35, 51, 47, 15, 31, 47, 62, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 9, 18, 27, 36, 45, 54, 63},
		{0, 0, 0, 0, 15, 0, 3, 7, 11, 27, 15, 7, 15, 23, 39, 31, 11, 23, 35, 51, 47, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 62, 63, 11, 23, 35, 51, 47, 7, 15, 23, 39, 31, 3, 7, 11, 27, 15, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 9, 18, 27, 36, 45, 54, 63},
		{0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 0, 0, 0, 15, 0, 3, 7, 11, 27, 15, 7, 15, 23, 39, 31, 11, 23, 35, 51, 47, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 63, 63, 15, 31, 47, 62, 63, 11, 23, 35, 51, 47, 7, 15, 23, 39, 31, 3, 7, 11, 27, 15, 9, 18, 27, 36, 45, 54, 63}
	},
	{ // Launchpad S legacy palette
		{0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63, 0, 21, 42, 63},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	}
};

u8 palette_selected = 0;
u8 vel_sensitive = 0;
u8 top_lights_config = 0; // 0 = PRO, 1 = MK2, 2 = MK2 Rotated, 3 = MK2 Mirrored

/*       LED management       */
/*----------------------------*/

void palette_led(u8 p, u8 v) {
	if (p == 99) {
		hal_plot_led(TYPESETUP, 0, palette[palette_selected][0][v], palette[palette_selected][1][v], palette[palette_selected][2][v]);
	} else {
		hal_plot_led(TYPEPAD, p, palette[palette_selected][0][v], palette[palette_selected][1][v], palette[palette_selected][2][v]);
	}
}

void display_u8(u8 v, u8 d, u8 x, u8 r, u8 g, u8 b) {
	for (u8 i = 0; i < 8; i++) {
		u8 w = ((v & math_pow(2, i)) >> i);
		hal_plot_led(TYPEPAD, (d == 0)? (8 - i + x * 10) : (i * 10 + x), r * w, g * w, b * w);
	}
}

void display_u6(u8 v, u8 d, u8 x, u8 r, u8 g, u8 b) {
	for (u8 i = 0; i < 6; i++) {
		u8 w = ((v & math_pow(2, i)) >> i);
		hal_plot_led(TYPEPAD, (d == 0)? (7 - i + x * 10) : ((i + 2) * 10 + x), r * w, g * w, b * w);
	}
}

/*  Flash storage management  */
/*----------------------------*/

u8 flash[1024] = {0}; // [0, 383) = palette[0], {384} = palette_selected, {385} = vel_sensitive, {386} = top_lights_config
u8 dirty = 0;

void flash_read() {
	hal_read_flash(0, &flash[0], 1024);
	
	memcpy(&palette[0][0][0], &flash[0], 384);
	palette_selected = flash[384];
	vel_sensitive = flash[385];
	top_lights_config = flash[386];
}

void flash_write() {
	if (dirty) {
		memcpy(&flash[0], &palette[0][0][0], 384);
		flash[384] = palette_selected;
		flash[385] = vel_sensitive;
		flash[386] = top_lights_config;
		
		hal_write_flash(0, &flash[0], 1024);
		
		dirty = 0;
	}
}

/*  Modes (Launchpad states)  */
/*----------------------------*/

u8 mode = 0;

// Arrays of function pointers. Used to programmatically call the correct mode's function
void (*mode_init[256])();
void (*mode_timer_event[256])();
void (*mode_surface_event[256])(u8 p, u8 v, u8 x, u8 y);
void (*mode_midi_event[256])(u8 t, u8 ch, u8 p, u8 v);

void mode_refresh() {
	for (u8 i = 1; i < 99; i++) { // Clear all LEDs
		hal_plot_led(TYPEPAD, i, 0, 0, 0);
	}
	hal_plot_led(TYPESETUP, 0, 0, 0, 0);
	
	(*mode_init[mode])();
}

void mode_update(u8 x) {
	flash_write();
	
	mode = x;
	mode_refresh();
}

/*       Event handling       */
/*----------------------------*/

void app_timer_event() {
	(*mode_timer_event[mode])();
}

void app_surface_event(u8 t, u8 p, u8 v) {
	if (!vel_sensitive) {
		v = (v == 0)? 0 : 127;
	}

	(*mode_surface_event[mode])(p, v, p / 10, p % 10);
}

void app_midi_event(u8 port, u8 t, u8 p, u8 v) {
	if (port == USBMIDI) {
		(*mode_midi_event[mode])(t >> 4, t % 16, p, v);
	}
}

void app_aftertouch_event(u8 p, u8 v) {} // Unused
void app_cable_event(u8 t, u8 v) {} // Unused

/*       Boot animation       */
/*----------------------------*/

#define boot_note_tick 33
u8 boot_note_elapsed = boot_note_tick;
u8 boot_notes[49] = {45, 55, 56, 46, 36, 35, 65, 66, 67, 57, 47, 37, 27, 26, 25, 75, 76, 77, 78, 68, 58, 48, 38, 28, 18, 17, 16, 15, 85, 86, 87, 88, 89, 79, 69, 59, 49, 39, 29, 19, 9, 8, 7, 6, 5, 95, 96, 97, 98};
u8 boot_note_floor = 0;
u8 boot_note_ceil = 0;

#define boot_fade_tick 5
u8 boot_fade_elapsed = boot_fade_tick;
u8 boot_fade_counter[49] = {0};
u8 boot_colors[252][3] = {{0, 0, 1}, {0, 0, 2}, {0, 0, 3}, {0, 0, 4}, {0, 0, 5}, {0, 0, 6}, {0, 0, 7}, {0, 0, 8}, {0, 0, 9}, {0, 0, 10}, {0, 0, 11}, {0, 0, 12}, {0, 0, 13}, {0, 0, 14}, {0, 0, 15}, {0, 0, 16}, {0, 0, 17}, {0, 0, 18}, {0, 0, 19}, {0, 0, 20}, {0, 0, 21}, {0, 0, 22}, {0, 0, 23}, {0, 0, 24}, {0, 0, 25}, {0, 0, 26}, {0, 0, 27}, {0, 0, 28}, {0, 0, 29}, {0, 0, 30}, {0, 0, 31}, {0, 0, 32}, {0, 0, 33}, {0, 0, 34}, {0, 0, 35}, {0, 0, 36}, {0, 0, 37}, {0, 0, 38}, {0, 0, 39}, {0, 0, 40}, {0, 0, 41}, {0, 0, 42}, {0, 0, 43}, {0, 0, 44}, {0, 0, 45}, {0, 0, 46}, {0, 0, 47}, {0, 0, 48}, {0, 0, 49}, {0, 0, 50}, {0, 0, 51}, {0, 0, 52}, {0, 0, 53}, {0, 0, 54}, {0, 0, 55}, {0, 0, 56}, {0, 0, 57}, {0, 0, 58}, {0, 0, 59}, {0, 0, 60}, {0, 0, 61}, {0, 0, 62}, {0, 0, 63}, {1, 0, 63}, {2, 0, 63}, {3, 0, 63}, {4, 0, 63}, {5, 0, 63}, {6, 0, 63}, {7, 0, 63}, {8, 0, 63}, {9, 0, 63}, {10, 0, 63}, {11, 0, 63}, {12, 0, 63}, {13, 0, 63}, {14, 0, 63}, {15, 0, 63}, {16, 0, 63}, {17, 0, 63}, {18, 0, 63}, {19, 0, 63}, {20, 0, 63}, {21, 0, 63}, {22, 0, 63}, {23, 0, 63}, {24, 0, 63}, {25, 0, 63}, {26, 0, 63}, {27, 0, 63}, {28, 0, 63}, {29, 0, 63}, {30, 0, 63}, {31, 0, 63}, {32, 0, 63}, {33, 0, 63}, {34, 0, 63}, {35, 0, 63}, {36, 0, 63}, {37, 0, 63}, {38, 0, 63}, {39, 0, 63}, {40, 0, 63}, {41, 0, 63}, {42, 0, 63}, {43, 0, 63}, {44, 0, 63}, {45, 0, 63}, {46, 0, 63}, {47, 0, 63}, {48, 0, 63}, {49, 0, 63}, {50, 0, 63}, {51, 0, 63}, {52, 0, 63}, {53, 0, 63}, {54, 0, 63}, {55, 0, 63}, {56, 0, 63}, {57, 0, 63}, {58, 0, 63}, {59, 0, 63}, {60, 0, 63}, {61, 0, 63}, {62, 0, 63}, {63, 0, 63}, {63, 0, 62}, {63, 0, 61}, {63, 0, 60}, {63, 0, 59}, {63, 0, 58}, {63, 0, 57}, {63, 0, 56}, {63, 0, 55}, {63, 0, 54}, {63, 0, 53}, {63, 0, 52}, {63, 0, 51}, {63, 0, 50}, {63, 0, 49}, {63, 0, 48}, {63, 0, 47}, {63, 0, 46}, {63, 0, 45}, {63, 0, 44}, {63, 0, 43}, {63, 0, 42}, {63, 0, 41}, {63, 0, 40}, {63, 0, 39}, {63, 0, 38}, {63, 0, 37}, {63, 0, 36}, {63, 0, 35}, {63, 0, 34}, {63, 0, 33}, {63, 0, 32}, {63, 0, 31}, {63, 0, 30}, {63, 0, 29}, {63, 0, 28}, {63, 0, 27}, {63, 0, 26}, {63, 0, 25}, {63, 0, 24}, {63, 0, 23}, {63, 0, 22}, {63, 0, 21}, {63, 0, 20}, {63, 0, 19}, {63, 0, 18}, {63, 0, 17}, {63, 0, 16}, {63, 0, 15}, {63, 0, 14}, {63, 0, 13}, {63, 0, 12}, {63, 0, 11}, {63, 0, 10}, {63, 0, 9}, {63, 0, 8}, {63, 0, 7}, {63, 0, 6}, {63, 0, 5}, {63, 0, 4}, {63, 0, 3}, {63, 0, 2}, {63, 0, 1}, {63, 0, 0}, {62, 0, 0}, {61, 0, 0}, {60, 0, 0}, {59, 0, 0}, {58, 0, 0}, {57, 0, 0}, {56, 0, 0}, {55, 0, 0}, {54, 0, 0}, {53, 0, 0}, {52, 0, 0}, {51, 0, 0}, {50, 0, 0}, {49, 0, 0}, {48, 0, 0}, {47, 0, 0}, {46, 0, 0}, {45, 0, 0}, {44, 0, 0}, {43, 0, 0}, {42, 0, 0}, {41, 0, 0}, {40, 0, 0}, {39, 0, 0}, {38, 0, 0}, {37, 0, 0}, {36, 0, 0}, {35, 0, 0}, {34, 0, 0}, {33, 0, 0}, {32, 0, 0}, {31, 0, 0}, {30, 0, 0}, {29, 0, 0}, {28, 0, 0}, {27, 0, 0}, {26, 0, 0}, {25, 0, 0}, {24, 0, 0}, {23, 0, 0}, {22, 0, 0}, {21, 0, 0}, {20, 0, 0}, {19, 0, 0}, {18, 0, 0}, {17, 0, 0}, {16, 0, 0}, {15, 0, 0}, {14, 0, 0}, {13, 0, 0}, {12, 0, 0}, {11, 0, 0}, {10, 0, 0}, {9, 0, 0}, {8, 0, 0}, {7, 0, 0}, {6, 0, 0}, {5, 0, 0}, {4, 0, 0}, {3, 0, 0}, {2, 0, 0}, {1, 0, 0}, {0, 0, 0}};

void boot_init() {}

void boot_timer_event() {
	if (++boot_note_elapsed >= boot_note_tick) { // Start fading next note
		if (boot_note_ceil < 49) {
			boot_note_ceil++;
		}
		
		boot_note_elapsed = 0;
	}
	if (++boot_fade_elapsed >= boot_fade_tick) { // Redraw fades
		if (boot_note_floor == 49) { // Enter Performance mode (end condition)
			mode_update(0);
		
		} else {
			for (u8 i = boot_note_floor; i < boot_note_ceil; i++) {
				if (boot_fade_counter[i] < 251) { // Draw next fade for note
					boot_fade_counter[i]++;
					
					if (boot_notes[i] != 98 || boot_fade_counter[i] <= 178) { // Check for leaving User LED on
						hal_plot_led(TYPEPAD, boot_notes[i], boot_colors[boot_fade_counter[i]][0], boot_colors[boot_fade_counter[i]][1], boot_colors[boot_fade_counter[i]][2]);
					}
					
					hal_plot_led(TYPEPAD, 99 - boot_notes[i], boot_colors[boot_fade_counter[i]][0], boot_colors[boot_fade_counter[i]][1], boot_colors[boot_fade_counter[i]][2]); // 180 degree rotation
					
					if (boot_notes[i] == 4 || 99 - boot_notes[i] == 4) { // Mode LED
						hal_plot_led(TYPESETUP, 0, boot_colors[boot_fade_counter[i]][0], boot_colors[boot_fade_counter[i]][1], boot_colors[boot_fade_counter[i]][2]);
					}
					
				} else { // Stop fading a finished note
					boot_note_floor++;
				}
			}
		}
		
		boot_fade_elapsed = 0;
	}
}

void boot_surface_event(u8 p, u8 v, u8 x, u8 y) {}

void boot_midi_event(u8 t, u8 ch, u8 p, u8 v) {}


/*      Performance mode      */
/*----------------------------*/

void performance_init() {
	hal_plot_led(TYPEPAD, 98, 63, 0, 10); // User LED
}

void performance_timer_event() {}

void performance_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(1);
				
	} else { // Send MIDI input to DAW
		hal_send_midi(USBMIDI, 0x90, xy_dr[p], v);
	}
}

void performance_midi_event(u8 t, u8 ch, u8 p, u8 v) {
	switch (ch) {
		case 0x5: // Ch. 6
			switch (t) {
				case 0x8: // Note off
					v = 0; // We cannot assume a note off will come with velocity 0. Note, there is no break statement here!
				
				case 0x9: // Note on
					if (top_lights_config != 0) {
						if (108 <= p && p <= 115) { // Conversion of MK2 Top Lights
							p += -80;
						}
						
						if (top_lights_config > 1) { // Display additional LEDs
							if (100 <= p && p <= 107) {
								palette_led(dr_xy[(top_lights_config == 2)? (215 - p) : (16 + p)], v);
							} else if (28 <= p && p <= 35) { // p has been changed from the earlier if statement, so we must check for [28, 35] now!
								palette_led(dr_xy[(top_lights_config == 2)? (151 - p) : (80 + p)], v);
							}
						}
					}
					
					palette_led(dr_xy[p], v);
					break;
			}		
	}
}

/*        Ableton mode        */
/*----------------------------*/

u8 ableton_enabled = 0;
u8 ableton_layout = 0x0;

void ableton_led(u8 p, u8 v) {
	hal_plot_led(TYPEPAD, p, palette[1][0][v], palette[1][1][v], palette[1][2][v]);
}

void ableton_init() {
	hal_plot_led(TYPESETUP, 0, 0, 63, 0); // Live mode LED
}

void ableton_timer_event() {}

void ableton_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(1);	
	} else {
		switch (ableton_layout) {
			case 0x0: // Session mode
			case 0x6: // Session mode - Record Arm
			case 0x7: // Session mode - Track Select
			case 0x8: // Session mode - Mute
			case 0x9: // Session mode - Solo
			case 0xD: // Session mode - Stop Clip
				if (x == 0 || x == 9 || y == 0 || y == 9) {
					hal_send_midi(USBMIDI, 0xB0, p, (v == 0)? 0 : 127);
				} else {
					hal_send_midi(USBMIDI, (v == 0)? 0x80 : 0x90, p, v);
				}
				break;
			
			case 0x1: // Note mode - Drum Rack layout
				if (x == 0 || x == 9 || y == 0 || y == 9) {
					hal_send_midi(USBMIDI, 0xB0, p, (v == 0)? 0 : 127);
				} else {
					hal_send_midi(USBMIDI, (v == 0)? 0x80 : 0x90, xy_dr[p], v);
				}
				break;
			
			case 0x2: // Note mode - Chromatic layout
				if (x == 0 || x == 9 || y == 0 || y == 9) {
					hal_send_midi(USBMIDI, 0xB0, p, (v == 0)? 0 : 127);
				} else {
					// TODO: Implement true Note mode
				}
				break;
			
			case 0x4: // Note mode - Blank layout (for Audio track)
				if (x == 0 || x == 9 || y == 0 || y == 9) {
					hal_send_midi(USBMIDI, 0xB0, p, (v == 0)? 0 : 127);
				}
				break;
			
			case 0x5: // Fader - Device mode
			case 0xA: // Fader - Volume
			case 0xB: // Fader - Pan
			case 0xC: // Fader - Sends
				if (x == 0 || x == 9 || y == 0 || y == 9) {
					hal_send_midi(USBMIDI, 0xB0, p, (v == 0)? 0 : 127);
				} else {
					// TODO: Implement true Device mode
				}
				break;
			
			case 0x3: // User mode
				if (x == 9) {
					hal_send_midi(USBMIDI, 0xB5, p, (v == 0)? 0 : 127);
				} else {
					hal_send_midi(USBMIDI, (v == 0)? 0x85 : 0x95, xy_dr[p], v);
				}
				break;
		}
	}
}

void ableton_midi_event(u8 t, u8 ch, u8 p, u8 v) {
	if (ch == 0x0 || (ch == 0x5 && ableton_layout == 0x3)) {
		switch(t) {
			case 0x8: // Note off
				v = 0;
			
			case 0x9: // Note on
				ableton_led((ch == 0x0)? p : dr_xy[p], v);
				break;
				
			case 0xB:
				ableton_led(p, v);
				break;
		}
	}
}

/*         Setup mode         */
/*----------------------------*/

#define setup_tick 33
u8 setup_elapsed = setup_tick;
u8 setup_rainbow[48][3] = {{63, 0, 0}, {63, 7, 0}, {63, 15, 0}, {63, 23, 0}, {63, 31, 0}, {63, 39, 0}, {63, 47, 0}, {63, 55, 0}, {63, 63, 0}, {55, 63, 0}, {47, 63, 0}, {39, 63, 0}, {31, 63, 0}, {23, 63, 0}, {15, 63, 0}, {7, 63, 0}, {0, 63, 0}, {0, 63, 7}, {0, 63, 15}, {0, 63, 23}, {0, 63, 31}, {0, 63, 39}, {0, 63, 47}, {0, 63, 55}, {0, 63, 63}, {0, 55, 63}, {0, 47, 63}, {0, 39, 63}, {0, 31, 63}, {0, 23, 63}, {0, 15, 63}, {0, 7, 63}, {0, 0, 63}, {7, 0, 63}, {15, 0, 63}, {23, 0, 63}, {31, 0, 63}, {39, 0, 63}, {47, 0, 63}, {55, 0, 63}, {63, 0, 63}, {63, 0, 55}, {63, 0, 47}, {63, 0, 39}, {63, 0, 31}, {63, 0, 23}, {63, 0, 15}, {63, 0, 7}};
u8 setup_mode_counter = 0;
u8 setup_editor_counter = 0;

void setup_init() {
	hal_plot_led(TYPEPAD, 25, 11, 15, 15); // Select flash palette
	hal_plot_led(TYPEPAD, 26, 11, 15, 11); // Select Novation palette
	hal_plot_led(TYPEPAD, 27, 11, 15, 11); // Select mat1 palette
	hal_plot_led(TYPEPAD, 28, 11, 15, 11); // Select LP S palette
	
	if (palette_selected == 0) {
		hal_plot_led(TYPEPAD, 25, 47, 63, 63); // Flash palette selected
	} else {
		hal_plot_led(TYPEPAD, palette_selected + 25, 47, 63, 47); // Preset palette selected
		hal_plot_led(TYPEPAD, 15, 0, 7, 0); // Apply preset palette to flash palette
	}
	
	if (vel_sensitive) {
		hal_plot_led(TYPEPAD, 18, 31, 63, 63); // Velocity sensitivity enabled
	} else {
		hal_plot_led(TYPEPAD, 18, 7, 15, 15); // Velocity sensitivity disabled
	}
	
	hal_plot_led(TYPEPAD, 35, 15, 11, 15); // PRO Top Lights
	hal_plot_led(TYPEPAD, 36, 11, 7, 15); // MK2 Top Lights
	hal_plot_led(TYPEPAD, 37, 11, 7, 15); // MK2 Rotated Top Lights
	hal_plot_led(TYPEPAD, 38, 11, 7, 15); // MK2 Mirrored Top Lights
	
	if (top_lights_config == 0) {
		hal_plot_led(TYPEPAD, 35, 63, 47, 63); // PRO Top Lights selected
	} else {
		hal_plot_led(TYPEPAD, top_lights_config + 35, 47, 31, 63); // MK2 Top Lights selected
	}
	
	hal_plot_led(TYPEPAD, 81, 15, 0, 2); // Performance mode
	hal_plot_led(TYPEPAD, 82, 0, 15, 0); // Ableton mode
	
	if (ableton_enabled) {
		hal_plot_led(TYPEPAD, 82, 0, 63, 0); // Ableton mode selected
	} else {
		hal_plot_led(TYPEPAD, 81, 63, 0, 10); // Performance mode selected
	}
}

void setup_timer_event() {
	if (++setup_elapsed >= setup_tick) {
		hal_plot_led(TYPESETUP, 0, setup_rainbow[setup_mode_counter][0], setup_rainbow[setup_mode_counter][1], setup_rainbow[setup_mode_counter][2]); // Mode LED indicator animation
		setup_mode_counter++; setup_mode_counter %= 48;
		
		if (palette_selected == 0) {
			hal_plot_led(TYPEPAD, 15, setup_rainbow[setup_editor_counter][0], setup_rainbow[setup_editor_counter][1], setup_rainbow[setup_editor_counter][2]);  // Enter palette editor button animation
			setup_editor_counter++; setup_editor_counter %= 48;
		}
		
		setup_elapsed = 0;
	}
}

void setup_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (v != 0) {
		if (p == 0) { // Enter Performance/Ableton mode
			mode_update(ableton_enabled);
		
		} else if (25 <= p && p <= 28) { // Palette switch
			palette_selected = p - 25;
			dirty = 1;
			mode_refresh();
		
		} else if (p == 15) {
			if (palette_selected == 0) { // Enter Palette editor mode
				mode_update(2);
			
			} else { // Save current preset as custom palette
				memcpy(&palette[0][0][0], &palette[palette_selected][0][0], 384);
				palette_selected = 0;
				dirty = 1;
				mode_refresh();
			}
		
		} else if (p == 18) { // Toggle velocity sensitivity
			vel_sensitive = (vel_sensitive)? 0 : 1;
			dirty = 1;
			mode_refresh();
		
		} else if (35 <= p && p <= 38) { // Change Top Lights configuration
			top_lights_config = p - 35;
			dirty = 1;
			mode_refresh();
		
		} else if (p == 81 || p == 82) { // Switch Ableton mode and Performance mode
			ableton_enabled = (p - 81) * 3;
			mode_refresh();
		}
	}
}

void setup_midi_event(u8 t, u8 ch, u8 p, u8 v) {}

/*       Palette editor       */
/*----------------------------*/

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
	
	display_u6(palette[0][0][editor_selected], 1, 0, 63, 0, 0);
	display_u6(palette[0][1][editor_selected], 0, 0, 0, 63, 0);
	display_u6(palette[0][2][editor_selected], 1, 9, 0, 0, 63);
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
	}
}

void editor_select_v(u8 v) {
	if (v != 0) {
		editor_selected = v;
		editor_refresh();
	}
}

void editor_select_flip(u8 i) {
	u8 v = editor_selected ^ math_pow(2, i);
	
	if (v != 0) {
		editor_selected = v;
		if (i == 6) {
			editor_draw();
		} else {
			editor_refresh();
		}
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
			mode_update(1);
		
		} else if (2 <= x && x <= 7 && y == 0) { // Modify red bit
			palette[0][0][editor_selected] ^= math_pow(2, x - 2);
			editor_refresh();
			dirty = 1;
		
		} else if (2 <= p && p <= 7) { // Modify green bit
			palette[0][1][editor_selected] ^= math_pow(2, 7 - p);
			editor_refresh();
			dirty = 1;
		
		} else if (2 <= x && x <= 7 && y == 9) { // Modify blue bit
			palette[0][2][editor_selected] ^= math_pow(2, x - 2);
			editor_refresh();
			dirty = 1;
		
		} else if (92 <= p && p <= 98) { // Modify velocity bit
			editor_select_flip(98 - p);
		
		} else if (p != 1 && p != 8 && p != 10 && p != 19 && p != 80 && p != 89 && p != 91) { // Select velocity on grid
			editor_select_xy(p);
		}
	}
}

void editor_midi_event(u8 t, u8 ch, u8 p, u8 v) {}

/*    SysEx event handling    */
/*----------------------------*/

u8 syx_device_inquiry[] = {0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7};
u8 syx_device_inquiry_response[] = {0xF0, 0x7E,
                                    0x00,                                                      // Device ID
                                    0x06, 0x02, 0x00, 0x20, 0x29, 0x51, 0x00, 0x00, 0x00,      // Constant
                                    0x00, 0x01, 0x05, 0x04,                                    // Firmware rev. (4 bytes)
                                    0xF7};

u8 syx_mode_selection[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x21};
u8 syx_mode_selection_response[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x2D, 0xFF, 0xF7};

u8 syx_live_layout_selection[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x22};
u8 syx_live_layout_selection_response[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x2E, 0xFF, 0xF7};

void app_sysex_event(u8 port, u8 * d, u16 l) {
	// Device Inquiry - Read information about the connected device
	if (syx_cmp(d, syx_device_inquiry, l)) {
		hal_send_sysex(USBMIDI, &syx_device_inquiry_response[0], arr_size(syx_device_inquiry_response));
		return;
	}
	
	// Mode selection - return the status
	if (syx_cmp(d, syx_mode_selection, l - 2)) {
		syx_mode_selection_response[7] = *(d + 7);
		
		ableton_enabled = 3 * (1 - *(d + 7));
		mode_update(ableton_enabled);
		
		hal_send_sysex(USBMIDI, &syx_mode_selection_response[0], arr_size(syx_mode_selection_response));
		return;
	}
	
	// Live layout selection - return the status
	if (syx_cmp(d, syx_live_layout_selection, l - 2)) {
		syx_live_layout_selection_response[7] = *(d + 7);
		
		ableton_layout = *(d + 7);
		
		hal_send_sysex(USBMIDI, &syx_live_layout_selection_response[0], arr_size(syx_live_layout_selection_response));
		return;
	}
}

/*  Initialize the Launchpad  */
/*----------------------------*/

void app_init(const u16 *adc_raw) {
	flash_read();
	
	mode_init[255] = boot_init;
	mode_timer_event[255] = boot_timer_event;
	mode_surface_event[255] = boot_surface_event;
	mode_midi_event[255] = boot_midi_event;
	
	mode_init[0] = performance_init;
	mode_timer_event[0] = performance_timer_event;
	mode_surface_event[0] = performance_surface_event;
	mode_midi_event[0] = performance_midi_event;
	
	mode_init[1] = setup_init;
	mode_timer_event[1] = setup_timer_event;
	mode_surface_event[1] = setup_surface_event;
	mode_midi_event[1] = setup_midi_event;
	
	mode_init[2] = editor_init;
	mode_timer_event[2] = editor_timer_event;
	mode_surface_event[2] = editor_surface_event;
	mode_midi_event[2] = editor_midi_event;
	
	mode_init[3] = ableton_init;
	mode_timer_event[3] = ableton_timer_event;
	mode_surface_event[3] = ableton_surface_event;
	mode_midi_event[3] = ableton_midi_event;
	
	mode_update(255);
}