#ifndef TEMPO_H
#define TEMPO_H

#include "app_defs.h"

#define tempo_message_counter_max 96

u8 tempo_listen, tempo_message_counter;
u32 tempo_counter, tempo_timer, tempo_bar;

void tempo_start();
void tempo_midi();
void tempo_stop();
void tempo_tick();

#endif