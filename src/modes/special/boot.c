#include "modes/special/boot.h"

#define boot_note_tick 54
#define boot_note_length 17

#define boot_rotate_180 1

#define boot_fade_tick 3

#define boot_colors_length 252

#define boot_user_stop 188

const u8 boot_points[boot_note_length + 1] = {0, 5, 10, 14, 18, 22, 26, 29, 32, 34, 37, 39, 41, 43, 46, 47, 48, 49};

const u8 boot_notes[49] = {
    89, 78, 67, 56, 45,
    79, 68, 57, 46, 55,
    69, 58, 47, 36,
    59, 48, 37, 35,
    49, 38, 27, 65,
    39, 28, 26, 66,
    29, 18, 25,
    19, 17, 75,
    16, 76,
    8, 15, 77,
    7, 85,
    6, 86,
    5, 87,
    95, 88, 99,
    96,
    97,
    98
};

u8 boot_colors(u8 i, u8 c) {
    if (c == 0) {
        if (i < 63) return 0;
        if (i < 126) return i - 62;
        if (i < 189) return 63;
        if (i < 252) return 251 - i;
    
    } else if (c == 1) {
        if (i < 63) return (i + 1) / 7;
        if (i < 126) return (125 - i) / 7;
    
    } else if (c == 2) {
        if (i < 63) return i + 1;
        if (i < 126) return 63;
        if (i < 189) return 188 - i;
        if (i < 252) return 0;
    }

    return 0;
}

u8 boot_note_elapsed = boot_note_tick;
u8 boot_note_floor = 0;
u8 boot_note_ceil = 0;

u8 boot_fade_elapsed = boot_fade_tick;
u8 boot_fade_counter[16] = {};

void boot_init() {}

void boot_timer_event() {
    if (++boot_note_elapsed >= boot_note_tick) { // Start fading next note
        if (boot_note_ceil < boot_note_length) boot_note_ceil++;
        
        boot_note_elapsed = 0;
    }
    
    if (++boot_fade_elapsed >= boot_fade_tick) { // Redraw fades
        if (boot_note_floor == boot_note_length) { // Enter Performance mode (end condition)
            mode_update(mode_performance);
        
        } else {
            for (u8 i = boot_note_floor; i < boot_note_ceil; i++) {
                if (boot_fade_counter[i] < boot_colors_length) { // Draw next fade for note
                    for (u8 j = 0; j < boot_points[i + 1] - boot_points[i]; j++) { // For each note in frame
                        if (boot_notes[boot_points[i] + j] != 98 || boot_fade_counter[i] <= boot_user_stop) { // Check for leaving User LED on
                            rgb_led(boot_notes[boot_points[i] + j], boot_colors(boot_fade_counter[i], 0), boot_colors(boot_fade_counter[i], 1), boot_colors(boot_fade_counter[i], 2));
                        }
                        
                        if (boot_rotate_180) rgb_led(99 - boot_notes[boot_points[i] + j], boot_colors(boot_fade_counter[i], 0), boot_colors(boot_fade_counter[i], 1), boot_colors(boot_fade_counter[i], 2));
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
    if (p == 0 && v != 0) mode_update(mode_performance); // Manually interrupt boot animation
}

void boot_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {}

void boot_aftertouch_event(u8 v) {}

void boot_poly_event(u8 p, u8 v) {}
