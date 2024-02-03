#ifndef CUSTOM_H
#define CUSTOM_H

#include "app.h"
#include "modes/mode.h"
#include "send/send.h"
#include "sysex/sysex.h"

#include "aftertouch/aftertouch.h"

#define mode_custom_r 63
#define mode_custom_g 63
#define mode_custom_b 63

void custom_upload_start(u8 i);
void custom_upload_push(const u8* d);
void custom_upload_end();

void custom_slot_change(u8 t);

void custom_init();
void custom_timer_event();
void custom_surface_event(u8 p, u8 v, u8 x, u8 y);
void custom_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v);
void custom_aftertouch_event(u8 v);
void custom_poly_event(u8 p, u8 v);

#endif