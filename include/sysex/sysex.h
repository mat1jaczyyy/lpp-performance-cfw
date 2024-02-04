#ifndef SYSEX_H
#define SYSEX_H

#include "app.h"

#include "modes/mode.h"
#include <string.h>

#define __syx_declare(x, l) u8 syx_ ## x[l]
#define syx_declare(x) __syx_declare(x,)
#define syx_declare_len(x, l) __syx_declare(x, l)

#define __syx_match(d, x) (!memcmp(d, syx_ ## x, sizeof(syx_ ## x)))
#define syx_match_offset(o, x) __syx_match(d + (o), x)
#define syx_match(x) __syx_match(d, x)

#define syx_resp_cpy(x) memcpy(syx_response_buffer + 1, syx_ ## x, sizeof(syx_ ## x));
#define syx_send(p, l) hal_send_sysex(p, syx_response_buffer, l)

syx_declare_len(response_buffer, 319);

const syx_declare_len(novation_header, 5);
const syx_declare_len(palette_header, 6);
const syx_declare_len(custom_header, 6);

#define uploading_export 0x23

void handle_sysex(u8 port, u8* d, u16 l);

#endif
