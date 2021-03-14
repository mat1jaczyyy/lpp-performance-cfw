#ifndef SETTINGS_H
#define SETTINGS_H

#include "app_defs.h"

#include "stm32f10x_imports.h"

#include "modes/mode.h"
#include "led/palettes.h"

const u32 settings_header;

typedef struct {
    u8 channel:4;
    u8 aftertouch_enabled:2;
    u8 vel_sensitive:1;
    u8 direct_enabled:1;
} mode_settings_t;

typedef struct {
    mode_settings_t mode[32];        // 1 byte per entry (normal modes), rest are reserved for future modes

    u8 idle_enabled;                 // 1 bit
    u8 led_brightness;               // 3 bit
    
    u8 palette_selected;             // 3 bit
    u8 top_lights_config;            // 2 bit; 0 = PRO, 1 = MK2, 2 = MK2 Rotated, 3 = MK2 Mirrored
    u8 performance_xy_enabled;       // 1 bit

    u8 custom_surface_led;           // 1 bit
    u8 custom_midi_led;              // 1 bit
    u8 custom_fader_vel_sensitive;   // 1 bit
} settings_t;  // make sure the total size of this is aligned to 4-byte, otherwise writing to flash won't work! (CRC periph word size is 4b)

settings_t settings;

void settings_init();
void settings_validate();
u32 settings_crc();

#endif