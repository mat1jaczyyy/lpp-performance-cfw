#include "modes/normal/chord.h"

// Thanks to EmilyLP for colors and testing!

#define chord_color_empty_r 0
#define chord_color_empty_g 0
#define chord_color_empty_b 0

#define chord_color_major_dominant_r 0
#define chord_color_major_dominant_g 0
#define chord_color_major_dominant_b 63

#define chord_color_minor_r 15
#define chord_color_minor_g 7
#define chord_color_minor_b 63

#define chord_color_diminished_r 0
#define chord_color_diminished_g 63
#define chord_color_diminished_b 15

#define chord_color_triad_r 63
#define chord_color_triad_g 15
#define chord_color_triad_b 0

#define chord_color_pressed_r 63
#define chord_color_pressed_g 63
#define chord_color_pressed_b 63

#define chord_color_locked_r 3
#define chord_color_locked_g 63
#define chord_color_locked_b 11

#define chord_color_unlocked_r 15
#define chord_color_unlocked_g 0
#define chord_color_unlocked_b 0

#define chord_color_bank_valid_r 63
#define chord_color_bank_valid_g 63
#define chord_color_bank_valid_b 63

#define chord_color_bank_invalid_r 15
#define chord_color_bank_invalid_g 15
#define chord_color_bank_invalid_b 15

#define chord_color_sustain_r 63
#define chord_color_sustain_g 15
#define chord_color_sustain_b 31

#define chord_color_delete_pressed_r 63
#define chord_color_delete_pressed_g 0
#define chord_color_delete_pressed_b 0

#define chord_color_delete_released_r 7
#define chord_color_delete_released_g 7
#define chord_color_delete_released_b 7

#define chord_octave_start 4

const u8 chord_octave_colors[5][3] = {{63, 0, 63}, {20, 0, 63}, {0, 0, 63}, {0, 0, 31}, {0, 0, 7}};
const u8 chord_map[5][5] = {
    {0, 4, 14, 11, 16},
    {0, 4, 10, 7, 12},
    {0, 4, 9, 6, 11},
    {0, 4, 8, 5, 10},
    {0, 4, 7, 3, 6}
};

s8 chord_octave = chord_octave_start;

u8 chord_shift = 0;
u8 chord_nav_pressed[2] = {};

u8 chord_kind[8] = {};
u8 chord_triads = 8;

u8 chord_lock = 0;
u8 chord_edit = 255;
u8 chord_edit_data[9];

u16 chord_bank_valid;
u16 chord_bank_pressed;
u8 chord_notes_pressed[24];

u8 chord_sustain;
u8 chord_sustain_holding[24];

u8 chord_delete_pressed;

void rmemcpy(u8* dest, const u8* src, s32 n) {
    for (n--; n >= 0; n--)
        dest[n] = src[n];
}

inline const u8* chord_bank(u8 i) {
    return chord_banks + i * 9;
}

u8 chord_edit_flip(const u8 p) {
    if (p > 0x7F) return 0;

    u8 f = chord_edit_data[7] > 0x7F;

    for (u8 i = 0; i < 8; i++) {
        if (chord_edit_data[i] == p) { // Clear position
            memcpy(chord_edit_data + i, chord_edit_data + i + 1, 8 - i - 1);
            chord_edit_data[7] = 0xFF;
            return 1;

        } else if (f && chord_edit_data[i] > p) { // Set position
            rmemcpy(chord_edit_data + i + 1, chord_edit_data + i, 8 - i - 1);
            chord_edit_data[i] = p;
            return 1;
        }
    }

    return 0;
}

void chord_note_register(const u8 p, const u8 v, u8 sustain) {
    if (p > 0x7F) return;

    const u8 n = sizeof(chord_notes_pressed);
    u8* buf = sustain? chord_sustain_holding : chord_notes_pressed;
    u8 f = buf[n - 1] > 0x7F;

    for (u8 i = 0; i < n; i++) {
        if (buf[i] == p) { // Clear position
            if (!v) {
                memcpy(buf + i, buf + i + 1, n - i - 1);
                buf[n - 1] = 0xFF;
            }
            return;

        } else if (f && buf[i] > p) { // Set position
            if (v) {
                rmemcpy(buf + i + 1, buf + i, n - i - 1);
                buf[i] = p;
            }
            return;
        }
    }
}

s8 chord_value(u8 x, u8 y) {
    u8 l = scales_length(settings.scale.selected);
    u8 i = chord_octave * l + (x - 1) + chord_map[l > 8? 0: 9 - l][y - 1];
    s8 n = 12 * (i / l) + scales(settings.scale.selected, i % l) + settings.scale.root;

    return n;
}

s8 chord_press_result[8] = {};

u8 chord_press(u8 x, u8 y, u8 v, s8 out_p) {
    u8 p = x * 10 + y;

    if (chord_kind[x - 1] == 0 || (y == 6 && x > chord_triads)) {
        rgb_led(p, chord_color_empty_r, chord_color_empty_g, chord_color_empty_b);
        return 0;
    }

    u8 root2 = scales_length(settings.scale.selected) + 1;
    s8 n = 0;
    s8 r = -1;

    if (y == 7 || y == 8) { // Bank
        u8 bank = (x - 2) * 2 + y - 7;
        
        if ((chord_bank_valid >> bank) & 1) {
            for (; n < 8; n++) {
                if (chord_bank(bank)[n] > 0x7F) break;
                chord_press_result[n] = chord_bank(bank)[n];
            }
        }

    } else if (y == 6) { // Triad
        u8 b = chord_value(1, 1) + 12;

        chord_press_result[0] = chord_value(x, 1) + 12;
        chord_press_result[1] = chord_value(x, 3);
        chord_press_result[2] = chord_value(x, 2) + 12;
        
        for (u8 i = 1; i < 3; i++) {
            while (chord_press_result[i] >= b + 12)
                chord_press_result[i] -= 12;
        }
        
        for (u8 i = 0; i < 3; i++) {
            if (chord_press_result[i] < 0) {
                rgb_led(p, chord_color_empty_r, chord_color_empty_g, chord_color_empty_b);
                return 0;
            }
        }
        
        n = 3;

    } else { // Chord
        r = chord_value(x, y);
        n = 1;

        if (out_p < 0)
            chord_press_result[0] = r;
    }

    if (n == 0) return 0;
    if (out_p == -10) return n; // Aftertouch

    #define brighten(x) x = ((3 * (x) + 63) >> 2)
    #define brighten_all if (x == 1 || x == root2) { brighten(r); brighten(g); brighten(b); }

    if (out_p < 0) {
        for (u8 i = 0; i < n; i++) {
            chord_note_register(chord_press_result[i], v, 0);

            if (chord_sustain && v)
                chord_note_register(chord_press_result[i], v, 1);

            if (out_p == -20) continue;

            u8 skip = 0;
            if (v && chord_edit < 14)
                skip = chord_edit_flip(chord_press_result[i]);
                
            if (!skip)
                for (u8 _x = 1; _x < 9; _x++) { // I'm lazy
                    for (u8 _y = 1; _y < 6; _y++) {
                        chord_press(_x, _y, v, chord_press_result[i]);
                    }
                }
        }
        
        if (y == 6) {
            if (v) rgb_led(p, chord_color_pressed_r, chord_color_pressed_g, chord_color_pressed_b);
            else {
                u8 r = chord_color_triad_r, g = chord_color_triad_g, b = chord_color_triad_b;

                brighten_all

                rgb_led(p, r, g, b);
            }
        }

    } else if (n == 1 && r == out_p) {
        if (v) {
            rgb_led(p, chord_color_pressed_r, chord_color_pressed_g, chord_color_pressed_b);

        } else {
            u8 r, g, b;

            switch (chord_kind[x - 1]) {
                case 1:
                    r = chord_color_major_dominant_r; g = chord_color_major_dominant_g; b = chord_color_major_dominant_b;
                    break;
                
                case 2:
                    r = chord_color_minor_r; g = chord_color_minor_g; b = chord_color_minor_b;
                    break;
                
                default:
                    r = chord_color_diminished_r; g = chord_color_diminished_g; b = chord_color_diminished_b;
                    break;
            }

            brighten_all

            rgb_led(p, r, g, b);
        }
    }

    return n;
}

void chord_draw_sustain_lock() {
    if (chord_lock) {
        rgb_led(18, chord_color_locked_r, chord_color_locked_g, chord_color_locked_b);

        if (chord_sustain) {
            rgb_led(17, chord_color_sustain_r, chord_color_sustain_g, chord_color_sustain_b);
        } else {
            rgb_led(17, chord_color_sustain_r >> 1, chord_color_sustain_g >> 1, chord_color_sustain_b >> 1);
        }

    } else {
        rgb_led(18, chord_color_unlocked_r, chord_color_unlocked_g, chord_color_unlocked_b);
        rgb_led(17, chord_color_sustain_r >> 2, chord_color_sustain_g >> 2, chord_color_sustain_b >> 2);
    }
}

void chord_draw_banks() {
    for (u8 i = 0; i < 14; i++) {
        u8 p = ((i >> 1) + 2) * 10 + (i & 1) + 7;

        if ((chord_bank_pressed >> i) & 1) {
            rgb_led(p, i != chord_edit? 0 : 63, i != chord_edit? 63 : 0, 0);
        } else if ((chord_bank_valid >> i) & 1) {
            rgb_led(p, chord_color_bank_valid_r, chord_color_bank_valid_g, chord_color_bank_valid_b);
        } else {
            rgb_led(p, chord_color_bank_invalid_r, chord_color_bank_invalid_g, chord_color_bank_invalid_b);
        }

        if (i == chord_edit) {
            for (u8 i = 0; i < 8; i++) {
                if (chord_edit_data[i] > 0x7F) break;

                for (u8 x = 1; x < 9; x++) {
                    for (u8 y = 1; y < 6; y++) {
                        u8 r = chord_value(x, y);

                        if (r == chord_edit_data[i])
                            rgb_led(x * 10 + y, 63, 0, 0);
                    }
                }
            }
        }
    }
}

void chord_draw_navigation() {
    u8 o = chord_octave > 4? 8 - chord_octave: chord_octave;
    u8 u = chord_octave > 4? o : 4;
    u8 d = chord_octave < 4? o : 4;

    rgb_led(91, chord_octave_colors[u][0], chord_octave_colors[u][1], chord_octave_colors[u][2]);
    rgb_led(92, chord_octave_colors[d][0], chord_octave_colors[d][1], chord_octave_colors[d][2]);
}

void chord_draw_delete_button() {
    if (chord_delete_pressed) {
        rgb_led(50, chord_color_delete_pressed_r, chord_color_delete_pressed_g, chord_color_delete_pressed_b);
    } else {
        rgb_led(50, chord_color_delete_released_r, chord_color_delete_released_g, chord_color_delete_released_b);
    }
}

void chord_scale_button() {
    if (chord_shift) { // Shift button pressed
        rgb_led(80, 63, 63, 63);
        rgb_led(96, 7, 7, 7);

    } else { // Shift button released
        rgb_led(80, 7, 7, 7);
        rgb_led(96, 0, 0, 0);
    }
}

void chord_draw() {
    chord_triads = 8;

    for (u8 x = 1; x < 9; x++) { // Regular notes
        for (u8 y = 1; y < 6; y++) {
            s8 r = chord_value(x, y);
            if (y < 4) chord_press_result[y - 1] = r;

            if (chord_triads >= 8 && (s8)(r + 12) < 0)
                chord_triads = x - 1;

            if (r < 0) {
                chord_kind[x - 1] = 0;
                goto next;
            }
        }

        chord_press_result[2] -= 12;
        s8 f = chord_press_result[2] - chord_press_result[0];
        s8 s = chord_press_result[1] - chord_press_result[2];

        if (f == 4 && s == 3) // Major
            chord_kind[x - 1] = 1;

        else if (f == 3 && s == 4) // Minor
            chord_kind[x - 1] = 2;
        
        else chord_kind[x - 1] = 3; // Has to be Diminished

        next:
        for (u8 y = 1; y < 7; y++) {
            chord_press(x, y, 0, -1);
        }
    }

    for (u8 i = 0; i < 128; i++) { // Turn off all notes
        send_midi(USBSTANDALONE, 0x80 | settings.mode[mode_chord].channel, i, 0);
    }

    chord_draw_navigation();
    chord_draw_sustain_lock();
    chord_draw_delete_button();
    chord_draw_banks();
}

void chord_send(u8 x, u8 y, u8 v, u8 t) {
    s8 out_p = -1;
    if ((t >> 4) == 0xA) out_p = -10;           // Aftertouch special value -10: ignores logic, only aftertouch output
    else if (chord_sustain && !v) out_p = -20;  // Sustain    special value -20: ignores logic, only sustain management

    u8 n = chord_press(x, y, v, out_p);

    if (out_p != -20)
        for (u8 i = 0; i < n; i++)
            send_midi(USBSTANDALONE, t | settings.mode[mode_chord].channel, chord_press_result[i], v);
}

u8 chord_bank_checksum(const u8* bank) {
    u8 sum = 0;

    for (u8 j = 0; j < 8; j++)
        sum += bank[j] ^ 0b10101010;
    
    return sum;
}

void chord_bank_save(const u8 next) {
    if (chord_edit < 14) {
        u8 checksum = chord_bank_checksum(chord_edit_data);
        chord_edit_data[8] = (chord_edit_data[0] == 0xFF)? 0x00 : checksum;

        u8 valid = checksum == chord_edit_data[8];
        u8 should_save = valid != ((chord_bank_valid >> chord_edit) & 1);

        if (!should_save && valid) {
            for (u8 i = 0; i < 8; i++) {
                if (chord_edit_data[i] != chord_bank(chord_edit)[i]) {
                    should_save = 1;
                    break;
                }
            }
        }

        if (should_save) {
            flash_write_chord_bank(chord_edit, chord_edit_data);

            u16 mask = 1 << chord_edit;
            chord_bank_valid &= ~mask;

            if (valid) chord_bank_valid |= mask;
        }
    }
    
    chord_edit = next;
    memset(chord_edit_data, 0xFF, sizeof(chord_edit_data));
}

void chord_sustain_toggle(u8 v) {
    if ((chord_sustain > 0) == (v > 0)) return;

    chord_sustain = v;
    chord_draw_sustain_lock();

    if (!chord_sustain) {
        for (u8 i = 0; i < sizeof(chord_sustain_holding); i++) {
            if (chord_sustain_holding[i] > 0x7F) break;
            
            u8 release = 1;
            for (u8 j = 0; j < sizeof(chord_notes_pressed); j++) {
                if (chord_notes_pressed[j] > 0x7F) break;
                if (chord_sustain_holding[i] == chord_notes_pressed[j]) {
                    release = 0;
                    break;
                }
            }

            if (release) {
                for (u8 x = 1; x < 9; x++) { // I'm lazy
                    for (u8 y = 1; y < 6; y++) {
                        chord_press(x, y, v, chord_sustain_holding[i]);
                    }
                }
                send_midi(USBSTANDALONE, 0x80 | settings.mode[mode_chord].channel, chord_sustain_holding[i], v);
            }
        }

        memset(chord_sustain_holding, 0xFF, sizeof(chord_sustain_holding));

    } else memcpy(chord_sustain_holding, chord_notes_pressed, sizeof(chord_sustain_holding));
}

void chord_init() {
    chord_bank_pressed = 0;
    chord_bank_valid = 0;
    chord_sustain = 0;
    chord_delete_pressed = 0;
    memset(chord_notes_pressed, 0xFF, sizeof(chord_notes_pressed));
    memset(chord_sustain_holding, 0xFF, sizeof(chord_sustain_holding));

    // Validate chord banks
    for (u8 i = 0; i < 14; i++) {
        if (chord_bank_checksum(chord_bank(i)) == chord_bank(i)[8])
            chord_bank_valid |= 1 << i;
    }

    chord_draw();
    chord_scale_button();
    if (mode == mode_chord) rgb_led(99, mode_chord_r, mode_chord_g, mode_chord_b); // Chord mode LED

    active_port = USBSTANDALONE;
}

void chord_timer_event() {}

void chord_surface_event(u8 p, u8 v, u8 x, u8 y) {
    if (p == 0) { // Enter Setup mode
        if (v != 0) mode_update(mode_setup);
        return;
    }

    if (x == 0 || (x == 9 && y >= 3) || y == 0 || y == 9) { // Unused side buttons
        send_midi(USBSTANDALONE, 0xB0 | settings.mode[mode_chord].channel, p, v);
        rgb_led(p, 0, (v == 0)? 0 : 63, 0);
    }
    
    if (p == 96 && chord_shift && v != 0) { // Shift+Note button
        mode_update(mode_scale_setup); // Enter Setup for Scale mode
        rgb_led(p, 63, 63, 63);

    } else if (p == 96 && v == 0) {
        chord_scale_button();

    } else if (p == 80) { // Shift button
        chord_shift = v;
        chord_scale_button();

    } else if (x == 9 && y < 5) { // Navigation buttons
        if (v != 0) {
            switch (p) {
                case 91: // Octave up
                    if (chord_octave < 8) chord_octave++;
                    break;

                case 92: // Octave down
                    if (chord_octave > 0) chord_octave--;
                    break;
            }
        }

        chord_nav_pressed[p - 91] = (v)? 1 : 0;
        
        if (chord_nav_pressed[0] && chord_nav_pressed[1]) { // Reset offset
            chord_octave = chord_octave_start;
        }

        chord_draw();

    } else if (1 <= x && x <= 8 && 1 <= y && y <= 6) { // Main grid
        chord_send(x, y, v, v? 0x90 : 0x80);

        if (chord_edit < 14)
            chord_draw_banks();
    
    } else if (p == 18) { // Lock
        if (v) {
            chord_lock = 1 - chord_lock;

            if (!chord_lock && chord_sustain)
                chord_sustain_toggle(0);

            chord_bank_save(255);
            
            chord_draw_sustain_lock();
            chord_draw_banks();
        }
    
    } else if (2 <= x && x <= 8 && 7 <= y && y <= 8) { // Chord banks
        u8 bank = (x - 2) * 2 + y - 7;

        if (v && chord_delete_pressed) {
            chord_bank_save(bank);  // Save previously loaded bank if any, and load empty bank for deleting
            chord_bank_save(bank);  // Save empty bank to selected bank, and continue editing it

        } else if (!chord_lock && (v || chord_edit == bank)) {
            chord_bank_save((!v && chord_edit == bank)? 255 : bank);

            if (chord_edit < 14) {
                for (u8 i = 0; i < sizeof(chord_notes_pressed); i++) {
                    if (chord_notes_pressed[i] > 0x7F) break;

                    chord_edit_flip(chord_notes_pressed[i]);
                }
            }
        }

        chord_send(x, y, v, v? 0x90 : 0x80);

        if (v) chord_bank_pressed |= 1 << bank;
        else chord_bank_pressed &= ~(1 << bank);

        chord_draw_banks();
    
    } else if (p == 17) { // Sustain pedal
        if (chord_lock)
            chord_sustain_toggle(v);
    
    } else if (p == 50) { // Delete bank
        chord_delete_pressed = v;
        chord_draw_delete_button();
    }
}

void chord_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
    if (port == USBSTANDALONE && ch == settings.mode[mode_chord].channel) {
        switch (t) {
            case 0x8:
                v = 0;

            case 0x9:
                if (chord_sustain && !v) return;

                for (u8 x = 1; x < 9; x++) { // I'm lazy
                    for (u8 y = 1; y < 6; y++) {
                        chord_press(x, y, v, p);
                    }
                }
                break;
        }
    }
}

void chord_aftertouch_event(u8 v) {
    aftertouch_send(USBSTANDALONE, 0xD0 | settings.mode[mode_chord].channel, v);
}

void chord_poly_event(u8 p, u8 v) {
    u8 x = p / 10;
    u8 y = p % 10;
    
    if (1 <= x && x <= 8 && 1 <= y && y <= 8 && p != 17 && p != 18)
        chord_send(p / 10, p % 10, v, 0xA0 | settings.mode[mode_chord].channel);
}
