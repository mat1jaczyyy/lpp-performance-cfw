#ifndef SEND_H
#define SEND_H

#include "app.h"

#include "flash/settings.h"

void send_midi(u8 port, u8 t, u8 p, u8 v);
void send_sysex(u8 port, const u8* d, u16 l);

#endif
