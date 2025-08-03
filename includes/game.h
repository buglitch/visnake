// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#include "cell.h"
#include "grid.h"
#include "term.h"

struct game {
    cell_t dir;
    struct grid *grid;
    struct keys keys;
    struct keys keys_virtual;
    struct pos pos_head;
    struct pos pos_tail;
    uint8_t delay;
    uint8_t score;
    uint8_t status;
    uint8_t tick;
    uint8_t tick_last;
};

uint8_t run_game(void);

void next_frame(struct game *game);

void dump_game(struct game *game);
void redraw_game(struct game *game);

#endif /* !GAME_H */
