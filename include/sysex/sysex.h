#ifndef SYSEX_H
#define SYSEX_H

#include "app.h"
#include "sysex/messages.h"

#include "string.h"
#include "modes/mode.h"

u8 custom_modifying;

void handle_sysex(u8 port, u8 * d, u16 l);

#endif