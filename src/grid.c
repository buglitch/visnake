// SPDX-License-Identifier: GPL-3.0-or-later
#include "grid.h"

#include <stdio.h>
#include <stdlib.h>

bool pos_cmp(struct pos pos1, struct pos pos2) {
    return (pos1.x == pos2.x && pos1.y == pos2.y);
}

bool pos_in_grid(struct grid *grid, struct pos pos) {
    return (pos.x < grid->width && pos.y < grid->height);
}

cell_t *get_cell(struct grid *grid, struct pos pos) {
    if (pos.x < grid->width && pos.y < grid->height)
        return &grid->cells[pos.x + pos.y * grid->width];
    return NULL;
}

inline cell_t *get_random_cell(struct grid *grid) {
    return &grid->cells[rand() % (grid->height * grid->width)];
}

cell_t *get_random_empty_cell(struct grid *grid) {
    cell_t *cell = get_random_cell(grid);
    while (*cell != SNAKE_EMPTY)
        cell = get_random_cell(grid);
    return cell;
}

struct pos find_cell(struct grid *grid, cell_t type) {
    struct pos pos;
    for (pos.y = 0; pos.y < grid->height; ++pos.y) {
        for (pos.x = 0; pos.x < grid->width; ++pos.x) {
            cell_t *cell = get_cell(grid, pos);
            if ((*cell & type) == type)
                return pos;
        }
    }
    return pos;
}

struct grid *make_grid(uint8_t width, uint8_t height) {
    struct grid *grid = malloc(sizeof(*grid));
    if (!grid) {
        perror(__func__);
        exit(EXIT_FAILURE);
    }
    grid->cells = calloc(width * height, sizeof(*grid->cells));
    if (!grid->cells) {
        free(grid);
        perror(__func__);
        exit(EXIT_FAILURE);
    }
    grid->width = width;
    grid->height = height;
    return grid;
}

void *destroy_grid(struct grid *grid) {
    if (grid) {
        if (grid->cells)
            free(grid->cells);
        free(grid);
    }
    return NULL;
}
