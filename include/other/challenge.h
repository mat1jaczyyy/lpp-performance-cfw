#ifndef CHALLENGE_H
#define CHALLENGE_H

#include "app.h"

#include "sysex/messages.h"

u8 challenge_do;
u16 challenge_counter;

void challenge_timer_event();

#endif