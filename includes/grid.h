// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef GRID_H
#define GRID_H

#include <stdbool.h>
#include <stdint.h>

#include "cell.h"

struct grid {
    cell_t *cells;
    uint8_t width;
    uint8_t height;
};

struct pos {
    uint8_t x;
    uint8_t y;
};

bool pos_cmp(struct pos pos1, struct pos pos2);
bool pos_in_grid(struct grid *grid, struct pos pos);

cell_t *get_cell(struct grid *grid, struct pos pos);
cell_t *get_random_cell(struct grid *grid);
cell_t *get_random_empty_cell(struct grid *grid);

struct pos find_cell(struct grid *grid, cell_t type);

struct grid *make_grid(uint8_t width, uint8_t height);
void *destroy_grid(struct grid *grid);

#endif /* !GRID_H */
