#include "aftertouch/poly.h"

void poly_send(u8 port, u8 t, u8 p, u8 v) {
    u8 data[3] = {t, p, v};
	hal_send_sysex(port, &data[0], 3);
}