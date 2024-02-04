#include "modes/mode.h"

// Arrays of function pointers. Used to programmatically call the correct mode's function
void (*const mode_init[16])() = {
    performance_init,
    ableton_init,
    note_init,
    drum_init,
    fader_init,
    programmer_init,
    piano_init,
    chord_init,
    custom_init,
    text_init,
    scale_setup_init,
    editor_init,
    setup_init,
    boot_init,
    puyo_init,
    idle_init
};
void (*const mode_timer_event[16])() = {
    performance_timer_event,
    ableton_timer_event,
    note_timer_event,
    drum_timer_event,
    fader_timer_event,
    programmer_timer_event,
    piano_timer_event,
    chord_timer_event,
    custom_timer_event,
    text_timer_event,
    scale_setup_timer_event,
    editor_timer_event,
    setup_timer_event,
    boot_timer_event,
    puyo_timer_event,
    idle_timer_event
};
void (*const mode_surface_event[16])(u8 p, u8 v, u8 x, u8 y) = {
    performance_surface_event,
    ableton_surface_event,
    note_surface_event,
    drum_surface_event,
    fader_surface_event,
    programmer_surface_event,
    piano_surface_event,
    chord_surface_event,
    custom_surface_event,
    text_surface_event,
    scale_setup_surface_event,
    editor_surface_event,
    setup_surface_event,
    boot_surface_event,
    puyo_surface_event,
    idle_surface_event
};
void (*const mode_midi_event[16])(u8 port, u8 t, u8 ch, u8 p, u8 v) = {
    performance_midi_event,
    ableton_midi_event,
    note_midi_event,
    drum_midi_event,
    fader_midi_event,
    programmer_midi_event,
    piano_midi_event,
    chord_midi_event,
    custom_midi_event,
    text_midi_event,
    scale_setup_midi_event,
    editor_midi_event,
    setup_midi_event,
    boot_midi_event,
    puyo_midi_event,
    idle_midi_event
};
void (*const mode_aftertouch_event[16])(u8 v) = {
    performance_aftertouch_event,
    ableton_aftertouch_event,
    note_aftertouch_event,
    drum_aftertouch_event,
    fader_aftertouch_event,
    programmer_aftertouch_event,
    piano_aftertouch_event,
    chord_aftertouch_event,
    custom_aftertouch_event,
    text_aftertouch_event,
    scale_setup_aftertouch_event,
    editor_aftertouch_event,
    setup_aftertouch_event,
    boot_aftertouch_event,
    puyo_aftertouch_event,
    idle_aftertouch_event
};
void (*const mode_poly_event[16])(u8 p, u8 v) = {
    performance_poly_event,
    ableton_poly_event,
    note_poly_event,
    drum_poly_event,
    fader_poly_event,
    programmer_poly_event,
    piano_poly_event,
    chord_poly_event,
    custom_poly_event,
    text_poly_event,
    scale_setup_poly_event,
    editor_poly_event,
    setup_poly_event,
    boot_poly_event,
    puyo_poly_event,
    idle_poly_event
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
