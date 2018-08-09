#include "flash.h"

#define flash_indicator_r 3
#define flash_indicator_g 0
#define flash_indicator_b 0
u8 flash[USER_AREA_SIZE] = {0};
u8 dirty = 0;

void flash_read() {
	hal_read_flash(0, &flash[0], USER_AREA_SIZE);
	u16 fp = 0;
	
	for (u8 p = 0; p < palette_custom; p++) {
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
		
		hal_write_flash(0, &flash[0], USER_AREA_SIZE);
		
		rgb_led(99, 0, 0, 0);
		dirty = 0;
	}
}