// SPDX-License-Identifier: GPL-3.0-or-later
#include "game.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "ansi.h"
#include "ctrl.h"

static inline uint8_t declen(uint8_t n);
static inline void set_delay(struct game *game);

uint8_t run_game(void) {
    srand(time(NULL));
    set_term(true);

    struct game game;
    game.delay = 40;
    game.dir = 0;
    game.grid = make_grid(18, 15);
    game.keys = (struct keys){ 0, ' ' };
    game.keys_virtual = (struct keys){ 0, '@' };
    game.score = 0;
    game.status = 0;
    game.tick = 0;
    game.tick_last = 0;

    *get_random_empty_cell(game.grid) = SNAKE_EXIST | SNAKE_HEAD | SNAKE_TAIL;
    *get_random_empty_cell(game.grid) = SNAKE_APPLE;

    game.pos_head = find_cell(game.grid, SNAKE_EXIST | SNAKE_HEAD);
    game.pos_tail = game.pos_head;

    while (true) {
        set_delay(&game);

        update_dir(&game);

        if (game.keys.motion != 'q')
            next_frame(&game);

        if (game.status || (game.keys_virtual.count == 0) ||
            game.keys.count <= 1)
            redraw_game(&game);
        if (game.status || game.keys.motion == 'q')
            break;

        game.tick = (game.tick + 1) % 120;
    }
    set_term(false);
    dump_game(&game);
    game.grid = destroy_grid(game.grid);
    return game.status;
}

void next_frame(struct game *game) {
    game->status = 0;
    if (game->dir &&
        (!((abs(game->tick - game->tick_last) % 120) % game->delay) ||
         (game->keys.motion != ' ' &&
          (game->keys.motion < 'A' || game->keys.motion > 'D')))) {
        game->tick_last = game->tick;

        cell_t *tail = get_cell(game->grid, game->pos_tail);
        cell_t *head = get_cell(game->grid, game->pos_head);

        struct pos pos_next = game->pos_head;

        if (game->dir == SNAKE_DIR_L)
            --pos_next.x;
        else if (game->dir == SNAKE_DIR_D)
            ++pos_next.y;
        else if (game->dir == SNAKE_DIR_U)
            --pos_next.y;
        else if (game->dir == SNAKE_DIR_R)
            ++pos_next.x;

        cell_t *head_next = get_cell(game->grid, pos_next);

        if (head && head_next) {
            bool ate = is_apple(*head_next);

            if (is_snake(*head_next) && !(is_tail(*head_next) && !ate)) {
                game->status = 1;
                return;
            }

            set_head(head, false);
            set_dir(head, game->dir);
            game->pos_head = pos_next;
            set_head(head_next, true);
            set_snake(head_next, true);

            if (ate) {
                ++game->score;
                if (game->score < game->grid->width * game->grid->height - 1)
                    *get_random_empty_cell(game->grid) = SNAKE_APPLE;
                else
                    game->status = 2;
                set_apple(head_next, false);
            } else {
                struct pos pos_new_tail = game->pos_tail;
                cell_t tail_dir = get_dir(*tail);

                if (tail_dir == SNAKE_DIR_L)
                    --pos_new_tail.x;
                else if (tail_dir == SNAKE_DIR_D)
                    ++pos_new_tail.y;
                else if (tail_dir == SNAKE_DIR_U)
                    --pos_new_tail.y;
                else if (tail_dir == SNAKE_DIR_R)
                    ++pos_new_tail.x;

                if (head_next != tail)
                    set_snake(tail, false);
                set_tail(tail, false);
                set_dir(tail, 0);
                game->pos_tail = pos_new_tail;
                cell_t *new_tail = get_cell(game->grid, game->pos_tail);
                set_tail(new_tail, true);
            }
        } else {
            game->status = 1;
            return;
        }
    }
}

void dump_game(struct game *game) {
    uint8_t i;
    struct pos pos;
    setvbuf(stdout, NULL, _IOFBF, BUFSIZ);

    printf("~   ");
    for (i = 0; i < game->grid->width; ++i) {
        if (i == game->pos_head.x)
            printf(BHYEL "%2u" CRESET, i + 1);
        else
            printf("  ");
    }
    puts("");

    printf("~   ");
    for (i = 0; i < game->grid->width; ++i) {
        if (i == game->pos_head.x) {
            printf("  ");
        } else {
            uint8_t hdiff = abs((int)game->pos_head.x - (int)i);
            if (hdiff < 10)
                printf(BHYEL "%2u" CRESET, hdiff);
            else if (hdiff < 20)
                printf(YEL "¹" CRESET BHYEL "%u" CRESET, hdiff - 10);
            else if (hdiff < 30)
                printf(YEL "²" CRESET BHYEL "%u" CRESET, hdiff - 20);
            else
                printf(YEL "³" CRESET BHYEL "%u" CRESET, hdiff - 30);
        }
    }
    puts("");

    printf("~   ┌");
    for (i = 0; i < (game->grid->width << 1) - 1; ++i)
        printf("─");
    puts("┐");

    for (pos.y = 0; pos.y < game->grid->height; ++pos.y) {
        if (pos.y == game->pos_head.y)
            printf(BHYEL "%2u " CRESET " │", pos.y + 1);
        else
            printf(BHYEL " %2u" CRESET " │",
                   abs((int)game->pos_head.y - (int)pos.y));
        for (pos.x = 0; pos.x < game->grid->width; ++pos.x) {
            print_cell(get_cell(game->grid, pos));
            if (pos.x != game->grid->width - 1)
                printf(" ");
            else
                printf("│");
        }
        puts("");
    }

    printf("~   └");
    for (i = 0; i < (game->grid->width << 1) - 1; ++i)
        printf("─");
    puts("┘");

    if (game->score != 0 && !(game->score % 10) && game->score <= 100) {
        printf("[" BHRED "ò" CRESET " score: %u] [+]", game->score);
        printf("%*s", (game->grid->width >> 1) - declen(game->score), "");
    } else {
        printf("[" BHRED "ò" CRESET " score: %u]", game->score);
        printf("%*s", (game->grid->width >> 1) + 4 - declen(game->score), "");
    }
    if (game->keys.motion != '@') {
        if (game->keys.motion >= 'A' && game->keys.motion <= 'D')
            printf(" +");
        else if (!game->keys.count)
            printf(" %c", game->keys.motion);
        else
            printf("%u%c", game->keys.count, game->keys.motion);
    } else {
        if (!game->keys_virtual.count)
            printf(" %c", game->keys_virtual.motion);
        else
            printf("%u%c", game->keys_virtual.count, game->keys_virtual.motion);
    }
    printf("%*s", (game->grid->width >> 1), "");
    printf("%2u,%-2u", game->pos_head.y + 1, game->pos_head.x + 1);

    fflush(stdout);
}

void redraw_game(struct game *game) {
    if (!((abs(game->tick - game->tick_last) % 120) % game->delay))
        printf(CLEAR);
    printf(CUP00);
    dump_game(game);
}

static inline uint8_t declen(uint8_t n) {
    if (n >= 10)
        return 2;
    else if (n >= 100)
        return 3;
    return 1;
}

static inline void set_delay(struct game *game) {
    switch (game->score) {
    case 10:
        game->delay = 30;
        break;
    case 20:
        game->delay = 24;
        break;
    case 30:
        game->delay = 20;
        break;
    case 40:
        game->delay = 15;
        break;
    case 50:
        game->delay = 12;
        break;
    case 60:
        game->delay = 10;
        break;
    case 70:
        game->delay = 8;
        break;
    case 80:
        game->delay = 6;
        break;
    case 90:
        game->delay = 5;
        break;
    case 100:
        game->delay = 4;
        break;
    default:
        break;
    }
}
