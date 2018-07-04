/*----------------------------------------------------------------------------*/
/*  Performance-optimized Launchpad Pro Firmware modification by mat1jaczyyy  */
/*----------------------------------------------------------------------------*/

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
 
 ******************************************************************************/

/*  Includes and definitions  */
/*----------------------------*/

#include <string.h> // array operations
#include "app.h"    // HAL interface

#define modulo(x, y) ((x % y + y) % y)
#define arr_size(x) (sizeof(x) / sizeof((x)[0]))
#define syx_cmp(x, y, n) (arr_size(y) == n && memcmp(x, &y[0], n) == 0)

u8 math_pow(u8 x, u8 e) {
	u8 y = 1;
	for (u8 j = 0; j < e; j++) {
		y *= x;
	}
	return y;
}

u8 xy_dr[128] = {0, 116, 117, 118, 119, 120, 121, 122, 123, 0, 115, 36, 37, 38, 39, 68, 69, 70, 71, 107, 114, 40, 41, 42, 43, 72, 73, 74, 75, 106, 113, 44, 45, 46, 47, 76, 77, 78, 79, 105, 112, 48, 49, 50, 51, 80, 81, 82, 83, 104, 111, 52, 53, 54, 55, 84, 85, 86, 87, 103, 110, 56, 57, 58, 59, 88, 89, 90, 91, 102, 109, 60, 61, 62, 63, 92, 93, 94, 95, 101, 108, 64, 65, 66, 67, 96, 97, 98, 99, 100, 0, 28, 29, 30, 31, 32, 33, 34, 35, 27, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
u8 dr_xy[128] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 99, 91, 92, 93, 94, 95, 96, 97, 98, 11, 12, 13, 14, 21, 22, 23, 24, 31, 32, 33, 34, 41, 42, 43, 44, 51, 52, 53, 54, 61, 62, 63, 64, 71, 72, 73, 74, 81, 82, 83, 84, 15, 16, 17, 18, 25, 26, 27, 28, 35, 36, 37, 38, 45, 46, 47, 48, 55, 56, 57, 58, 65, 66, 67, 68, 75, 76, 77, 78, 85, 86, 87, 88, 89, 79, 69, 59, 49, 39, 29, 19, 80, 70, 60, 50, 40, 30, 20, 10, 1, 2, 3, 4, 5, 6, 7, 8, 0, 0, 0, 0};

/*----------------------------*/

u8 syx_device_inquiry[] = {0xF0, 0x7E, 0x7F, 0x06, 0x01, 0xF7};
u8 syx_device_inquiry_response[] = {0xF0, 0x7E,
                                    0x00,                                                      // Device ID
                                    0x06, 0x02, 0x00, 0x20, 0x29, 0x51, 0x00, 0x00, 0x00,      // Constant
                                    0x00, 0x01, 0x08, 0x02,                                    // Firmware rev. (4 bytes)
                                    0xF7};

u8 syx_challenge[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x40};
u8 syx_challenge_response[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x40, 0xFF, 0xFF, 0xF7};
u8 challenge_do = 0;
u16 challenge_counter = 0;

u8 syx_mode_selection[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x21};
u8 syx_mode_selection_response[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x2D, 0xFF, 0xF7};

u8 syx_live_layout_selection[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x22};
u8 syx_live_layout_selection_response[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x2E, 0xFF, 0xF7};

u8 syx_standalone_layout_selection[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x2C};
u8 syx_standalone_layout_selection_response[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x2F, 0xFF, 0xF7};

u8 syx_led_single[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x0A};
u8 syx_led_column[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x0C};
u8 syx_led_row[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x0D};
u8 syx_led_all[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x0E};

u8 syx_led_rgb[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x0B};
u8 syx_led_grid[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x0F};

u8 syx_text[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x14};
u8 syx_text_response[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x15, 0xF7};

u8 syx_palette_start[] = {0xF0, 0x52, 0x45, 0x54, 0x49, 0x4E, 0x41, 0x7B};
u8 syx_palette_write[] = {0xF0, 0x52, 0x45, 0x54, 0x49, 0x4E, 0x41, 0x3D};
u8 syx_palette_end[] = {0xF0, 0x52, 0x45, 0x54, 0x49, 0x4E, 0x41, 0x7D};

u8 syx_palette_text[] = {0xF0, 0x00, 0x20, 0x29, 0x02, 0x10, 0x14, 0x03, 0x01, 0x04, 0x44, 0x6f, 0x77, 0x6E, 0x6C, 0x6F, 0x61, 0x64, 0x69, 0x6E, 0x67, 0x20, 0x70, 0x61, 0x6C, 0x65, 0x74, 0x74, 0x65, 0x2E, 0x2E, 0x2E, 0xF7};

/*----------------------------*/

u8 palette[6][3][128] = {
	{ // User-defined flash-backed palette 1
		{0},
		{0},
		{0}
	},
	{ // User-defined flash-backed palette 2
		{0},
		{0},
		{0}
	},
	{ // User-defined flash-backed palette 3
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

u8 palette_selected = 1;
u8 vel_sensitive = 0;
u8 top_lights_config = 0; // 0 = PRO, 1 = MK2, 2 = MK2 Rotated, 3 = MK2 Mirrored

u8 flash[1024] = {0}; // [0, 287) = palette[0], [288, 575) = palette[1], [576, 863) = palette[2], {864} = palette_selected, {865} = vel_sensitive, {866} = top_lights_config
u8 dirty = 0;

/*----------------------------*/

u8 mode = 0;
u8 mode_default = 0;

// Arrays of function pointers. Used to programmatically call the correct mode's function
void (*mode_init[256])();
void (*mode_timer_event[256])();
void (*mode_surface_event[256])(u8 p, u8 v, u8 x, u8 y);
void (*mode_midi_event[256])(u8 port, u8 t, u8 ch, u8 p, u8 v);

/*----------------------------*/

#define boot_note_tick 72
u8 boot_note_elapsed = boot_note_tick;
u8 boot_notes[17][6] = {
	{5, 89, 78, 67, 56, 45},
	{5, 79, 68, 57, 46, 55},
	{4, 69, 58, 47, 36},
	{4, 59, 48, 37, 35},
	{4, 49, 38, 27, 65},
	{4, 39, 28, 26, 66},
	{3, 29, 18, 25},
	{3, 19, 17, 75},
	{2, 16, 76},
	{3, 8, 15, 77},
	{2, 7, 85},
	{2, 6, 86},
	{2, 5, 87},
	{2, 95, 88},
	{1, 96},
	{1, 97},
	{1, 98}
};
u8 boot_note_floor = 0;
u8 boot_note_ceil = 0;

#define boot_fade_tick 4
u8 boot_fade_elapsed = boot_fade_tick;
u8 boot_fade_counter[16] = {};
u8 boot_colors[252][3] = {{0, 0, 1}, {0, 0, 2}, {0, 0, 3}, {0, 0, 4}, {0, 0, 5}, {0, 0, 6}, {0, 0, 7}, {0, 0, 8}, {0, 0, 9}, {0, 0, 10}, {0, 0, 11}, {0, 0, 12}, {0, 0, 13}, {0, 0, 14}, {0, 0, 15}, {0, 0, 16}, {0, 0, 17}, {0, 0, 18}, {0, 0, 19}, {0, 0, 20}, {0, 0, 21}, {0, 0, 22}, {0, 0, 23}, {0, 0, 24}, {0, 0, 25}, {0, 0, 26}, {0, 0, 27}, {0, 0, 28}, {0, 0, 29}, {0, 0, 30}, {0, 0, 31}, {0, 0, 32}, {0, 0, 33}, {0, 0, 34}, {0, 0, 35}, {0, 0, 36}, {0, 0, 37}, {0, 0, 38}, {0, 0, 39}, {0, 0, 40}, {0, 0, 41}, {0, 0, 42}, {0, 0, 43}, {0, 0, 44}, {0, 0, 45}, {0, 0, 46}, {0, 0, 47}, {0, 0, 48}, {0, 0, 49}, {0, 0, 50}, {0, 0, 51}, {0, 0, 52}, {0, 0, 53}, {0, 0, 54}, {0, 0, 55}, {0, 0, 56}, {0, 0, 57}, {0, 0, 58}, {0, 0, 59}, {0, 0, 60}, {0, 0, 61}, {0, 0, 62}, {0, 0, 63}, {1, 0, 63}, {2, 0, 63}, {3, 0, 63}, {4, 0, 63}, {5, 0, 63}, {6, 0, 63}, {7, 0, 63}, {8, 0, 63}, {9, 0, 63}, {10, 0, 63}, {11, 0, 63}, {12, 0, 63}, {13, 0, 63}, {14, 0, 63}, {15, 0, 63}, {16, 0, 63}, {17, 0, 63}, {18, 0, 63}, {19, 0, 63}, {20, 0, 63}, {21, 0, 63}, {22, 0, 63}, {23, 0, 63}, {24, 0, 63}, {25, 0, 63}, {26, 0, 63}, {27, 0, 63}, {28, 0, 63}, {29, 0, 63}, {30, 0, 63}, {31, 0, 63}, {32, 0, 63}, {33, 0, 63}, {34, 0, 63}, {35, 0, 63}, {36, 0, 63}, {37, 0, 63}, {38, 0, 63}, {39, 0, 63}, {40, 0, 63}, {41, 0, 63}, {42, 0, 63}, {43, 0, 63}, {44, 0, 63}, {45, 0, 63}, {46, 0, 63}, {47, 0, 63}, {48, 0, 63}, {49, 0, 63}, {50, 0, 63}, {51, 0, 63}, {52, 0, 63}, {53, 0, 63}, {54, 0, 63}, {55, 0, 63}, {56, 0, 63}, {57, 0, 63}, {58, 0, 63}, {59, 0, 63}, {60, 0, 63}, {61, 0, 63}, {62, 0, 63}, {63, 0, 63}, {63, 0, 62}, {63, 0, 61}, {63, 0, 60}, {63, 0, 59}, {63, 0, 58}, {63, 0, 57}, {63, 0, 56}, {63, 0, 55}, {63, 0, 54}, {63, 0, 53}, {63, 0, 52}, {63, 0, 51}, {63, 0, 50}, {63, 0, 49}, {63, 0, 48}, {63, 0, 47}, {63, 0, 46}, {63, 0, 45}, {63, 0, 44}, {63, 0, 43}, {63, 0, 42}, {63, 0, 41}, {63, 0, 40}, {63, 0, 39}, {63, 0, 38}, {63, 0, 37}, {63, 0, 36}, {63, 0, 35}, {63, 0, 34}, {63, 0, 33}, {63, 0, 32}, {63, 0, 31}, {63, 0, 30}, {63, 0, 29}, {63, 0, 28}, {63, 0, 27}, {63, 0, 26}, {63, 0, 25}, {63, 0, 24}, {63, 0, 23}, {63, 0, 22}, {63, 0, 21}, {63, 0, 20}, {63, 0, 19}, {63, 0, 18}, {63, 0, 17}, {63, 0, 16}, {63, 0, 15}, {63, 0, 14}, {63, 0, 13}, {63, 0, 12}, {63, 0, 11}, {63, 0, 10}, {63, 0, 9}, {63, 0, 8}, {63, 0, 7}, {63, 0, 6}, {63, 0, 5}, {63, 0, 4}, {63, 0, 3}, {63, 0, 2}, {63, 0, 1}, {63, 0, 0}, {62, 0, 0}, {61, 0, 0}, {60, 0, 0}, {59, 0, 0}, {58, 0, 0}, {57, 0, 0}, {56, 0, 0}, {55, 0, 0}, {54, 0, 0}, {53, 0, 0}, {52, 0, 0}, {51, 0, 0}, {50, 0, 0}, {49, 0, 0}, {48, 0, 0}, {47, 0, 0}, {46, 0, 0}, {45, 0, 0}, {44, 0, 0}, {43, 0, 0}, {42, 0, 0}, {41, 0, 0}, {40, 0, 0}, {39, 0, 0}, {38, 0, 0}, {37, 0, 0}, {36, 0, 0}, {35, 0, 0}, {34, 0, 0}, {33, 0, 0}, {32, 0, 0}, {31, 0, 0}, {30, 0, 0}, {29, 0, 0}, {28, 0, 0}, {27, 0, 0}, {26, 0, 0}, {25, 0, 0}, {24, 0, 0}, {23, 0, 0}, {22, 0, 0}, {21, 0, 0}, {20, 0, 0}, {19, 0, 0}, {18, 0, 0}, {17, 0, 0}, {16, 0, 0}, {15, 0, 0}, {14, 0, 0}, {13, 0, 0}, {12, 0, 0}, {11, 0, 0}, {10, 0, 0}, {9, 0, 0}, {8, 0, 0}, {7, 0, 0}, {6, 0, 0}, {5, 0, 0}, {4, 0, 0}, {3, 0, 0}, {2, 0, 0}, {1, 0, 0}, {0, 0, 0}};

/*----------------------------*/

#define setup_tick 33
u8 setup_elapsed = setup_tick;
u8 setup_rainbow[48][3] = {{63, 0, 0}, {63, 7, 0}, {63, 15, 0}, {63, 23, 0}, {63, 31, 0}, {63, 39, 0}, {63, 47, 0}, {63, 55, 0}, {63, 63, 0}, {55, 63, 0}, {47, 63, 0}, {39, 63, 0}, {31, 63, 0}, {23, 63, 0}, {15, 63, 0}, {7, 63, 0}, {0, 63, 0}, {0, 63, 7}, {0, 63, 15}, {0, 63, 23}, {0, 63, 31}, {0, 63, 39}, {0, 63, 47}, {0, 63, 55}, {0, 63, 63}, {0, 55, 63}, {0, 47, 63}, {0, 39, 63}, {0, 31, 63}, {0, 23, 63}, {0, 15, 63}, {0, 7, 63}, {0, 0, 63}, {7, 0, 63}, {15, 0, 63}, {23, 0, 63}, {31, 0, 63}, {39, 0, 63}, {47, 0, 63}, {55, 0, 63}, {63, 0, 63}, {63, 0, 55}, {63, 0, 47}, {63, 0, 39}, {63, 0, 31}, {63, 0, 23}, {63, 0, 15}, {63, 0, 7}};
u8 setup_mode_counter = 0;
u8 setup_editor_counter = 0;
u8 setup_jump = 0;

u8 editor_selected = 1;

/*----------------------------*/

s8 note_octave = 3;
u8 note_octave_colors[10][3] = {{63, 0, 63}, {20, 0, 63}, {0, 0, 63}, {0, 0, 31}, {0, 0, 7}, {0, 0, 31}, {0, 0, 63}, {20, 0, 63}, {40, 0, 63}, {63, 0, 63}};

s8 note_transpose = 0;
u8 note_transpose_colors[13][3] = {{0, 7, 0}, {0, 21, 0}, {0, 31, 0}, {0, 42, 0}, {0, 52, 0}, {0, 63, 0}, {15, 63, 0}, {23, 63, 0}, {31, 63, 0}, {39, 63, 0}, {47, 63, 0}, {55, 63, 0}, {63, 63, 0}};

#define note_length 12
#define note_segment 5
u8 note_shift = 0;

u8 scales[32][13] = {
	{5, 0, 1, 5, 7, 10}, // In Sen
	{5, 0, 2, 5, 7, 9}, // Yo scale
	{5, 0, 1, 5, 6, 10}, // Iwato
	{8, 0, 2, 3, 5, 6, 8, 9, 11}, // Whole Half
	{8, 0, 2, 3, 5, 7, 8, 10, 11}, // BeBop Minor
	{6, 0, 2, 3, 4, 7, 9}, // Major blues
	{5, 0, 2, 3, 7, 9}, // Kumoi
	{8, 0, 2, 4, 5, 7, 8, 9, 11}, // BeBop Major
	{7, 0, 2, 4, 6, 7, 9, 11}, // Lydian
	{7, 0, 1, 3, 5, 6, 8, 10}, // Locrian
	{5, 0, 2, 4, 7, 9}, // Major Pentatonic
	{7, 0, 1, 4, 5, 7, 8, 10}, // Phyrigian Dominant
	{8, 0, 1, 3, 4, 6, 7, 9, 10}, // Half-Whole Diminished
	{8, 0, 2, 4, 5, 7, 9, 10, 11}, // Mixolydian BeBop
	{7, 0, 1, 3, 4, 6, 8, 10}, // Super Locrian
	{5, 0, 2, 3, 6, 7}, // Hirajoshi
	{6, 0, 3, 5, 6, 7, 10}, // Blues
	{5, 0, 3, 5, 7, 10}, // Minor Pentatonic
	{7, 0, 2, 3, 6, 7, 8, 11}, // Hungarian Minor
	{7, 0, 2, 3, 6, 7, 9, 10}, // Ukrainian Dorian
	{7, 0, 1, 4, 6, 7, 9, 11}, // Marva
	{7, 0, 1, 3, 5, 6, 7, 11}, // Todi
	{6, 0, 2, 4, 6, 8, 10}, // Whole Tone
	{12, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, // Chromatic
	{7, 0, 2, 3, 5, 7, 8, 10}, // Minor
	{7, 0, 2, 4, 5, 7, 9, 11}, // Major
	{7, 0, 2, 3, 5, 7, 9, 10}, // Dorian
	{7, 0, 1, 3, 5, 7, 8, 10}, // Phrygian
	{7, 0, 2, 4, 5, 7, 9, 10}, // Mixolydian
	{7, 0, 2, 3, 5, 7, 9, 11}, // Melodic Minor (ascending)
	{7, 0, 2, 3, 5, 7, 8, 11}, // Harmonic Minor
	{8, 0, 2, 3, 4, 5, 7, 9, 10} // BeBop Dorian
};
u8 scale_keys[12] = {51, 62, 52, 63, 53, 54, 65, 55, 66, 56, 67, 57};

u8 scale_enabled = 0;
u8 scale_segment = 7;
u8 scale_selected = 25;
u8 scale_root = 0;

/*----------------------------*/

u8 ableton_pads[100] = {};
u8 ableton_layout = 0x0;

/*----------------------------*/

u8 drum_offset = 9;
u8 drum_colors[9][3] = {{21, 0, 21}, {0, 0, 63}, {63, 15, 0}, {63, 63, 0}, {56, 20, 15}, {0, 41, 63}, {24, 63, 7}, {33, 18, 63}, {21, 0, 7}};
u8 drum_nav_pressed[4] = {};
u8 drum_align[17] = {1, 1, 1, 1, 5, 5, 5, 5, 9, 9, 9, 9, 13, 13, 13, 13, 13};

/*----------------------------*/

u8 faders[8] = {};
u8 fader_stops[8] = {0, 17, 34, 52, 70, 89, 108, 127};
u8 fader_levels[8] = {1, 17, 34, 52, 70, 89, 108, 127};

u16 fader_tick[8] = {};
u16 fader_elapsed[8] = {};
u8 fader_counter[8] = {};
u8 fader_final[8] = {};

s8 fader_change[8] = {};

/*----------------------------*/

u8 text_bitmap[96][6] = {
	{4, 0b00000000, 0b00000000, 0b00000000, 0b00000000}, // 32 = Space
	{1, 0b11111011}, // 33 = !
	{3, 0b11100000, 0b00000000, 0b11100000}, // 34 = "
	{5, 0b00101000, 0b11111110, 0b00101000, 0b11111110, 0b00101000}, // 35 = #
	{5, 0b00100100, 0b01010100, 0b11111110, 0b01010100, 0b01001000}, // 36 = $
	{5, 0b11000100, 0b11001000, 0b00010000, 0b00100110, 0b01000110}, // 37 = %
	{5, 0b01101100, 0b10010010, 0b10010010, 0b01101100, 0b00001010}, // 38 = &
	{2, 0b00100000, 0b11000000}, // 39 = '
	{2, 0b01111100, 0b10000010}, // 40 = (
	{2, 0b10000010, 0b01111100}, // 41 = )
	{5, 0b00101000, 0b00111000, 0b01111100, 0b00111000, 0b00101000}, // 42 = *
	{5, 0b00010000, 0b00010000, 0b01111100, 0b00010000, 0b00010000}, // 43 = +
	{4, 0b00000000, 0b00000101, 0b00000110, 0b00000000}, // 44 = ,
	{4, 0b00010000, 0b00010000, 0b00010000, 0b00010000}, // 45 = -
	{4, 0b00000000, 0b00000110, 0b00000110, 0b00000000}, // 46 = .
	{5, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01000000}, // 47 = /
	{5, 0b01111100, 0b10000010, 0b10000010, 0b10000010, 0b01111100}, // 48 = 0
	{3, 0b01000010, 0b11111110, 0b00000010}, // 49 = 1
	{5, 0b01001110, 0b10010010, 0b10010010, 0b10010010, 0b01100010}, // 50 = 2
	{5, 0b01000100, 0b10000010, 0b10010010, 0b10010010, 0b01101100}, // 51 = 3
	{5, 0b00011000, 0b00101000, 0b01001000, 0b11111110, 0b00001000}, // 52 = 4
	{5, 0b11100100, 0b10100010, 0b10100010, 0b10100010, 0b10011100}, // 53 = 5
	{5, 0b01111100, 0b10010010, 0b10010010, 0b10010010, 0b01001100}, // 54 = 6
	{5, 0b10000000, 0b10000110, 0b10011000, 0b10100000, 0b11000000}, // 55 = 7
	{5, 0b01101100, 0b10010010, 0b10010010, 0b10010010, 0b01101100}, // 56 = 8
	{5, 0b01100100, 0b10010010, 0b10010010, 0b10010010, 0b01111100}, // 57 = 9
	{4, 0b00000000, 0b00110110, 0b00110110, 0b00000000}, // 58 = :
	{4, 0b00000000, 0b00110101, 0b00110110, 0b00000000}, // 59 = ;
	{4, 0b00010000, 0b00101000, 0b01000100, 0b10000010}, // 60 = <
	{4, 0b00101000, 0b00101000, 0b00101000, 0b00101000}, // 61 = =
	{4, 0b10000010, 0b01000100, 0b00101000, 0b00010000}, // 62 = >
	{5, 0b01000000, 0b10000000, 0b10001010, 0b10010000, 0b01100000}, // 63 = ?
	{5, 0b01111100, 0b10000010, 0b10010010, 0b10101010, 0b01111000}, // 64 = @
	{5, 0b00000110, 0b00111000, 0b11001000, 0b00111000, 0b00000110}, // 65 = A
	{5, 0b11111110, 0b10010010, 0b10010010, 0b10010010, 0b01101100}, // 66 = B
	{5, 0b01111100, 0b10000010, 0b10000010, 0b10000010, 0b01000100}, // 67 = C
	{5, 0b11111110, 0b10000010, 0b10000010, 0b01000100, 0b00111000}, // 68 = D
	{5, 0b11111110, 0b10010010, 0b10010010, 0b10010010, 0b10000010}, // 69 = E
	{5, 0b11111110, 0b10010000, 0b10010000, 0b10010000, 0b10000000}, // 70 = F
	{5, 0b01111100, 0b10000010, 0b10000010, 0b10010010, 0b10011110}, // 71 = G
	{5, 0b11111110, 0b00010000, 0b00010000, 0b00010000, 0b11111110}, // 72 = H
	{3, 0b10000010, 0b11111110, 0b10000010}, // 73 = I
	{4, 0b00000100, 0b00000010, 0b10000010, 0b11111100}, // 74 = J
	{5, 0b11111110, 0b00010000, 0b00101000, 0b01000100, 0b10000010}, // 75 = K
	{5, 0b11111110, 0b00000010, 0b00000010, 0b00000010, 0b00000010}, // 76 = L
	{5, 0b11111110, 0b01000000, 0b00110000, 0b01000000, 0b11111110}, // 77 = M
	{5, 0b11111110, 0b01100000, 0b00010000, 0b00001100, 0b11111110}, // 78 = N
	{5, 0b01111100, 0b10000010, 0b10000010, 0b10000010, 0b01111100}, // 79 = O
	{5, 0b11111110, 0b10010000, 0b10010000, 0b10010000, 0b01100000}, // 80 = P
	{5, 0b01111100, 0b10000010, 0b10001010, 0b10000100, 0b01111010}, // 81 = Q
	{5, 0b11111110, 0b10010000, 0b10011000, 0b10010100, 0b01100010}, // 82 = R
	{5, 0b01100100, 0b10110010, 0b10010010, 0b10010010, 0b01001100}, // 83 = S
	{5, 0b10000000, 0b10000000, 0b11111110, 0b10000000, 0b10000000}, // 84 = T
	{5, 0b11111100, 0b00000010, 0b00000010, 0b00000010, 0b11111100}, // 85 = U
	{5, 0b11100000, 0b00011000, 0b00000110, 0b00011000, 0b11100000}, // 86 = V
	{5, 0b11110000, 0b00001110, 0b00110000, 0b00001110, 0b11110000}, // 87 = W
	{5, 0b11000110, 0b00101000, 0b00010000, 0b00101000, 0b11000110}, // 88 = X
	{5, 0b11000000, 0b00100000, 0b00011110, 0b00100000, 0b11000000}, // 89 = Y
	{5, 0b10000110, 0b10001010, 0b10010010, 0b10100010, 0b11000010}, // 90 = Z
	{3, 0b11111110, 0b10000010, 0b10000010}, // 91 = [
	{5, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100}, // 92 = Backslash
	{3, 0b10000010, 0b10000010, 0b11111110}, // 93 = ]
	{5, 0b00100000, 0b01000000, 0b10000000, 0b01000000, 0b00100000}, // 94 = ^
	{5, 0b00000010, 0b00000010, 0b00000010, 0b00000010, 0b00000010}, // 95 = _
	{2, 0b11000000, 0b00100000}, // 96 = `
	{5, 0b00000100, 0b00101010, 0b00101010, 0b00101010, 0b00011110}, // 97 = a
	{5, 0b11111110, 0b00100010, 0b00100010, 0b00100010, 0b00011100}, // 98 = b
	{5, 0b00011100, 0b00100010, 0b00100010, 0b00100010, 0b00100010}, // 99 = c
	{5, 0b00011100, 0b00100010, 0b00100010, 0b00100010, 0b11111110}, // 100 = d
	{5, 0b00011100, 0b00101010, 0b00101010, 0b00101010, 0b00011010}, // 101 = e
	{4, 0b00010000, 0b01111110, 0b10010000, 0b10010000}, // 102 = f
	{5, 0b00011000, 0b00100101, 0b00100101, 0b00100101, 0b00111110}, // 103 = g
	{5, 0b11111110, 0b00100000, 0b00100000, 0b00100000, 0b00011110}, // 104 = h
	{2, 0b10111100, 0b00000010}, // 105 = i
	{3, 0b00000010, 0b00000001, 0b10111110}, // 106 = j
	{4, 0b11111110, 0b00001000, 0b00010100, 0b00100010}, // 107 = k
	{2, 0b11111100, 0b00000010}, // 108 = l
	{5, 0b00111110, 0b00100000, 0b00011110, 0b00100000, 0b00011110}, // 109 = m
	{5, 0b00111110, 0b00100000, 0b00100000, 0b00100000, 0b00011110}, // 110 = n
	{5, 0b00011100, 0b00100010, 0b00100010, 0b00100010, 0b00011100}, // 111 = o
	{5, 0b00111111, 0b00100100, 0b00100100, 0b00100100, 0b00011000}, // 112 = p
	{5, 0b00011100, 0b00100010, 0b00100010, 0b00100100, 0b00111111}, // 113 = q
	{4, 0b00111110, 0b00010000, 0b00100000, 0b00100000}, // 114 = r
	{5, 0b00010010, 0b00101010, 0b00101010, 0b00101010, 0b00100100}, // 115 = s
	{3, 0b00100000, 0b01111100, 0b00100010}, // 116 = t
	{5, 0b00111100, 0b00000010, 0b00000010, 0b00000100, 0b00111110}, // 117 = u
	{5, 0b00110000, 0b00001100, 0b00000010, 0b00001100, 0b00110000}, // 118 = v
	{5, 0b00111000, 0b00000110, 0b00111000, 0b00000110, 0b00111000}, // 119 = w
	{5, 0b00100010, 0b00010100, 0b00001000, 0b00010100, 0b00100010}, // 120 = x
	{5, 0b00111000, 0b00000101, 0b00000101, 0b00000101, 0b00111110}, // 121 = y
	{5, 0b00100010, 0b00100110, 0b00101010, 0b00110010, 0b00100010}, // 122 = z
	{3, 0b00010000, 0b01101100, 0b10000010}, // 123 = {
	{1, 0b11111111}, // 124 = |
	{3, 0b10000010, 0b01101100, 0b00010000}, // 125 = }
	{4, 0b01000000, 0b10000000, 0b01000000, 0b10000000, }, // 126 = ~
	{5, 0b00111000, 0b00101000, 0b01101100, 0b00101000, 0b00010000} // 127 = DEL
};

u8 text_port = 0;
u8 text_color = 127;
u8 text_loop = 0;
u8 text_bytes[323] = {};

u16 text_tick = 80;
u16 text_elapsed = 80;
u16 text_counter = 1;
u8 text_subcounter = 0;

u8 text_frame[10] = {};
u8 text_frame_empty[10] = {};
u8 text_done = 0;

u8 text_palette = 0;

/*      Challenge solver      */
/*----------------------------*/

void challenge_timer_event() {
	for (u8 i = 0; i < 9; i++) {
		syx_challenge_response[7] = challenge_counter >> 7;
		syx_challenge_response[8] = challenge_counter & 127;
		
		hal_send_sysex(USBMIDI, &syx_challenge_response[0], arr_size(syx_challenge_response));
		
		if (++challenge_counter >= 16384) {
			challenge_do = 0;
			return;
		}
	}
}

/*       LED management       */
/*----------------------------*/

void clear_led() {
	for (u8 i = 1; i < 99; i++) {
		hal_plot_led(TYPEPAD, i, 0, 0, 0);
	}
	hal_plot_led(TYPESETUP, 0, 0, 0, 0);
}

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
		hal_plot_led(TYPEPAD, (d == 0)? (8 - i + x * 10) : ((i + 1) * 10 + x), r * w, g * w, b * w);
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

void flash_read() {
	hal_read_flash(0, &flash[0], 1024);
	u16 fp = 0;
	
	for (u8 p = 0; p < 3; p++) {
		for (u8 i = 0; i < 3; i++) {
			for (u8 j = 0; j < 128; j += 4) {
				palette[p][i][j] = flash[fp] >> 2;
				palette[p][i][j + 1] = ((flash[fp] << 4) + (flash[fp + 1] >> 4)) & 63;
				palette[p][i][j + 2] = ((flash[fp + 1] << 2) + (flash[fp + 2] >> 6)) & 63;
				palette[p][i][j + 3] = flash[fp + 2] & 63;
				
				fp += 3;
			}
			
			palette[p][i][0] = 0;
		}
	}
	
	// Doesn't need to check palette for 0xFF
	
	palette_selected = flash[fp++];
	if (palette_selected >> 2) palette_selected = 1;
	
	vel_sensitive = flash[fp++];
	if (vel_sensitive >> 1) vel_sensitive = 0;
	
	top_lights_config = flash[fp++];
	if (top_lights_config >> 2) top_lights_config = 0;
}

void flash_write() {
	if (dirty) {
		hal_plot_led(TYPESETUP, 0, 3, 0, 0); // Writing indicator
		u16 fp = 0;
		
		// Compress 4 color values into 3 bytes. 6 bits * 4 values = 24 bits = 3 bytes
		for (u8 p = 0; p < 3; p++) {
			for (u8 i = 0; i < 3; i++) {
				for (u8 j = 0; j < 128; j += 4) {
					flash[fp++] = (palette[p][i][j] << 2) + (palette[p][i][j + 1] >> 4);
					flash[fp++] = (palette[p][i][j + 1] << 4) + (palette[p][i][j + 2] >> 2);
					flash[fp++] = (palette[p][i][j + 2] << 6) + palette[p][i][j + 3];
				}
			}
		}
		
		flash[fp++] = palette_selected;
		flash[fp++] = vel_sensitive;
		flash[fp++] = top_lights_config;
		
		hal_write_flash(0, &flash[0], 1024);
		
		hal_plot_led(TYPESETUP, 0, 0, 0, 0);
		dirty = 0;
	}
}

/*  Modes and event handling  */
/*----------------------------*/

void mode_refresh() {
	clear_led();
	(*mode_init[mode])();
}

void mode_update(u8 x) {
	clear_led();
	flash_write();
	
	mode = x;
	(*mode_init[mode])();
}

void app_timer_event() {
	if (challenge_do) challenge_timer_event();
	(*mode_timer_event[mode])();
}

void app_surface_event(u8 t, u8 p, u8 v) {
	if (!vel_sensitive) {
		v = (v == 0)? 0 : 127;
	}

	(*mode_surface_event[mode])(p, v, p / 10, p % 10);
}

void app_midi_event(u8 port, u8 t, u8 p, u8 v) {
	u8 ch = t % 16;
	t >>= 4;
	
	if (mode != 1 && mode_default == 1) {
		(*mode_midi_event[mode_default])(port, t, ch, p, v);
	}
	
	(*mode_midi_event[mode])(port, t, ch, p, v);
}

void app_aftertouch_event(u8 p, u8 v) {} // Unused
void app_cable_event(u8 t, u8 v) {} // Unused

/*       Boot animation       */
/*----------------------------*/

void boot_init() {}

void boot_timer_event() {
	if (++boot_note_elapsed >= boot_note_tick) { // Start fading next note
		if (boot_note_ceil < 17) boot_note_ceil++;
		
		boot_note_elapsed = 0;
	}
	
	if (++boot_fade_elapsed >= boot_fade_tick) { // Redraw fades
		if (boot_note_floor == 17) { // Enter Performance mode (end condition)
			mode_update(0);
		
		} else {
			for (u8 i = boot_note_floor; i < boot_note_ceil; i++) {
				if (boot_fade_counter[i] < 252) { // Draw next fade for note
					for (u8 j = 1; j <= boot_notes[i][0]; j++) { // For each note in frame
						if (boot_notes[i][j] != 98 || boot_fade_counter[i] <= 188) { // Check for leaving User LED on
							hal_plot_led(TYPEPAD, boot_notes[i][j], boot_colors[boot_fade_counter[i]][0], boot_colors[boot_fade_counter[i]][1], boot_colors[boot_fade_counter[i]][2]);
						}
						
						hal_plot_led(TYPEPAD, 99 - boot_notes[i][j], boot_colors[boot_fade_counter[i]][0], boot_colors[boot_fade_counter[i]][1], boot_colors[boot_fade_counter[i]][2]); // 180 degree rotation
						
						if (boot_notes[i][j] == 4 || 99 - boot_notes[i][j] == 4) { // Mode LED
							hal_plot_led(TYPESETUP, 0, boot_colors[boot_fade_counter[i]][0], boot_colors[boot_fade_counter[i]][1], boot_colors[boot_fade_counter[i]][2]);
						}
					}
					boot_fade_counter[i]++;
					
				} else { // Stop fading a finished note
					boot_note_floor++;
				}
			}
		}
		
		boot_fade_elapsed = 0;
	}
}

void boot_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0 && v != 0) mode_update(0); // Manually interrupt boot animation
}

void boot_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {}

/*         Setup mode         */
/*----------------------------*/

void setup_init() {
	for (u8 i = 16; i < 19; i++) {
		hal_plot_led(TYPEPAD, i, 11, 15, 11); // Select preset palette
	}
	
	for (u8 i = 26; i < 29; i++) {
		hal_plot_led(TYPEPAD, i, 11, 15, 15); // Select flash (custom) palette
	}
	
	if (palette_selected < 3) {
		hal_plot_led(TYPEPAD, palette_selected + 26, 47, 63, 63); // Flash palette selected
	} else {
		hal_plot_led(TYPEPAD, palette_selected + 13, 47, 63, 47); // Preset palette selected
	}
	
	if (vel_sensitive) {
		hal_plot_led(TYPEPAD, 11, 31, 63, 63); // Velocity sensitivity enabled
	} else {
		hal_plot_led(TYPEPAD, 11, 7, 15, 15); // Velocity sensitivity disabled
	}
	
	hal_plot_led(TYPEPAD, 85, 15, 11, 15); // PRO Top Lights
	for (u8 i = 86; i < 89; i++) {
		hal_plot_led(TYPEPAD, i, 11, 7, 15); // MK2 Top Lights
	}
	
	if (!top_lights_config) {
		hal_plot_led(TYPEPAD, 85, 63, 47, 63); // PRO Top Lights selected
	} else {
		hal_plot_led(TYPEPAD, top_lights_config + 85, 47, 31, 63); // MK2 Top Lights selected
	}
	
	hal_plot_led(TYPEPAD, 81, 15, 0, 0); // Performance mode
	hal_plot_led(TYPEPAD, 82, 0, 15, 0); // Ableton mode
	hal_plot_led(TYPEPAD, 83, 0, 15, 15); // Note mode
	hal_plot_led(TYPEPAD, 71, 15, 15, 0); // Drum mode
	hal_plot_led(TYPEPAD, 72, 15, 0, 15); // Fader mode
	hal_plot_led(TYPEPAD, 73, 15, 5, 0); // Programmer mode
	
	switch (mode_default) {
		case 0:
			hal_plot_led(TYPEPAD, 81, 63, 0, 0); // Performance mode selected
			break;
		
		case 1:
			hal_plot_led(TYPEPAD, 82, 0, 63, 0); // Ableton mode selected
			break;
		
		case 2:
			hal_plot_led(TYPEPAD, 83, 0, 63, 63); // Note mode selected
			break;
		
		case 3:
			hal_plot_led(TYPEPAD, 71, 63, 63, 0); // Drum mode selected
			break;
		
		case 4:
			hal_plot_led(TYPEPAD, 72, 63, 0, 63); // Fader mode selected
			break;
		
		case 5:
			hal_plot_led(TYPEPAD, 73, 63, 18, 0); // Programmer mode selected
			break;
	}
	
	setup_elapsed = setup_tick;
}

void setup_timer_event() {
	if (++setup_elapsed >= setup_tick) {
		hal_plot_led(TYPESETUP, 0, setup_rainbow[setup_mode_counter][0], setup_rainbow[setup_mode_counter][1], setup_rainbow[setup_mode_counter][2]); // Mode LED indicator animation
		setup_mode_counter++; setup_mode_counter %= 48;
		
		if (palette_selected < 3) {
			hal_plot_led(TYPEPAD, 25, setup_rainbow[setup_editor_counter][0], setup_rainbow[setup_editor_counter][1], setup_rainbow[setup_editor_counter][2]);  // Enter palette editor button animation
			setup_editor_counter++; setup_editor_counter %= 48;
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
		
		} else if (1 <= x && x <= 2 && 6 <= y && y <= 8) { // Palette switch
			palette_selected = (2 - x) * 3 + y - 6;
			dirty = 1;
			mode_refresh();
		
		} else if (p == 25) {
			if (palette_selected < 3) { // Enter Palette editor mode
				mode_update(253);
				setup_jump = 0;
			}
		
		} else if (p == 11) { // Toggle velocity sensitivity
			vel_sensitive = (vel_sensitive)? 0 : 1;
			dirty = 1;
			mode_refresh();
		
		} else if (85 <= p && p <= 88) { // Change Top Lights configuration
			top_lights_config = p - 85;
			dirty = 1;
			mode_refresh();
		
		} else if (81 <= p && p <= 83) { // Switch default mode
			mode_default = p - 81;
			mode_refresh();
		
		} else if (71 <= p && p <= 73) {
			mode_default = p - 68;
			mode_refresh();
		}
	
	} else { // Note released
		if (p == 0 && setup_jump) { // Quickly jump back to main mode
			mode_update(mode_default);
			setup_jump = 0;
		}
	}
}

void setup_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {}

/*       Palette editor       */
/*----------------------------*/

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
	u8 v = editor_selected ^ math_pow(2, i);
	
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
			mode_update(254);
		
		} else if (2 <= x && x <= 7 && y == 0) { // Modify red bit
			palette[palette_selected][0][editor_selected] ^= math_pow(2, x - 2);
			editor_refresh();
			dirty = 1;
		
		} else if (2 <= p && p <= 7) { // Modify green bit
			palette[palette_selected][1][editor_selected] ^= math_pow(2, 7 - p);
			editor_refresh();
			dirty = 1;
		
		} else if (2 <= x && x <= 7 && y == 9) { // Modify blue bit
			palette[palette_selected][2][editor_selected] ^= math_pow(2, x - 2);
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

/*      Performance mode      */
/*----------------------------*/

void performance_init() {
	hal_plot_led(TYPEPAD, 98, 63, 0, 0); // User LED
}

void performance_timer_event() {}

void performance_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(254);
				
	} else { // Send MIDI input to DAW
		hal_send_midi(USBSTANDALONE, (v == 0)? 0x85 : 0x95, xy_dr[p], v);
	}
}

void performance_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
	if (port == USBSTANDALONE && ch == 0x5) {
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

/*    Note and Scale modes    */
/*----------------------------*/

void note_single(u8 *p, u8 l, u8 r, u8 g, u8 b) {
	for (u8 i = 0; i < l; i++) { // Used to update LEDs on buttons that trigger the same actual note
		hal_plot_led(TYPEPAD, *(p+i), r, g, b);
	}
}

s8 note_press(u8 x, u8 y, u8 v) {
	u8 length = (scale_enabled)? scales[scale_selected][0] : note_length;
	u8 segment = (scale_enabled)? scale_segment : note_segment;
	
	u8 offset = (x - 1) * segment + (y - 1); // Note pressed in relation to lowest
	u8 up = offset / length; // Octaves above lowest
	
	offset %= length; // Note pressed in relation to its octave
	if (scale_enabled) offset = scales[scale_selected][1 + offset]; // Translate for Scale mode
	
	s8 c = (note_octave + up) * 12 + offset; // Note in relation to C
	s8 n = c + note_transpose + ((scale_enabled)? scale_root : 0); // Actual note (transposition applied)
	
	u8 p[8] = {x * 10 + y}; // Pitches to update on the Launchpad
	u8 l = 1;
	
	for (u8 i = 0; i < 7; i++) { // Add pitches above
		s8 e = x + i;
		s8 f = y - segment * i;
		
		if (1 <= e && e <= 8 && 1 <= f && f <= 8) {
			p[l++] = e * 10 + f; // Append to array
		} else {
			break;
		}
	}
	
	for (u8 i = 0; i < 7; i++) { // Add pitches below
		s8 e = x - i;
		s8 f = y + segment * i;
		
		if (1 <= e && e <= 8 && 1 <= f && f <= 8) {
			p[l++] = e * 10 + f;
		} else {
			break;
		}
	}
	
	if (n < 0) { // Invalid note - also affects notes larger than 127 due to overflow!
		note_single(&p[0], l, 7, 0, 0);
		
	} else { // Valid note
		if (v == 0) { // Note released
			u8 m = modulo(c, 12); // Note without octave
			
			if (scale_enabled) {
				note_single(&p[0], l, (m == 0) ? 20 : 63, 0, 63);

			} else {
				switch (m) {
					case 0: // C base note
						note_single(&p[0], l, 63, 0, 63);
						break;
					
					case 2:
					case 4:
					case 5:
					case 7:
					case 9:
					case 11: // White note
						note_single(&p[0], l, 0, 41, 63);
						break;
					
					default: // Black note
						note_single(&p[0], l, 0, 0, 0);
						break;	
				}
				
				u8 b = modulo(note_transpose, length); // Base note of scale
				if (b != 0 && b == m) note_single(&p[0], l, 20, 0, 63); // Scale base note
			}
			
		} else { // Note pressed
			note_single(&p[0], l, 0, 63, 0);
		}
	}
	
	return n;
}

void note_scale_button() {
	if (note_shift) { // Shift button pressed
		hal_plot_led(TYPEPAD, 80, 63, 63, 63);
		hal_plot_led(TYPEPAD, 96, 7, 7, 7);
	
	} else { // Shift button released
		hal_plot_led(TYPEPAD, 80, 7, 7, 7);
		if (mode_default == 1) {
			hal_plot_led(TYPEPAD, 96, 0, 63, 63);
		} else if (mode_default == 2) {
			hal_plot_led(TYPEPAD, 96, 0, 0, 0);
		}
	}
}

void note_draw() {
	for (u8 x = 1; x < 9; x++) { // Regular notes
		for (u8 y = 1; y < 9; y++) {
			note_press(x, y, 0);
		}
	}
	
	for (u8 i = 0; i < 128; i++) { // Turn off all notes
		hal_send_midi(2 - mode_default, 0x80, i, 0);
	}
	
	u8 o = note_octave + 1; // Octave navigation
	if (o < 5) {
		hal_plot_led(TYPEPAD, 91, note_octave_colors[4][0], note_octave_colors[4][1], note_octave_colors[4][2]);
		hal_plot_led(TYPEPAD, 92, note_octave_colors[o][0], note_octave_colors[o][1], note_octave_colors[o][2]);
	} else {
		hal_plot_led(TYPEPAD, 91, note_octave_colors[o][0], note_octave_colors[o][1], note_octave_colors[o][2]);
		hal_plot_led(TYPEPAD, 92, note_octave_colors[4][0], note_octave_colors[4][1], note_octave_colors[4][2]);
	}
	
	if (note_transpose > 0) { // Transpose navigation
		hal_plot_led(TYPEPAD, 93, note_transpose_colors[0][0], note_transpose_colors[0][1], note_transpose_colors[0][2]);
		hal_plot_led(TYPEPAD, 94, note_transpose_colors[note_transpose][0], note_transpose_colors[note_transpose][1], note_transpose_colors[note_transpose][2]);
	} else {
		hal_plot_led(TYPEPAD, 93, note_transpose_colors[-note_transpose][0], note_transpose_colors[-note_transpose][1], note_transpose_colors[-note_transpose][2]);
		hal_plot_led(TYPEPAD, 94, note_transpose_colors[0][0], note_transpose_colors[0][1], note_transpose_colors[0][2]);
	}
}

void note_init() {
	note_draw();
	note_scale_button();
	if (mode == 2) hal_plot_led(TYPESETUP, 0, 0, 63, 63); // Note mode LED
}

void note_timer_event() {}

void note_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(254);
	
	} else if (p == 96 && note_shift && v != 0) { // Shift+Note button
		mode_update(252); // Enter Setup for Scale mode
		hal_plot_led(TYPEPAD, p, 63, 63, 63);
	
	} else if (p == 96 && v == 0) {
		note_scale_button();
	
	} else if (x == 0 || y == 9 || (y == 0 && x < 8) || (x == 9 && y > 4)) { // Unused side buttons
		hal_send_midi(2 - mode_default, 0xB0, p, v);
		hal_plot_led(TYPEPAD, p, 0, (v == 0)? 0 : 63, 0);
	
	} else if (p == 80) { // Shift button
		hal_send_midi(2 - mode_default, 0xB0, p, v);
		note_shift = v;
		note_scale_button();
	
	} else if (x == 9 && y < 5) { // Navigation buttons
		if (v != 0) {
			switch (p) {
				case 91: // Octave up
					if (note_octave < 8) note_octave++;
					break;
				
				case 92: // Octave down
					if (note_octave > -1) note_octave--;
					break;
				
				case 93: // Transpose down
					if (note_transpose > -12) note_transpose--;
					break;
				
				case 94: // Transpose up
					if (note_transpose < 12) note_transpose++;
					break;
			}
			note_draw();
		}
	
	} else { // Main grid
		s8 n = note_press(x, y, v);
		if (n >= 0) hal_send_midi(2 - mode_default, (v == 0)? 0x80 : 0x90, n, v);
	}
}

void note_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {}

void scale_setup_init() {
	hal_plot_led(TYPESETUP, 0, 0, 63, 63); // Note mode LED
	
	if (scale_enabled) {
		hal_plot_led(TYPEPAD, 88, 20, 63, 0); // Scale mode enabled
	} else {
		hal_plot_led(TYPEPAD, 88, 4, 10, 0); // Scale mode disabled
	}
	
	for (u8 i = 81; i < 88; i++) {
		hal_plot_led(TYPEPAD, i, 21, 7, 0); // Segment length options
	}
	hal_plot_led(TYPEPAD, 88 - scale_segment, 63, 20, 0); // Selected segment
	
	for (u8 i = 0; i < 12; i++) {
		hal_plot_led(TYPEPAD, scale_keys[i], 0, 7, 7); // Root note selector and scale preview
	}	
	for (u8 i = 2; i <= scales[scale_selected][0]; i++) {
		hal_plot_led(TYPEPAD, scale_keys[modulo(scales[scale_selected][i] + scale_root, 12)], 0, 63, 63); // Notes in current scale
	}
	hal_plot_led(TYPEPAD, scale_keys[scale_root], 0, 0, 63); // Scale root note
	
	for (u8 x = 1; x < 5; x++) {
		for (u8 y = 1; y < 9; y++) {
			hal_plot_led(TYPEPAD, x * 10 + y, 63, 0, 63); // Select scale
		}
	}
	hal_plot_led(TYPEPAD, ((scale_selected >> 3) + 1) * 10 + (scale_selected % 8) + 1, 20, 0, 63); // Selected scale
}

void scale_setup_timer_event() {}

void scale_setup_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(254);
	
	} else if (p == 96) {
		if (v == 0) {
			hal_plot_led(TYPEPAD, 96, 0, 7, 7);
		} else {
			hal_plot_led(TYPEPAD, 96, 0, 63, 0);
			mode_update(mode_default); // Enter Note/Live mode
		}
	
	} else if (p == 80) { // Shift button
		hal_send_midi(2 - mode_default, 0xB0, p, v);
		note_shift = v;
	
	} else if (v != 0) {
		if (p == 88) { // Enable or disable Scale mode
			scale_enabled = 1 - scale_enabled;
			scale_setup_init();
		
		} else if (81 <= p && p <= 87) { // Select segment length
			scale_segment = 88 - p;
			scale_setup_init();
		
		} else if (1 <= x && x <= 4 && 1 <= y && y <= 8) { // Select scale
			scale_selected = (x - 1) * 8 + (y - 1);
			scale_setup_init();
		
		} else {
			u8 i = 0;
			do {
				if (scale_keys[i] == p) break;
			} while (++i < 12);
			
			if (i < 12) {
				scale_root = i;
				scale_setup_init();
			}
		}
	}
}

void scale_setup_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {}

/*        Ableton mode        */
/*----------------------------*/

void ableton_led(u8 p, u8 v) {
	if (mode == 1) hal_plot_led(TYPEPAD, p, palette[3][0][v], palette[3][1][v], palette[3][2][v]);
	ableton_pads[p] = v;
}

void ableton_init() {
	hal_plot_led(TYPESETUP, 0, 0, 63, 0); // Live mode LED
	
	for (u8 i = 0; i < 100; i++) {
		hal_plot_led(TYPEPAD, i, palette[3][0][ableton_pads[i]], palette[3][1][ableton_pads[i]], palette[3][2][ableton_pads[i]]);
	}
	
	if (ableton_layout == 0x2) note_init();
	if (ableton_layout == 0x3) hal_plot_led(TYPEPAD, 98, 20, 0, 63); // User LED
}

void ableton_timer_event() {}

void ableton_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(254);	
	} else {
		switch (ableton_layout) {
			case 0x0: // Session mode
			case 0x6: // Session mode - Record Arm
			case 0x7: // Session mode - Track Select
			case 0x8: // Session mode - Mute
			case 0x9: // Session mode - Solo
			case 0xD: // Session mode - Stop Clip
				if (x == 0 || x == 9 || y == 0 || y == 9) {
					hal_send_midi(USBMIDI, 0xB0, p, v);
				} else {
					hal_send_midi(USBMIDI, (v == 0)? 0x80 : 0x90, p, v);
				}
				break;
			
			case 0x1: // Note mode - Drum Rack layout
				if (x == 0 || x == 9 || y == 0 || y == 9) {
					hal_send_midi(USBMIDI, 0xB0, p, v);
				} else {
					hal_send_midi(USBMIDI, (v == 0)? 0x80 : 0x90, xy_dr[p], v);
				}
				break;
			
			case 0x2: // Note mode - Chromatic layout
				if (x == 0 || (x == 9 && (y == 5 || y > 6)) || (y == 0 && x < 8) || y == 9) {
					hal_send_midi(USBMIDI, 0xB0, p, v);
				} else {
					note_surface_event(p, v, x, y);
				}
				break;
			
			case 0x4: // Note mode - Blank layout (for Audio track)
				if (x == 0 || x == 9 || y == 0 || y == 9) {
					hal_send_midi(USBMIDI, 0xB0, p, v);
				}
				break;
			
			case 0x5: // Fader - Device mode
			case 0xA: // Fader - Volume
			case 0xB: // Fader - Pan
			case 0xC: // Fader - Sends
				if (x == 0 || x == 9 || y == 0 || y == 9) {
					hal_send_midi(USBMIDI, 0xB0, p, v);
				} else {
					// TODO: Implement true Device mode
				}
				break;
			
			case 0x3: // User mode
				if (x == 9) {
					hal_send_midi(USBMIDI, 0xB5, p, v);
				} else {
					hal_send_midi(USBMIDI, (v == 0)? 0x85 : 0x95, xy_dr[p], v);
				}
				break;
		}
	}
}

void ableton_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
	if (port == USBMIDI) {
		u8 x = p / 10;
		u8 y = p % 10;
		
		if (ableton_layout == 0x2 && !(x == 0 || (x == 9 && (y == 5 || y > 6)) || (y == 0 && x < 8) || y == 9)) {
			note_midi_event(port, t, ch, p, v);
			return;
		}
		
		switch (t) {
			case 0x8:
				v = 0;
			
			case 0x9:
				if (ableton_layout == 0x1 && ch == 0x0 && 36 <= p && p <= 99) {
					ableton_led(dr_xy[p], v);
					break;
				}
				if (ableton_layout == 0x3 && ch == 0x5) {
					ableton_led(dr_xy[p], v);
					break;
				}
				if (ableton_layout == 0x4 && 1 <= x && x <= 8 && 1 <= y && y <= 8) {
					break;
				}
							
			case 0xB:
				if (ableton_layout == 0x3) {
					if (ch == 0x5) ableton_led(p, v);
				} else {
					if (ch == 0x0) ableton_led(p, v);
				}
				break;
		}
	}
}

/*    Standalone Drum mode    */
/*----------------------------*/

s8 drum_press(u8 x, u8 y, u8 v) {
	u8 n = (drum_offset + x - 1) * 4 + (y - 1) % 4;
	if (y >= 5) n += 32;
	
	u8 p = x * 10 + y;
	
	if (v == 0) { // Note released
		u8 i = (n + 12) / 16;
		hal_plot_led(TYPEPAD, p, drum_colors[i][0], drum_colors[i][1], drum_colors[i][2]);
	
	} else { // Note pressed
		hal_plot_led(TYPEPAD, p, 0, 63, 0);
	}
	
	return n;
}

void drum_init() {
	for (u8 x = 1; x < 9; x++) { // Regular notes
		for (u8 y = 1; y < 9; y++) {
			drum_press(x, y, 0);
		}
	}
	
	for (u8 i = 0; i < 128; i++) { // Turn off all notes
		hal_send_midi(USBSTANDALONE, 0x81, i, 0);
	}
	
	hal_plot_led(TYPEPAD, 91, 0, 0, (drum_offset < 13)? (drum_nav_pressed[0]? 63 : 31) : 0); // Navigation buttons
	hal_plot_led(TYPEPAD, 92, 0, 0, (drum_offset > 3)? (drum_nav_pressed[1]? 63 : 31) : 0);
	hal_plot_led(TYPEPAD, 93, 0, 0, (drum_offset < 16)? (drum_nav_pressed[2]? 63 : 31) : 0);
	hal_plot_led(TYPEPAD, 94, 0, 0, (drum_offset > 0)? (drum_nav_pressed[3]? 63 : 31) : 0);
	
	hal_plot_led(TYPESETUP, 0, 63, 63, 0); // Drum mode LED
}

void drum_timer_event() {}

void drum_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(254);
	
	} else if (x == 0 || (x == 9 && y >= 5) || y == 0 || y == 9) { // Unused side buttons
		hal_send_midi(USBSTANDALONE, 0xB1, p, v);
		hal_plot_led(TYPEPAD, p, 0, (v == 0)? 0 : 63, 0);
	
	} else if (x == 9 && y <= 4) { // Navigation buttons
		if (v != 0) {
			switch (p) {
				case 91:
					drum_offset += 4 * (drum_offset < 13);
					break;
				
				case 92:
					drum_offset -= 4 * (drum_offset > 3);
					break;
				
				case 93:
					drum_offset += drum_offset < 16;
					break;
				
				case 94:
					drum_offset -= drum_offset > 0;
					break;
			}
		}
		drum_nav_pressed[p - 91] = v;
		
		if (drum_nav_pressed[0] && drum_nav_pressed[1]) { // Reset offset. Note: Undocumented in Programmer's reference
			drum_offset = 9;
		} else if (drum_nav_pressed[2] && drum_nav_pressed[3]) { // Align offset
			drum_offset = drum_align[drum_offset];
		}
		drum_init(); // Redraw grid
	
	} else { // Main grid
		hal_send_midi(USBSTANDALONE, (v == 0)? 0x81 : 0x91, drum_press(x, y, v), v);
	}
}

void drum_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {}

/*         Fader mode         */
/*----------------------------*/

void fader_init() {
	hal_plot_led(TYPESETUP, 0, 63, 0, 63); // Fader mode LED
}

void fader_timer_event() {
	for (u8 y = 0; y < 8; y++) {
		if (fader_counter[y]) {
			if (++fader_elapsed[y] >= fader_tick[y]) {
				faders[y] += fader_change[y]; // Update fader line
				
				fader_counter[y]--; // 
				if (!fader_counter[y]) {
					faders[y] = fader_final[y]; // Set fader to supposed final value
				}
				
				hal_send_midi(USBSTANDALONE, 0xB2, 21 + y, faders[y]); // Send fader
				
				for (u8 x = 0; x < 8; x++) { // Update fader LEDs
					if (faders[y] >= fader_levels[x]) {
						hal_plot_led(TYPEPAD, (x + 1) * 10 + (y + 1), 63, 0, 63);
					} else {
						hal_plot_led(TYPEPAD, (x + 1) * 10 + (y + 1), 0, 0, 0);
					}
				}
				
				fader_elapsed[y] = 0;
			}
		}
	}
}

void fader_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(254);
	
	} else {
		if (x == 0 || x == 9 || y == 0 || y == 9) { // Unused side buttons
			hal_send_midi(USBSTANDALONE, 0xB2, p, v);
			hal_plot_led(TYPEPAD, p, 0, (v == 0)? 0 : 63, 0);
		
		} else if (v != 0) { // Main grid
			x--; y--;
			u16 time = (128 - v) * 25; // Time it takes to do the line
						
			fader_final[y] = fader_stops[x]; // Save final value of the line
			
			s8 direction = 2 * (faders[y] < fader_final[y]) - 1; // Direction of line - {-1} = down, {1} = up
			u16 diff = (direction > 0)? (fader_final[y] - faders[y]) : (faders[y] - fader_final[y]); // Difference between current value and new value
			
			fader_elapsed[y] = 0; // Stop current line
			
			if (time >= diff) { // Enough time to do line smoothly
				fader_tick[y] = time / diff;
				fader_counter[y] = diff;
				fader_change[y] = direction;
				
			} else { // Not enough time - compensate with smaller steps (not always accurate - find a better way? - maybe floats)
				fader_tick[y] = 1;
				fader_counter[y] = time;
				fader_change[y] = direction * (diff / time);
			}
		}
	}
}

void fader_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {}

/*   Programmer mode (MIDI)   */
/*----------------------------*/

void programmer_init() {
	hal_plot_led(TYPESETUP, 0, 63, 18, 0); // Programmer mode LED
}

void programmer_timer_event() {}

void programmer_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (p == 0) { // Enter Setup mode
		if (v != 0) mode_update(254);
	
	} else {
		hal_send_midi(USBSTANDALONE, (x == 0 || x == 9 || y == 0 || y == 9)? 0xB3 : ((v == 0)? 0x83 : 0x93), p, v);
	}
}

void programmer_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
	if (port == USBSTANDALONE && ch == 0x3) {
		if (t == 0x8) {
			v = 0; // Note off
			t = 0x9;
		}
		
		u8 x = p / 10;
		u8 y = p % 10;
		
		if ((t == 0xB && (x == 0 || x == 9 || y == 0 || y == 9)) || (t == 0x9 && 1 <= x && x <= 8 && 1 <= y && y <= 8)) {
			hal_plot_led(TYPEPAD, p, palette[3][0][v], palette[3][1][v], palette[3][2][v]);
		}
	}
}

/*       Text scrolling       */
/*----------------------------*/

void text_init() {
	text_elapsed = text_tick;
	text_counter = 1;
	text_subcounter = 0;
	text_done = 0;
	
	for (u8 i = 0; i < 10; i++) text_frame[i] = 0;
}

void text_timer_event() {
	if (++text_elapsed >= text_tick) { // Next frame
		if (text_done) {
			mode_update(mode_default);
		
		} else {
			for (u8 i = 0; i < 9; i++) text_frame[i] = text_frame[i + 1]; // Shift all frames left
			
			u8 b = 0;
			
			if (text_counter) {
				b = text_bytes[text_counter];
				
				while (b < 8) {
					text_tick = (8 - b) * 20;
					
					if (++text_counter == text_bytes[0]) {
						text_counter = 0;
						break;
					}
					
					b = text_bytes[text_counter];
				}
			}
			
			if (text_counter) {
				b -= 32;
				
				if (text_subcounter++ < text_bitmap[b][0]) {
					text_frame[9] = text_bitmap[b][text_subcounter];
				} else {
					text_frame[9] = 0; // Padding between bytes
				}
				
				if (text_subcounter == text_bitmap[b][0] + 2) { // Increment counters
					text_subcounter = 0;
					if (++text_counter == text_bytes[0]) {
						text_counter = 0;
					}
				}
			
			} else {
				text_frame[9] = 0;
				
				if (!memcmp(&text_frame[0], &text_frame_empty[0], 10)) { // If frames empty
					hal_send_sysex(text_port, &syx_text_response[0], arr_size(syx_text_response));
					
					if (text_loop) {
						text_counter = 1;
						text_subcounter = 0;
					
					} else {
						text_done = 1;
					}
				}
			}
			
			for (u8 i = 0; i < 10; i++) display_u8(text_frame[i], 1, i, palette[3][0][text_color], palette[3][1][text_color], palette[3][2][text_color]); // Draw text
			
			text_elapsed = 0;
		}
	}
}

void text_surface_event(u8 p, u8 v, u8 x, u8 y) {
	if (!text_palette) mode_update(mode_default);
}

void text_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {}

/*    SysEx event handling    */
/*----------------------------*/

void app_sysex_event(u8 port, u8 * d, u16 l) {
	// Device Inquiry - Read information about the connected device
	if (syx_cmp(d, syx_device_inquiry, 6)) {
		hal_send_sysex(port, &syx_device_inquiry_response[0], arr_size(syx_device_inquiry_response));
		return;
	}
	
	// Challenge from the Control Surface
	if (syx_cmp(d, syx_challenge, 7)) {
		if (port == USBMIDI) {
			if (l == 12) {
				challenge_do = 1;
				challenge_counter = 0;
			
			} else if (l == 8) {
				if (mode_default != 0) {
					mode_default = 0;
					mode_update(mode_default);
				}
			}
		}
		return;
	}
	
	// Mode selection - return the status
	if (syx_cmp(d, syx_mode_selection, 7)) {
		syx_mode_selection_response[7] = *(d + 7);
		
		hal_send_sysex(port, &syx_mode_selection_response[0], arr_size(syx_mode_selection_response));
		
		u8 new = 1 - *(d + 7);
		if (mode_default != new) {
			mode_default = new;
			mode_update(mode_default); // This will interrupt boot animation!
		}
		
		return;
	}
	
	// Live layout selection - return the status
	if (syx_cmp(d, syx_live_layout_selection, 7)) {
		syx_live_layout_selection_response[7] = *(d + 7);
		
		hal_send_sysex(port, &syx_live_layout_selection_response[0], arr_size(syx_live_layout_selection_response));
		
		ableton_layout = *(d + 7);
		
		if (mode == 1) {
			if (ableton_layout == 0x3) {
				for (u8 i = 0; i < 100; i++) ableton_pads[i] = 0;
			}
			
			if (ableton_layout == 0x4) {
				for (u8 x = 10; x < 90; x += 10) {
					for (u8 y = 1; y < 9; y++) ableton_pads[x + y] = 0;
				}
			}
			
			mode_refresh();
		}
		return;
	}
	
	// Standalone layout selection - return the status
	if (syx_cmp(d, syx_standalone_layout_selection, 7)) {
		if (mode_default != 1) { // If not in Ableton mode
			syx_standalone_layout_selection_response[7] = *(d + 7);
			
			hal_send_sysex(port, &syx_standalone_layout_selection_response[0], arr_size(syx_standalone_layout_selection_response));
			
			u8 new = (syx_standalone_layout_selection_response[7] < 4)? (syx_standalone_layout_selection_response[7] + 2) : 0; // 4 for Performance mode (unavailable on stock)
			if (mode_default != new) {
				mode_default = new;
				mode_update(mode_default); // This will interrupt boot animation!
			}
		}
		return;
	}
	
	// Light LED using SysEx
	if (syx_cmp(d, syx_led_single, 7)) {
		for (u8 i = 7; i <= l - 3 && i <= 199; i += 2) {
			u8 p = *(d + i);
			u8 v = *(d + i + 1);
			
			if (p == 99) {
				hal_plot_led(TYPESETUP, 0, palette[3][0][v], palette[3][1][v], palette[3][2][v]);
			} else {
				hal_plot_led(TYPEPAD, p, palette[3][0][v], palette[3][1][v], palette[3][2][v]);
			}
		}
		return;
	}
	
	// Light a column of LEDs using SysEx
	if (syx_cmp(d, syx_led_column, 7)) {
		u8 y = *(d + 7);
		
		for (u8 i = 8; i <= l - 2 && i <= 17; i++) {
			u8 v = *(d + i);
			hal_plot_led(TYPEPAD, (i - 8) * 10 + y, palette[3][0][v], palette[3][1][v], palette[3][2][v]);
		}
		return;
	}
	
	// Light a row of LEDs using SysEx
	if (syx_cmp(d, syx_led_row, 7)) {
		u8 x = *(d + 7) * 10;
		
		for (u8 i = 8; i <= l - 2 && i <= 17; i++) {
			u8 v = *(d + i);
			hal_plot_led(TYPEPAD, x + i - 8, palette[3][0][v], palette[3][1][v], palette[3][2][v]);
		}
		return;
	}
	
	// Light all LEDs using SysEx
	if (syx_cmp(d, syx_led_all, 7)) {
		u8 v = *(d + 7);
		
		for (u8 p = 1; p < 99; p++) {
			hal_plot_led(TYPEPAD, p, palette[3][0][v], palette[3][1][v], palette[3][2][v]);
		}
		return;
	}
	
	// Light LED using SysEx (RGB mode)
	if (syx_cmp(d, syx_led_rgb, 7)) {
		for (u16 i = 7; i <= l - 5 && i <= 315; i += 4) {
			u8 p = *(d + i);
			u8 r = *(d + i + 1);
			u8 g = *(d + i + 2);
			u8 b = *(d + i + 3);
			
			if (p == 99) {
				hal_plot_led(TYPESETUP, 0, r, g, b);
			} else {
				hal_plot_led(TYPEPAD, p, r, g, b);
			}
		}
		return;
	}
	
	// Light LED grid using SysEx (RGB mode)
	if (syx_cmp(d, syx_led_grid, 7)) {
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
			u8 r = *(d + i);
			u8 g = *(d + i + 1);
			u8 b = *(d + i + 2);
			
			hal_plot_led(TYPEPAD, p, r, g, b);
			
			if (++p % 10 == 9 && t) {
				p += 2;
			}
		}
		return;
	}
	
	// Text scrolling
	if (syx_cmp(d, syx_text, 7)) {
		if (l <= 10) { // Empty message
			if (mode == 6 && port == text_port && !text_palette) mode_update(mode_default); // Stops the text scrolling
			
		} else if ((mode_default == 1 && port == USBMIDI) || (mode_default != 1 && port == USBSTANDALONE)) { // Valid message
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
			
			mode_update(6);
		}
		return;
	}
	
	// Retina palette download start
	if (syx_cmp(d, syx_palette_start, 8)) {
		if (!text_palette) {
			text_palette = 1;
			app_sysex_event(port, &syx_palette_text[0], arr_size(syx_palette_text));
		}
		return;
	}
	
	// Retina palette download write
	if (syx_cmp(d, syx_palette_write, 8)) {
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
		return;
	}
	
	// Retina palette download end
	if (syx_cmp(d, syx_palette_end, 8)) {
		if (text_palette) {
			text_palette = 0;
			mode_update(mode_default);
		}
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
	
	mode_init[254] = setup_init;
	mode_timer_event[254] = setup_timer_event;
	mode_surface_event[254] = setup_surface_event;
	mode_midi_event[254] = setup_midi_event;
	
	mode_init[253] = editor_init;
	mode_timer_event[253] = editor_timer_event;
	mode_surface_event[253] = editor_surface_event;
	mode_midi_event[253] = editor_midi_event;
	
	mode_init[252] = scale_setup_init;
	mode_timer_event[252] = scale_setup_timer_event;
	mode_surface_event[252] = scale_setup_surface_event;
	mode_midi_event[252] = scale_setup_midi_event;
	
	mode_init[0] = performance_init;
	mode_timer_event[0] = performance_timer_event;
	mode_surface_event[0] = performance_surface_event;
	mode_midi_event[0] = performance_midi_event;
	
	mode_init[1] = ableton_init;
	mode_timer_event[1] = ableton_timer_event;
	mode_surface_event[1] = ableton_surface_event;
	mode_midi_event[1] = ableton_midi_event;

	mode_init[2] = note_init;
	mode_timer_event[2] = note_timer_event;
	mode_surface_event[2] = note_surface_event;
	mode_midi_event[2] = note_midi_event;
	
	mode_init[3] = drum_init;
	mode_timer_event[3] = drum_timer_event;
	mode_surface_event[3] = drum_surface_event;
	mode_midi_event[3] = drum_midi_event;
	
	mode_init[4] = fader_init;
	mode_timer_event[4] = fader_timer_event;
	mode_surface_event[4] = fader_surface_event;
	mode_midi_event[4] = fader_midi_event;
	
	mode_init[5] = programmer_init;
	mode_timer_event[5] = programmer_timer_event;
	mode_surface_event[5] = programmer_surface_event;
	mode_midi_event[5] = programmer_midi_event;
	
	mode_init[6] = text_init;
	mode_timer_event[6] = text_timer_event;
	mode_surface_event[6] = text_surface_event;
	mode_midi_event[6] = text_midi_event;
	
	mode_update(255);
}