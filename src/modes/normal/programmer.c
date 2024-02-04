#include "modes/normal/programmer.h"

u8 programmer_screen[100][4] = {};

void programmer_led(u8 ch, u8 p, u8 v, u8 s) {
    switch (ch) {
        case 0x0:
            novation_led(p, v);
            break;

        case 0x1:
            flash_led(p, v);
            break;

        case 0x2:
            pulse_led(p, v);
            break;
    }

    if (ch <= 0x2 && s) {
        programmer_screen[p][0] = ch;
        for (int i = 0; i < 3; i++) {
            programmer_screen[p][i + 1] = palette_value(palette_novation, v, i);
        }
    }
}

void programmer_led_rgb(u8 ch, u8 p, u8 r, u8 g, u8 b, u8 s) {
    switch (ch) {
        case 0x0:
            rgb_led(p, r, g, b);
            break;

        case 0x1:
            flash_rgb_led(p, r, g, b);
            break;

        case 0x2:
            pulse_rgb_led(p, r, g, b);
            break;
    }
    
    if (ch <= 0x2 && s) {
        programmer_screen[p][0] = ch;
        programmer_screen[p][1] = r;
        programmer_screen[p][2] = g;
        programmer_screen[p][3] = b;
    }
}

void programmer_init() {
    for (u8 i = 0; i < 100; i++) {
        programmer_led_rgb(programmer_screen[i][0], i, programmer_screen[i][1], programmer_screen[i][2], programmer_screen[i][3], 0);
    }

    rgb_led(99, mode_programmer_r, mode_programmer_g, mode_programmer_b); // Programmer mode LED

    active_port = USBSTANDALONE;
}

void programmer_timer_event() {}

void programmer_surface_event(u8 p, u8 v, u8 x, u8 y) {
    if (p == 0) { // Enter Setup mode
        if (v != 0) mode_update(mode_setup);
    
    } else {
        send_midi(USBSTANDALONE, ((x == 0 || x == 9 || y == 0 || y == 9)? 0xB0 : (v? 0x90 : 0x80)) | settings.mode[mode_programmer].channel, p, v);
    }
}

void programmer_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {
    if (port == USBSTANDALONE && ch == settings.mode[mode_programmer].channel) {
        if (t == 0x8) {
            v = 0; // Note off
            t = 0x9;
        }
        
        u8 x = p / 10;
        u8 y = p % 10;
        
        if ((t == 0xB && (x == 0 || x == 9 || y == 0 || y == 9)) || (t == 0x9 && 1 <= x && x <= 8 && 1 <= y && y <= 8)) {
            programmer_led(0, p, v, 1);
        }
    }
}

void programmer_aftertouch_event(u8 v) {
    aftertouch_send(USBSTANDALONE, 0xD0 | settings.mode[mode_programmer].channel, v);
}

void programmer_poly_event(u8 p, u8 v) {
    send_midi(USBSTANDALONE, 0xA0 | settings.mode[mode_programmer].channel, p, v);
}
