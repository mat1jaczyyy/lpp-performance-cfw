#ifndef TEMPO_H
#define TEMPO_H

#include "app.h"

#include "modes/mode.h"

u32 tempo_timer;

void tempo_start();
void tempo_midi();
void tempo_stop();
void tempo_tick();

#endif
