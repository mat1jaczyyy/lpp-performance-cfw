#ifndef AFTERTOUCH_H
#define AFTERTOUCH_H

#include "app.h"
#include "send/send.h"

u8 aftertouch_update(u8 p, u8 v);
void aftertouch_send(u8 port, u8 t, u8 v);

#endif