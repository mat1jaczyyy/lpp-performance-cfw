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

    u8 reserved1;                    // reserved for future settings
    u8 reserved2;
    u8 reserved3;
} mode_settings_t;

typedef struct {
    u8 enabled:1;                    // 1 bit
    u8 note_translate:1;             // 1 bit
    u8 selected;                     // 5 bit, but reserve in case new scales
    u8 root:4;                       // < 12
    u8 segment:3;                    // 3 bit
    u8 reserved;                     // reserved for future settings
} scale_settings_t;

typedef struct {
    mode_settings_t mode[32];        // 4 byte per entry (normal modes), rest are reserved for future modes
    scale_settings_t scale;          // 4 byte

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