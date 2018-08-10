#include "modes/mode.h"
#include "led/led.h"
#include "flash/flash.h"

u8 mode = 0;
u8 mode_default = 0;

// Arrays of function pointers. Used to programmatically call the correct mode's function
void (*mode_init[11])();
void (*mode_timer_event[11])();
void (*mode_surface_event[11])(u8 p, u8 v, u8 x, u8 y);
void (*mode_midi_event[11])(u8 port, u8 t, u8 ch, u8 p, u8 v);

void mode_refresh() {
	clear_led();
	(*mode_init[mode])();
}

void mode_update(u8 x) {
	clear_led();
	flash_write();
	
	mode = x;
	(*mode_init[mode])();
}

void mode_default_update(u8 x) {
	if (mode_default != x) {
		mode_default = x;
		mode_update(mode_default);
	}
}