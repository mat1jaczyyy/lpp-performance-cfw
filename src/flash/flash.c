#include "flash/flash.h"

#define flash_indicator_r 3
#define flash_indicator_g 0
#define flash_indicator_b 0

#define flash_user_size (palette_custom * 3 * 32 * 3 + 18)

#define flash_user_start 0x2400

u8 flash[flash_user_size] = {}; // TODO Optimize RAM

u8 dirty = 0;

void flash_read() {
	u16 fp = flash_user_start;
	
	for (u8 p = 0; p < palette_custom; p++) {
		for (u8 i = 0; i < 3; i++) {
			for (u8 j = 0; j < 128; j += 4) {
				palette[p][i][j] = flash_direct_read(fp) >> 2;
				palette[p][i][j + 1] = ((flash_direct_read(fp) << 4) + (flash_direct_read(fp + 1) >> 4)) & 63;
				palette[p][i][j + 2] = ((flash_direct_read(fp + 1) << 2) + (flash_direct_read(fp + 2) >> 6)) & 63;
				palette[p][i][j + 3] = flash_direct_read(fp + 2) & 63;
				
				fp += 3;
			}
			
			palette[p][i][0] = 0;
		}
	}
	
	// Doesn't need to check palette for 0xFF
	
	palette_selected = flash_direct_read(fp++);
	if (palette_selected >= palette_count) palette_selected = palette_novation;
	
	vel_sensitive = flash_direct_read(fp++);
	if (vel_sensitive >> 1) vel_sensitive = 0;
	
	top_lights_config = flash_direct_read(fp++);
	if (top_lights_config >> 2) top_lights_config = 0;

	performance_xy_enabled = flash_direct_read(fp++);
	if (performance_xy_enabled >> 1) performance_xy_enabled = 0;

	aftertouch_enabled = flash_direct_read(fp++);
	if (aftertouch_enabled >> 1) aftertouch_enabled = 0;

	direct_enabled = flash_direct_read(fp++);
	if (direct_enabled >> 1) direct_enabled = 0;

	idle_enabled = flash_direct_read(fp++);
	if (idle_enabled >> 1) idle_enabled = 1;

	led_brightness = flash_direct_read(fp++);
	if (led_brightness >> 3) led_brightness = 7;

	custom_surface_led = flash_direct_read(fp++);
	if (custom_surface_led >> 1) custom_surface_led = 1;

	custom_midi_led = flash_direct_read(fp++);
	if (custom_midi_led >> 1) custom_midi_led = 1;

	custom_fader_vel_sensitive = flash_direct_read(fp++);
	if (custom_fader_vel_sensitive >> 1) custom_fader_vel_sensitive = 1;

	ableton_channel = flash_direct_read(fp++) & 0xF;
	if ((ableton_channel & 0x7) < 5) ableton_channel = 5;

	for (u8 i = 0; i < sizeof(channels); i++)
		channels[i] = flash_direct_read(fp++) & 0xF;
}

void flash_write() {
	if (!dirty) return;

	rgb_led(99, flash_indicator_r, flash_indicator_g, flash_indicator_b);

	u16 fp = 0;
	
	// Compress 4 color values into 3 bytes. 6 bits * 4 values = 24 bits = 3 bytes
	for (u8 p = 0; p < palette_custom; p++) {
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
	flash[fp++] = performance_xy_enabled;
	flash[fp++] = aftertouch_enabled;
	flash[fp++] = direct_enabled;
	flash[fp++] = idle_enabled;
	flash[fp++] = led_brightness;
	flash[fp++] = custom_surface_led;
	flash[fp++] = custom_midi_led;
	flash[fp++] = custom_fader_vel_sensitive;
	flash[fp++] = ableton_channel;

	for (u8 i = 0; i < sizeof(channels); i++)
		flash[fp++] = channels[i];
	
	flash_direct_write(flash_user_start, &flash[0], fp);
	
	rgb_led(99, 0, 0, 0);
	dirty = 0;
}

void flash_write_custom(u8 index, const u8* buffer) {
	rgb_led(99, flash_indicator_r, flash_indicator_g, flash_indicator_b);

	flash_direct_write(0x400 + 0x400 * index, buffer, 1024);
	
	rgb_led(99, 0, 0, 0);
}

const u8* chord_banks = (const u8*)0x0801D400;

void flash_write_chord_bank(u8 index, const u8* buffer) {
	memcpy(flash, chord_banks, 14 * 9);
	memcpy(flash + 9 * index, buffer, 9);
	flash_direct_write(0x0, flash, 14 * 9);
}