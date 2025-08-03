// SPDX-License-Identifier: GPL-3.0-or-later
#include "cell.h"

#include <stdio.h>

#include "ansi.h"

bool is_apple(cell_t cell) {
    return cell & SNAKE_APPLE;
}

bool is_head(cell_t cell) {
    return cell & SNAKE_HEAD;
}

bool is_snake(cell_t cell) {
    return cell & SNAKE_EXIST;
}

bool is_tail(cell_t cell) {
    return cell & SNAKE_TAIL;
}

cell_t get_dir(cell_t cell) {
    return cell & SNAKE_DIR_MASK;
}

void set_apple(cell_t *cell, bool apple) {
    *cell = (*cell & ~SNAKE_APPLE) | (apple ? SNAKE_APPLE : 0);
}

void set_head(cell_t *cell, bool head) {
    *cell = (*cell & ~SNAKE_HEAD) | (head ? SNAKE_HEAD : 0);
}

void set_snake(cell_t *cell, bool snake) {
    *cell = (*cell & ~SNAKE_EXIST) | (snake ? SNAKE_EXIST : 0);
}

void set_tail(cell_t *cell, bool tail) {
    *cell = (*cell & ~SNAKE_TAIL) | (tail ? SNAKE_TAIL : 0);
}

void set_dir(cell_t *cell, cell_t dir) {
    *cell = (*cell & ~SNAKE_DIR_MASK) | (dir & SNAKE_DIR_MASK);
}

void print_cell(cell_t *cell) {
    if (*cell == SNAKE_EMPTY) {
        printf(HBLK "·" CRESET);
    } else if (*cell == (SNAKE_EXIST | SNAKE_HEAD | SNAKE_TAIL)) {
        printf(BHGRN "&" CRESET);
    } else if (*cell == (SNAKE_EXIST | SNAKE_HEAD)) {
        printf(BHGRN "ö" CRESET);
    } else if ((*cell & ~SNAKE_DIR_MASK & ~SNAKE_TAIL) == SNAKE_EXIST) {
        cell_t dir = get_dir(*cell);
        if (dir & (SNAKE_DIR_L | SNAKE_DIR_R)) {
            printf(BHGRN "~" CRESET);
        } else {
            printf(BHGRN "s" CRESET);
        }
    } else if (*cell == SNAKE_APPLE) {
        printf(BHRED "ò" CRESET);
    } else {
        printf(RED "E" CRESET);
    }
}
