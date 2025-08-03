// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef CELL_H
#define CELL_H

#include <stdbool.h>
#include <stdint.h>

#define SNAKE_EMPTY (0x0)
#define SNAKE_EXIST (0x1)
#define SNAKE_DIR_R (0x1 << 1)
#define SNAKE_DIR_U (0x1 << 2)
#define SNAKE_DIR_D (0x1 << 3)
#define SNAKE_DIR_L (0x1 << 4)
#define SNAKE_HEAD (0x1 << 5)
#define SNAKE_TAIL (0x1 << 6)
#define SNAKE_APPLE (0x1 << 7)

#define SNAKE_DIR_MASK (SNAKE_DIR_R | SNAKE_DIR_U | SNAKE_DIR_D | SNAKE_DIR_L)

typedef uint8_t cell_t;

bool is_apple(cell_t cell);
bool is_head(cell_t cell);
bool is_snake(cell_t cell);
bool is_tail(cell_t cell);

cell_t get_dir(cell_t cell);

void set_apple(cell_t *cell, bool apple);
void set_head(cell_t *cell, bool head);
void set_snake(cell_t *cell, bool snake);
void set_tail(cell_t *cell, bool tail);

void set_dir(cell_t *cell, cell_t dir);

void print_cell(cell_t *cell);

#endif /* !CELL_H */
