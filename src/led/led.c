#include "led/led.h"

u8 led_screen[100][3] = {};
u8 flash_screen[100][3] = {};
u8 pulse_screen[100][3] = {};

double apply_led_brightness(u8 c) {
    if (c == 0) return 0;

    u32 x = (u32)settings.led_brightness;
    return ((c - 1) * (63 * (x + 5) * (x + 5) - 144) / (62 * 144)) + 1;
}

void direct_led(u8 p, u8 r, u8 g, u8 b) {
    if (p == 99) {
        hal_plot_led(TYPESETUP, 0, apply_led_brightness(r), apply_led_brightness(g), apply_led_brightness(b));
    } else {
        hal_plot_led(TYPEPAD, p, apply_led_brightness(r), apply_led_brightness(g), apply_led_brightness(b));
    }
}

void rgb_led(u8 p, u8 r, u8 g, u8 b) {
    direct_led(p, r, g, b);

    led_screen[p][0] = r;
    led_screen[p][1] = g;
    led_screen[p][2] = b;

    for (int i = 0; i < 3; i++) {
        flash_screen[p][i] = -1;
        pulse_screen[p][i] = 0;
    }
}

void clear_led() {
    for (u8 i = 0; i < 100; i++) rgb_led(i, 0, 0, 0);
}

void palette_led(u8 p, u8 v) {
    rgb_led(p, palette_value(settings.palette_selected, v, 0), palette_value(settings.palette_selected, v, 1), palette_value(settings.palette_selected, v, 2));
}

void novation_led(u8 p, u8 v) {
    if (p != 99) rgb_led(p, palette_value(palette_novation, v, 0), palette_value(palette_novation, v, 1), palette_value(palette_novation, v, 2));
}

void flash_led(u8 p, u8 v) {
    u8 palette_using = (mode == mode_performance)? settings.palette_selected : palette_novation;

    for (int i = 0; i < 3; i++) {
        flash_screen[p][i] = palette_value(palette_using, v, i);
        pulse_screen[p][i] = 0;
    }
}

void flash_rgb_led(u8 p, u8 r, u8 g, u8 b) {
    flash_screen[p][0] = r;
    flash_screen[p][1] = g;
    flash_screen[p][2] = b;

    for (int i = 0; i < 3; i++) {
        pulse_screen[p][i] = 0;
    }
}

void pulse_led(u8 p, u8 v) {
    if (!v) {
        rgb_led(p, 0, 0, 0);
    } else {
        u8 palette_using = (mode == mode_performance)? settings.palette_selected : palette_novation;

        for (int i = 0; i < 3; i++) {
            pulse_screen[p][i] = palette_value(palette_using, v, i);
        }
    }
    for (int i = 0; i < 3; i++) {
        flash_screen[p][i] = -1;
    }
}

void pulse_rgb_led(u8 p, u8 r, u8 g, u8 b) {
    if (!r && !g && !b) {
        rgb_led(p, 0, 0, 0);
    } else {
        pulse_screen[p][0] = r;
        pulse_screen[p][1] = g;
        pulse_screen[p][2] = b;
    }

    for (int i = 0; i < 3; i++) {
        led_screen[p][i] = 0;
        flash_screen[p][i] = -1;
    }
}

void display_u8(u8 v, u8 d, u8 x, u8 r, u8 g, u8 b) {
    for (u8 i = 0; i < 8; i++) {
        u8 w = (v >> i) & 1;
        rgb_led((d == 0)? (8 - i + x * 10) : ((i + 1) * 10 + x), r * w, g * w, b * w);
    }
}

void display_u6(u8 v, u8 d, u8 x, u8 r, u8 g, u8 b) {
    for (u8 i = 0; i < 6; i++) {
        u8 w = (v >> i) & 1;
        rgb_led((d == 0)? (7 - i + x * 10) : ((i + 2) * 10 + x), r * w, g * w, b * w);
    }
}
