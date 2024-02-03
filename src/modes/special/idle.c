#include "modes/special/idle.h"

u16 idle_rng_state;

u8 idle_index = 2;
s32 idle_timer = 0;

u8 idle_return;

struct idle_light {
    s32 start;
    u8 x;
    u8 y;
    u8 r:1;
    u8 g:1;
    u8 b:1;
    s8 step;
};

struct idle_light idle_effects[3];

u16 idle_rng() {
    idle_rng_state ^= idle_rng_state << 5;
    idle_rng_state ^= idle_rng_state >> 7;
    idle_rng_state += 28383;

    return idle_rng_state;
}

u8 idle_interp(u8 c, u16 i) {
    if (c) {
        if (i <= 63) return 63;
        else if (i <= 126) return 126 - i;
        else return 128;
    } else {
        if (i <= 63) return 63 - i;
        else if (i <= 126) return 0;
        else return 128;
    }
}

u8 idle_validate(u8 x, u8 y, u8 p) {
    return 0 <= x && x <= 9 && 0 <= y && y <= 9 && p != 0 && p != 9 && p != 90 && p != 99;
}

void idle_init() {
    idle_rng_state = global_timer & 0xFFFF;

    for (u8 i = 0; i < 3; i++)
        idle_effects[i].start = -1;
}

void idle_timer_event() {
    ++idle_timer;

    if (idle_timer % 360 == 0) {
        idle_index = (idle_index + 1) % 3;

        idle_effects[idle_index].start = idle_timer;
        idle_effects[idle_index].x = (idle_rng() & 7) + 1;
        idle_effects[idle_index].y = (idle_rng() & 7) + 1;

        do {
            idle_effects[idle_index].r = idle_rng() & 1;
            idle_effects[idle_index].g = idle_rng() & 1;
            idle_effects[idle_index].b = idle_rng() & 1;
        } while (idle_effects[idle_index].r == 0 && idle_effects[idle_index].g == 0 && idle_effects[idle_index].b == 0);

        idle_effects[idle_index].step = -1;
    }

    if (idle_timer % 60 == 0) {
        for (u8 i = 0; i < 3; i++) {
            if (idle_effects[i].start != -1 && idle_effects[i].step < 8) {
                idle_effects[i].step++;
            }
        }
    }

    if (idle_timer % 2 == 0) {
        for (u8 i = 0; i < 3; i++) {
            if (idle_effects[i].start != -1) {
                for (s8 j = idle_effects[i].step; j >= 0; j--) {
                    u16 t = (idle_timer - idle_effects[i].start - j * 60) / 2;

                    u8 r = idle_interp(idle_effects[i].r, t);
                    u8 g = idle_interp(idle_effects[i].g, t);
                    u8 b = idle_interp(idle_effects[i].b, t);

                    if (r != 128 || g != 128 || b != 128) {
                        u8 x, y, p;

                        x = idle_effects[i].x + j;
                        y = idle_effects[i].y;
                        p = x * 10 + y;

                        if (idle_validate(x, y, p))
                            rgb_led(p, r, g, b);

                        x = idle_effects[i].x - j;
                        p = x * 10 + y;

                        if (idle_validate(x, y, p))
                            rgb_led(p, r, g, b);

                        x = idle_effects[i].x;
                        y = idle_effects[i].y + j;
                        p = x * 10 + y;

                        if (idle_validate(x, y, p))
                            rgb_led(p, r, g, b);

                        y = idle_effects[i].y - j;
                        p = x * 10 + y;

                        if (idle_validate(x, y, p))
                            rgb_led(p, r, g, b);
                    }
                }
            }
        }
    }
}

void idle_surface_event(u8 p, u8 v, u8 x, u8 y) {}

void idle_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {}

void idle_aftertouch_event(u8 v) {}

void idle_poly_event(u8 p, u8 v) {}