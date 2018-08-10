#ifndef CHALLENGE_H
#define CHALLENGE_H

#include "app_defs.h"

#define challenge_speed 9 // The higher this value is, the more prone the FW is for crashes for whatever reason. Speeds over 32 can't be handled by the buffer at all
#define challenge_max 0x4000

u8 challenge_do;
u16 challenge_counter;

void challenge_timer_event();

#endif