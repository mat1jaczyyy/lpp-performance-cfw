#include "led/led.h"
#include "led/palettes.h"
#include "app.h"
#include "flash/settings.h"

u8 flash_screen[100] = {};
u8 pulse_screen[100] = {};

void rgb_led(u8 p, u8 r, u8 g, u8 b) {
	if (p == 99) {
		hal_plot_led(TYPESETUP, 0, r, g, b);
	} else {
		hal_plot_led(TYPEPAD, p, r, g, b);
	}
	flash_screen[p] = 0;
	pulse_screen[p] = 0;
}

void clear_led() {
	for (u8 i = 1; i < 100; i++) rgb_led(i, 0, 0, 0);
}

void palette_led(u8 p, u8 v) {
	rgb_led(p, palette[palette_selected][0][v], palette[palette_selected][1][v], palette[palette_selected][2][v]);
}

void novation_led(u8 p, u8 v) {
	if (p != 99) rgb_led(p, palette[palette_novation][0][v], palette[palette_novation][1][v], palette[palette_novation][2][v]);
}

void flash_led(u8 p, u8 v) {
	if (!v) {
		rgb_led(p, 0, 0, 0);
	} else {
		flash_screen[p] = v;
	}
	pulse_screen[p] = 0;
}

void pulse_led(u8 p, u8 v) {
	if (!v) {
		rgb_led(p, 0, 0, 0);
	} else {
		pulse_screen[p] = v;
	}
	flash_screen[p] = 0;
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