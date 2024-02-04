#include "flash/settings.h"

const u32 settings_header = 0x00574643;  // {'C', 'F', 'W', '\0'} there is no nice way to init this

const settings_t settings_default = {
    {   // mode settings
        {0, 0, 0, 0},  // performance
        {5, 0, 0, 0},  // ableton
        {0, 0, 1, 0},  // note
        {1, 0, 1, 0},  // drum
        {2, 0, 1, 0},  // fader
        {0, 0, 0, 0},  // programmer
        {3, 0, 1, 0},  // piano
        {4, 0, 1, 0},  // chord
        {0, 0, 1, 0},  // custom
    },

    {   // scale settings
        0,   // enabled
        1,   // note_translate
        25,  // selected
        0,   // root
        7,   // segment
    },

    1,                // idle_enabled
    7,                // led_brightness

    palette_novation, // palette_selected
    0,                // top_lights_config
    0,                // performance_xy_enabled

    1,                // custom_surface_led
    1,                // custom_midi_led
    0                 // custom_fader_vel_sensitive
};

settings_t settings;

void settings_init() {
    settings = settings_default;
}

#define validate(key, check) if (settings.key check) settings.key = settings_default.key

void settings_validate() {
    for (u8 i = 0; i < 32; i++) {
        validate(mode[i].aftertouch_enabled, > 2);
    }

    validate(scale.selected, >> 5);
    validate(scale.root, >= 12);
    validate(scale.segment, - 1 > 7);

    validate(mode[mode_ableton].channel, % 8 < 5);

    validate(idle_enabled, >> 1);
    validate(led_brightness, >> 3);

    validate(palette_selected, >= palette_count);
    validate(top_lights_config, >> 2);
    validate(performance_xy_enabled, >> 1);

    validate(custom_surface_led, >> 1);
    validate(custom_midi_led, >> 1);
    validate(custom_fader_vel_sensitive, >> 1);
}

u32 settings_crc(const void* x, const u32 len) {
    CRC_ResetDR();
    return CRC_CalcBlockCRC((u32*)x, len / sizeof(u32));
}
