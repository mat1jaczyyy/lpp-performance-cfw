#include "modes/special/puyo.h"

#define puyo_drop_timeout 650
#define puyo_empty -1

#define puyo_drop_fast 7
#define puyo_drop_slow 1

#define puyo_preparing 0x0
#define puyo_placing 0x1
#define puyo_falling 0x2
#define puyo_chaining 0x3
#define puyo_dead 0x4

#define puyo_x 7

#define puyo_chain_r 63
#define puyo_chain_g 47
#define puyo_chain_b 0

#define puyo_allclear 9

#define puyo_moveleft 6
#define puyo_moveright 8
#define puyo_drop 7
#define puyo_ccw 1
#define puyo_cw 2

#define puyo_dead_frametime 100
#define puyo_dead_r 7
#define puyo_dead_g 0
#define puyo_dead_b 0

u8 puyo_dead_counter = 0;
u8 puyo_dead_frame = 0;

#define puyo_automove_start 190
#define puyo_automove_end 250
u8 puyo_automove_direction = 0;
u8 puyo_automove_counter = 0;
u8 puyo_automove_active = 0;

const u8 puyo_colors[4][3] = {
    {63, 0, 0},
    {63, 63, 0},
    {0, 63, 0},
    {0, 0, 63}
};

struct puyo_piece {
    u8 top: 2;
    u8 bottom: 2;
};

u16 puyo_rng_state;
u8 puyo_state;

struct puyo_piece puyo_pieces[3] = {};

struct puyo_object {
    s8 x; // x pos of bottom puyo
    s8 y; // y pos of bottom puyo
    s8 r; // degrees rotation of top puyo
    s8 d; // double rotate flag
};

struct puyo_object puyo_current;

u16 puyo_drop_counter = 0;
u8 puyo_drop_increment = 1;
u8 puyo_just_dropped = 0;

s8 puyo_board[9][8];
u8 puyo_chains[8][8];

u8 puyo_chains_length[65];
u8 puyo_chain_combo = 0;
u16 puyo_chaining_timer = 0;

#define puyo_fall_timeout 100
u16 puyo_fall_counter = 0;

#define puyo_pop 4

u16 puyo_rng() {
    puyo_rng_state ^= puyo_rng_state << 5;
    puyo_rng_state ^= puyo_rng_state >> 7;
    puyo_rng_state += 28383;

    return puyo_rng_state;
}

s8 puyo_top_offset(u8 r, u8 f) {
    s8 x = 0;
    s8 y = 0;
    
    switch (r) {
        case 0: // 0 degrees
            x = 1;
            break;
        
        case 1: // 90 degrees
            y = 1;
            break;
        
        case 2: // 180 degrees
            x = -1;
            break;
        
        case 3: // 270 degrees
            y = -1;
            break;
    }

    switch (f) {
        case 0: // XY format
            return (x * 10) + y;
            break;
        
        case 1: // X only
            return x;
            break;
        
        case 2: // Y only
            return y;
            break;
    }

    return 0;
}

void puyo_make_piece() {
    for (u8 i = 0; i < 2; i++) puyo_pieces[i] = puyo_pieces[i + 1];
    puyo_pieces[2].top = puyo_rng() % 4;
    puyo_pieces[2].bottom = puyo_rng() % 4;
}

void puyo_draw_next() {
    rgb_led(89, puyo_colors[puyo_pieces[1].top][0] >> 2, puyo_colors[puyo_pieces[1].top][1] >> 2, puyo_colors[puyo_pieces[1].top][2] >> 2);
    rgb_led(79, puyo_colors[puyo_pieces[1].bottom][0] >> 2, puyo_colors[puyo_pieces[1].bottom][1] >> 2, puyo_colors[puyo_pieces[1].bottom][2] >> 2);
    rgb_led(69, puyo_colors[puyo_pieces[2].top][0] >> 4, puyo_colors[puyo_pieces[2].top][1] >> 4, puyo_colors[puyo_pieces[2].top][2] >> 4);
    rgb_led(59, puyo_colors[puyo_pieces[2].bottom][0] >> 4, puyo_colors[puyo_pieces[2].bottom][1] >> 4, puyo_colors[puyo_pieces[2].bottom][2] >> 4);
}

void puyo_draw_board() {
    for (u8 x = 0; x < 8; x++) {
        for (u8 y = 0; y < 8; y++) {
            if (puyo_board[x][y] == puyo_empty) {
                rgb_led((x + 1) * 10 + (y + 1), 0, 0, 0);
            } else {
                rgb_led((x + 1) * 10 + (y + 1), puyo_colors[puyo_board[x][y]][0], puyo_colors[puyo_board[x][y]][1], puyo_colors[puyo_board[x][y]][2]);
            }
        }
    }
    if (puyo_board[7][3] == puyo_empty) {
        pulse_led(84, puyo_x);
    }
}

void puyo_draw_current() {
    if (puyo_current.x <= 7) rgb_led((puyo_current.x + 1) * 10 + (puyo_current.y + 1), puyo_colors[puyo_pieces[0].bottom][0], puyo_colors[puyo_pieces[0].bottom][1], puyo_colors[puyo_pieces[0].bottom][2]);
    
    s8 top_offset = puyo_top_offset(puyo_current.r, 0);
    if (top_offset < 1 || puyo_current.x != 7) {
        rgb_led((puyo_current.x + 1) * 10 + (puyo_current.y + 1) + puyo_top_offset(puyo_current.r, 0), puyo_colors[puyo_pieces[0].top][0], puyo_colors[puyo_pieces[0].top][1], puyo_colors[puyo_pieces[0].top][2]);
    }
}

void puyo_draw_ghost() {
    s8 x;
    switch (puyo_current.r) {
        case 0: // 0 degrees
            for (x = 8; -1 < x; x--) {
                if (puyo_board[x][puyo_current.y] != puyo_empty) break;
            }
            if (++x < 8) rgb_led((x + 1) * 10 + (puyo_current.y + 1), puyo_colors[puyo_pieces[0].bottom][0] >> 3, puyo_colors[puyo_pieces[0].bottom][1] >> 3, puyo_colors[puyo_pieces[0].bottom][2] >> 3);
            if (++x < 7) rgb_led((x + 1) * 10 + (puyo_current.y + 1), puyo_colors[puyo_pieces[0].top][0] >> 3, puyo_colors[puyo_pieces[0].top][1] >> 3, puyo_colors[puyo_pieces[0].top][2] >> 3);
            break;

        case 1: // 90 degrees
            for (x = 8; -1 < x; x--) {
                if (puyo_board[x][puyo_current.y] != puyo_empty) break;
            }
            if (++x < 8) rgb_led((x + 1) * 10 + (puyo_current.y + 1), puyo_colors[puyo_pieces[0].bottom][0] >> 3, puyo_colors[puyo_pieces[0].bottom][1] >> 3, puyo_colors[puyo_pieces[0].bottom][2] >> 3);

            for (x = 8; -1 < x; x--) {
                if (puyo_board[x][puyo_current.y + 1] != puyo_empty) break;
            }
            if (++x < 8) rgb_led((x + 1) * 10 + (puyo_current.y + 2), puyo_colors[puyo_pieces[0].top][0] >> 3, puyo_colors[puyo_pieces[0].top][1] >> 3, puyo_colors[puyo_pieces[0].top][2] >> 3);
            break;
        
        case 2: // 180 degrees
            for (x = 8; -1 < x; x--) {
                if (puyo_board[x][puyo_current.y] != puyo_empty) break;
            }
            if (++x < 8) rgb_led((x + 1) * 10 + (puyo_current.y + 1), puyo_colors[puyo_pieces[0].top][0] >> 3, puyo_colors[puyo_pieces[0].top][1] >> 3, puyo_colors[puyo_pieces[0].top][2] >> 3);
            if (++x < 8) rgb_led((x + 1) * 10 + (puyo_current.y + 1), puyo_colors[puyo_pieces[0].bottom][0] >> 3, puyo_colors[puyo_pieces[0].bottom][1] >> 3, puyo_colors[puyo_pieces[0].bottom][2] >> 3);
            break;
        
        case 3: // 270 degrees
            for (x = 8; -1 < x; x--) {
                if (puyo_board[x][puyo_current.y] != puyo_empty) break;
            }
            if (++x < 8) rgb_led((x + 1) * 10 + (puyo_current.y + 1), puyo_colors[puyo_pieces[0].bottom][0] >> 3, puyo_colors[puyo_pieces[0].bottom][1] >> 3, puyo_colors[puyo_pieces[0].bottom][2] >> 3);

            for (x = 8; -1 < x; x--) {
                if (puyo_board[x][puyo_current.y - 1] != puyo_empty) break;
            }
            if (++x < 8) rgb_led((x + 1) * 10 + (puyo_current.y), puyo_colors[puyo_pieces[0].top][0] >> 3, puyo_colors[puyo_pieces[0].top][1] >> 3, puyo_colors[puyo_pieces[0].top][2] >> 3);
            break;
    }
}

void puyo_draw_chains(u8 b) {
    for (u8 x = 0; x < 8; x++) {
        for (u8 y = 0; y < 8; y++) {
            if (puyo_chains[x][y] && puyo_chains_length[puyo_chains[x][y]] >= puyo_pop) {
                rgb_led((x + 1) * 10 + (y + 1), puyo_colors[puyo_board[x][y]][0] * b, puyo_colors[puyo_board[x][y]][1] * b, puyo_colors[puyo_board[x][y]][2] * b);
            }
        }
    }
}

void puyo_remove_chains() {
    for (u8 x = 0; x < 8; x++) {
        for (u8 y = 0; y < 8; y++) {
            if (puyo_chains[x][y] && puyo_chains_length[puyo_chains[x][y]] >= puyo_pop) {
                puyo_board[x][y] = -1;
            }
        }
    }
}

void puyo_spawn() {
    puyo_make_piece();

    puyo_current.x = 7;
    puyo_current.y = 3;
    puyo_current.r = 0;
    puyo_current.d = 0;

    if (puyo_board[7][3] != puyo_empty) {
        puyo_state = puyo_dead;
    }
}

void puyo_move(u8 a) {
    switch (a) {
        case 0: // Move left
            switch (puyo_current.r) {
                case 0: // 0 degrees
                case 1: // 90 degrees
                    if (puyo_current.y == 0) return;
                    if (puyo_board[puyo_current.x][puyo_current.y - 1] != puyo_empty) return;
                    break;

                case 2: // 180 degrees
                    if (puyo_current.y == 0) return;
                    if (puyo_board[puyo_current.x - 1][puyo_current.y - 1] != puyo_empty) return;
                
                case 3: // 270 degrees
                    if (puyo_current.y == 1) return;
                    if (puyo_board[puyo_current.x][puyo_current.y - 2] != puyo_empty) return;
                    break;
            }
            puyo_current.y--;
            break;
        
        case 1: // Move right
            switch (puyo_current.r) {
                case 0: // 0 degrees
                case 3: // 270 degrees
                    if (puyo_current.y == 7) return;
                    if (puyo_board[puyo_current.x][puyo_current.y + 1] != puyo_empty) return;
                    break;

                case 2: // 180 degrees
                    if (puyo_current.y == 7) return;
                    if (puyo_board[puyo_current.x - 1][puyo_current.y + 1] != puyo_empty) return;
                    break;
                
                case 1: // 90 degrees
                    if (puyo_current.y == 6) return;
                    if (puyo_board[puyo_current.x][puyo_current.y + 2] != puyo_empty) return;
                    break;
            }
            puyo_current.y++;
            break;
    }
}

void puyo_rotate(u8 a) {
    u8 intended;
    
    a &= 1;
    switch (a) {
        case 0: // Rotate CCW
            intended = modulo(puyo_current.r - 1, 4);
            break;
        
        case 1: // Rotate CW
            intended = modulo(puyo_current.r + 1, 4);
            break;
    }

    // Double rotate
    u8 condl = (puyo_current.y == 0);
    if (!condl) condl = (puyo_board[puyo_current.x][puyo_current.y - 1] != puyo_empty);
    u8 condr = (puyo_current.y == 7);
    if (!condr) condr = (puyo_board[puyo_current.x][puyo_current.y + 1] != puyo_empty);

    if (condl && condr) {
        puyo_current.d++;

        if (puyo_current.d >= 2) {
            puyo_current.d = 0;
            puyo_current.r = modulo(puyo_current.r + 2, 4);

            // Floor kick check
            u8 cond = (puyo_current.x == 0);
            if (!cond) cond = (puyo_board[puyo_current.x - 1][puyo_current.y] != puyo_empty);
            if (puyo_current.r == 2 && cond) {
                puyo_current.x++;
            }
        }
        return;
    }
    puyo_current.d = 0;

    // Floor kick
    u8 cond = (puyo_current.x == 0);
    if (!cond) cond = (puyo_board[puyo_current.x - 1][puyo_current.y] != puyo_empty);
    if (((puyo_current.r == 1 && a == 1) || (puyo_current.r == 3 && a == 0)) && cond) {
        puyo_current.r = intended;
        puyo_current.x++;
        return;
    }

    // Wall kick left side
    if (((puyo_current.r == 0 && a == 0) || (puyo_current.r == 2 && a == 1)) && condl) {
        puyo_current.r = intended;
        puyo_current.y++;
        return;
    }

    // Wall kick right side
    if (((puyo_current.r == 0 && a == 1) || (puyo_current.r == 2 && a == 0)) && condr) {
        puyo_current.r = intended;
        puyo_current.y--;
        return;
    }

    puyo_current.r = intended;
}

void puyo_control(u8 p, u8 v) {
    v = (v)? 63 : 0;
    rgb_led(p, v, v, v);
}

void puyo_down() {
    puyo_current.x--;
    u8 dropped = 0;
    
    switch (puyo_current.r) {
        case 0: // 0 degrees
            dropped = (puyo_board[puyo_current.x][puyo_current.y] != puyo_empty || puyo_current.x < 0);
            if (dropped) {
                puyo_board[++puyo_current.x][puyo_current.y] = puyo_pieces[0].bottom;
                puyo_board[puyo_current.x + 1][puyo_current.y] = puyo_pieces[0].top;
            }
            break;

        case 1: // 90 degrees
            dropped = (puyo_board[puyo_current.x][puyo_current.y] != puyo_empty || puyo_board[puyo_current.x][puyo_current.y + 1] != puyo_empty || puyo_current.x < 0);
            if (dropped) {
                puyo_board[++puyo_current.x][puyo_current.y] = puyo_pieces[0].bottom;
                puyo_board[puyo_current.x][puyo_current.y + 1] = puyo_pieces[0].top;
            }
            break;

        case 2: // 180 degrees
            dropped = (puyo_board[puyo_current.x - 1][puyo_current.y] != puyo_empty || puyo_current.x < 1);
            if (dropped) {
                puyo_board[++puyo_current.x - 1][puyo_current.y] = puyo_pieces[0].top;
                puyo_board[puyo_current.x][puyo_current.y] = puyo_pieces[0].bottom;
            }
            break;

        case 3: // 270 degrees
            dropped = (puyo_board[puyo_current.x][puyo_current.y] != puyo_empty || puyo_board[puyo_current.x][puyo_current.y - 1] != puyo_empty || puyo_current.x < 0);
            if (dropped) {
                puyo_board[++puyo_current.x][puyo_current.y] = puyo_pieces[0].bottom;
                puyo_board[puyo_current.x][puyo_current.y - 1] = puyo_pieces[0].top;
            }
            break;
    }

    if (dropped) {
        puyo_just_dropped = 1;
        puyo_state = puyo_falling;
    }
}

u8 puyo_check_fall() {
    u8 ret = 0;
    
    for (u8 x = 0; x < 8; x++) {
        for (u8 y = 0; y < 8; y++) {
            if (puyo_board[x][y] == puyo_empty && puyo_board[x + 1][y] != puyo_empty) {
                puyo_board[x][y] = puyo_board[x + 1][y];
                puyo_board[x + 1][y] = puyo_empty;
                ret = 1;
            }
        }
    }

    u8 y;
    for (y = 0; y < 8; y++) {
        if (puyo_board[0][y] != puyo_empty) break;
    }
    if (y == 8) {
        for (u8 i = 92; i < 98; i++) {
            pulse_led(i, puyo_allclear);
        }
    }

    return ret;
}

u8 puyo_chain(u8 x, u8 y, u8 c, u8 id, u8 s) {
    puyo_chains[x][y] = id;

    if (x < 7) {
        if (puyo_board[x + 1][y] == c && !puyo_chains[x + 1][y]) s = puyo_chain(x + 1, y, c, id, ++s);
    }
    if (x > 0) {
        if (puyo_board[x - 1][y] == c && !puyo_chains[x - 1][y]) s = puyo_chain(x - 1, y, c, id, ++s);
    }
    if (y < 7) {
        if (puyo_board[x][y + 1] == c && !puyo_chains[x][y + 1]) s = puyo_chain(x, y + 1, c, id, ++s);
    }
    if (y > 0) {
        if (puyo_board[x][y - 1] == c && !puyo_chains[x][y - 1]) s = puyo_chain(x, y - 1, c, id, ++s);
    }

    return s;
}

u8 puyo_check_chain() {
    for (u8 x = 0; x < 8; x++) for (u8 y = 0; y < 8; y++) puyo_chains[x][y] = 0;
    for (u8 i = 0; i < 65; i++) puyo_chains_length[i] = 0;
    u8 chain_exists = 0;

    u8 id = 0;
    for (u8 x = 0; x < 8; x++) {
        for (u8 y = 0; y < 8; y++) {
            if (puyo_board[x][y] != puyo_empty && !puyo_chains[x][y]) {
                id++;
                puyo_chains_length[id] = puyo_chain(x, y, puyo_board[x][y], id, 1);
                chain_exists |= (puyo_chains_length[id] >= puyo_pop); 
            }
        }
    }

    if (puyo_just_dropped) {
        puyo_chain_combo = 0;
        puyo_just_dropped = 0;
    }
    if (chain_exists) {
        puyo_chain_combo++;
        for (u8 i = 92; i < 98; i++) { // Send All Clear
            rgb_led(i, 0, 0, 0);
        }
    }
    display_u8(puyo_chain_combo, 1, 0, puyo_chain_r, puyo_chain_g, puyo_chain_b);

    return chain_exists;
}

void puyo_init() {
    puyo_rng_state = global_timer & 0xFFFF;
    puyo_state = puyo_preparing;

    for (u8 x = 0; x < 9; x++) for (u8 y = 0; y < 8; y++) puyo_board[x][y] = -1;
    for (u8 x = 0; x < 8; x++) for (u8 y = 0; y < 8; y++) puyo_chains[x][y] = 0;

    puyo_dead_counter = 0;
    puyo_dead_frame = 0;

    puyo_automove_active = 0;
    puyo_drop_increment = puyo_drop_slow;

    puyo_make_piece();
    puyo_make_piece();

    puyo_draw_next();
    puyo_draw_board();
}

void puyo_timer_event() {
    if (puyo_automove_active) {
        if (++puyo_automove_counter >= puyo_automove_end) {
            if (puyo_state == puyo_placing) puyo_move(puyo_automove_direction);
            puyo_automove_counter = puyo_automove_start;
        }
    }
    switch (puyo_state) {
        case puyo_placing:
            puyo_drop_counter += puyo_drop_increment;
            
            if (puyo_drop_counter >= puyo_drop_timeout) {
                puyo_drop_counter = 0;

                puyo_down();
            }

            puyo_draw_next();
            puyo_draw_board();
            if (puyo_state != puyo_falling) {
                puyo_draw_ghost();
                puyo_draw_current();
            }
            break;
        
        case puyo_falling:
            if (++puyo_fall_counter > puyo_fall_timeout) {
                if (!puyo_check_fall()) {
                    if (puyo_check_chain()) {
                        puyo_chaining_timer = 0;
                        puyo_state = puyo_chaining;
                    } else {
                        puyo_state = puyo_placing;
                        puyo_spawn();
                    }
                } else {
                    puyo_draw_board();
                }
                puyo_fall_counter = 0;
            }
            break;
        
        case puyo_chaining:
            if (++puyo_chaining_timer < 1000) {
                puyo_draw_chains((puyo_chaining_timer / 80) % 2);
            } else {
                puyo_remove_chains();

                puyo_chaining_timer = 0;
                puyo_state = puyo_falling;
            }
            break;
        
        case puyo_dead:
            if (++puyo_dead_counter >= puyo_dead_frametime) {
                puyo_dead_counter = 0;
                if (puyo_dead_frame <= 9) {
                    for (u8 y = 0; y < 10; y++) {
                        rgb_led((puyo_dead_frame * 10) + y, puyo_dead_r, puyo_dead_g, puyo_dead_b);
                    }
                }
                puyo_dead_frame++;
            }
            if (puyo_dead_frame >= 15) mode_update(mode_setup);
            break;
    }
}

void puyo_surface_event(u8 p, u8 v, u8 x, u8 y) {
    if (p == 0 && v) {
        mode_update(mode_setup);
        return;
    }

    if (puyo_state == puyo_preparing && p == 94 && !v) {
        puyo_spawn();
        puyo_state = puyo_placing;

        rgb_led(p, 0, 0, 0);
        return;
    }

    if (puyo_state != puyo_dead) {
        switch (p) {
            case puyo_moveleft:
                if (v) {
                    if (puyo_state == puyo_placing) puyo_move(0);
                    puyo_automove_active = 1;
                    puyo_automove_direction = 0;
                    puyo_automove_counter = 0;
                } else if (puyo_automove_direction == 0 && puyo_automove_active == 1) {
                    puyo_automove_active = 0;
                }
                puyo_control(p, v);
                break;
            
            case puyo_moveright:
                if (v) {
                    if (puyo_state == puyo_placing) puyo_move(1);
                    puyo_automove_active = 1;
                    puyo_automove_direction = 1;
                    puyo_automove_counter = 0;
                } else if (puyo_automove_direction == 1 && puyo_automove_active == 1) {
                    puyo_automove_active = 0;
                }
                puyo_control(p, v);
                break;
            
            case puyo_drop:
                if (v) {
                    puyo_drop_increment = puyo_drop_fast;
                } else {
                    puyo_drop_increment = puyo_drop_slow;
                }
                puyo_control(p, v);
                break;
            
            case puyo_ccw:
                if (puyo_state == puyo_placing && v) puyo_rotate(0);
                puyo_control(p, v);
                break;
            
            case puyo_cw:
                if (puyo_state == puyo_placing && v) puyo_rotate(1);
                puyo_control(p, v);
                break;
        }
    }
}

void puyo_midi_event(u8 port, u8 t, u8 ch, u8 p, u8 v) {}

void puyo_aftertouch_event(u8 v) {}

void puyo_poly_event(u8 p, u8 v) {}