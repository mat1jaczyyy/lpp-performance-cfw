#ifndef MODE_H
#define MODE_H

#include "app_defs.h"

#include "led/led.h"
#include "flash/flash.h"

#define mode_performance 0
#include "modes/normal/performance.h"

#define mode_ableton 1
#include "modes/normal/ableton.h"

#define mode_note 2
#include "modes/normal/note.h"

#define mode_drum 3
#include "modes/normal/drum.h"

#define mode_fader 4
#include "modes/normal/fader.h"

#define mode_programmer 5
#include "modes/normal/programmer.h"

#define mode_normal 6

#define mode_text 6
#include "modes/special/text.h"

#define mode_boot 10
#include "modes/special/boot.h"

#define mode_setup 9
#include "modes/special/setup.h"

#define mode_editor 8
#include "modes/special/editor.h"

#define mode_scale_setup 7
#include "modes/special/scale.h"

void (*const mode_init[11])();
void (*const mode_timer_event[11])();
void (*const mode_surface_event[11])(u8 p, u8 v, u8 x, u8 y);
void (*const mode_midi_event[11])(u8 port, u8 t, u8 ch, u8 p, u8 v);

u8 mode, mode_default;

void mode_refresh();
void mode_update(u8 x);
void mode_default_update(u8 x);

#endif