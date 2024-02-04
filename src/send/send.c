#include "send/send.h"

void send_midi(u8 port, u8 t, u8 p, u8 v) {
    hal_send_midi(port, t, p, v);

    if (settings.mode[mode_default].direct_enabled && port == USBSTANDALONE)
        hal_send_midi(DINMIDI, t, p, v);
}

void send_sysex(u8 port, const u8* d, u16 l) {
    hal_send_sysex(port, d, l);

    if (settings.mode[mode_default].direct_enabled && port == USBSTANDALONE)
        hal_send_sysex(DINMIDI, d, l);
}
