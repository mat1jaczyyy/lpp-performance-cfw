#include "aftertouch/aftertouch.h"

u8 aftertouch_screen[64] = {};
u8 aftertouch_current = 0;

u8 aftertouch_update(u8 p, u8 v) {
    u8 x = p / 10;
    u8 y = p % 10;

    if (x < 1 || 8 < x || y < 1 || 8 < y) return -1;

    p = (x - 1) * 8 + y - 1;

    aftertouch_screen[p] = v;

    int v_max = 0;
    for (int i = 0; i < 64; i++) {
        if (aftertouch_screen[i] > v_max)
            v_max = aftertouch_screen[i];
    }

    int ret = (v_max != aftertouch_current)? v_max : -1;
    aftertouch_current = v_max;

    return ret;
}

void aftertouch_send(u8 port, u8 t, u8 v) {
    u8 data[2] = {t, v};

	send_sysex(port, &data[0], 2);
}