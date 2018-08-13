#include "modes/mode.h"

// Arrays of function pointers. Used to programmatically call the correct mode's function
void (*const mode_init[12])() = {
	performance_init,
	ableton_init,
	note_init,
	drum_init,
	fader_init,
	programmer_init,
	text_init,
	scale_setup_init,
	editor_init,
	setup_init,
	boot_init,
	puyo_init
};
void (*const mode_timer_event[12])() = {
	performance_timer_event,
	ableton_timer_event,
	note_timer_event,
	drum_timer_event,
	fader_timer_event,
	programmer_timer_event,
	text_timer_event,
	scale_setup_timer_event,
	editor_timer_event,
	setup_timer_event,
	boot_timer_event,
	puyo_timer_event
};
void (*const mode_surface_event[12])(u8 p, u8 v, u8 x, u8 y) = {
	performance_surface_event,
	ableton_surface_event,
	note_surface_event,
	drum_surface_event,
	fader_surface_event,
	programmer_surface_event,
	text_surface_event,
	scale_setup_surface_event,
	editor_surface_event,
	setup_surface_event,
	boot_surface_event,
	puyo_surface_event
};
void (*const mode_midi_event[12])(u8 port, u8 t, u8 ch, u8 p, u8 v) = {
	performance_midi_event,
	ableton_midi_event,
	note_midi_event,
	drum_midi_event,
	fader_midi_event,
	programmer_midi_event,
	text_midi_event,
	scale_setup_midi_event,
	editor_midi_event,
	setup_midi_event,
	boot_midi_event,
	puyo_midi_event
};

u8 mode = 0;
u8 mode_default = 0;

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