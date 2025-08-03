// SPDX-License-Identifier: GPL-3.0-or-later
#include "ctrl.h"

inline void update_keys(struct keys *keys, useconds_t usec) {
    if (keys->motion != ' ') {
        keys->count = 0;
        keys->motion = ' ';
    }

    char c = get_key(usec);

    switch (c) {
    case '\x1B':
        c = get_key(0);
        c = get_key(0);
        if (c >= 'A' && c <= 'D') {
            keys->count = 0;
            keys->motion = c;
        }
        break;
    case '$':
    case '^':
    case '_':
    case 'H':
    case 'M': //TODO
    case 'L':
    case 'q':
        keys->count = 0;
        keys->motion = c;
        break;
    case 'G': //TODO
    case 'h':
    case 'j':
    case 'k':
    case 'l':
        keys->motion = c;
        break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        if (keys->count != 0 && keys->count < 10)
            keys->count = keys->count * 10 + c - '0';
        else
            keys->count = c - '0';
        keys->motion = ' ';
        break;
    default:
        break;
    }
}

void update_dir(struct game *game) {
    if (game->keys.count > 1 && game->keys.motion != ' ') {
        if (game->keys_virtual.motion == '@') {
            game->keys_virtual = game->keys;
        }
        game->keys.motion = '@';
        --game->keys.count;
    } else {
        switch (game->keys.motion) {
        case '^':
        case '_':
        case '$':
        case 'H':
        case 'L':
        case 'M':
            if (game->keys_virtual.motion == '@') {
                game->keys_virtual = game->keys;
                game->keys_virtual.count = 0;
                uint8_t half_grid_height = (game->grid->height - 1) >> 1;
                switch (game->keys.motion) {
                case '^':
                case '_':
                    game->keys.count = game->pos_head.x + 1;
                    break;
                case '$':
                    game->keys.count = game->grid->width - game->pos_head.x;
                    break;
                case 'H':
                    game->keys.count = game->pos_head.y + 1;
                    break;
                case 'L':
                    game->keys.count = game->grid->height - game->pos_head.y;
                    break;
                case 'M':
                    if (game->pos_head.y < half_grid_height + 1)
                        game->keys.count =
                            half_grid_height - game->pos_head.y + 1;
                    else if (game->pos_head.y > half_grid_height + 1)
                        game->keys.count =
                            game->pos_head.y - half_grid_height + 1;
                    break;
                default:
                    break;
                }
            }
            game->keys.motion = '@';
            if (game->keys.count)
                if (!--game->keys.count)
                    update_keys(&game->keys, 0);
            break;
        case ' ':
            if (game->keys_virtual.motion != '@') {
                update_keys(&game->keys, 0);
                game->tick = (game->tick + 1) % 120;
                game->keys_virtual.motion = '@';
                game->keys_virtual.count = 0;
                break;
            }
            __attribute__((fallthrough));
        default:
            update_keys(&game->keys, 25000);
            break;
        }
    }

    if (game->keys.motion == 'q') {
        return;
    } else if ((game->keys.motion == 'h' || game->keys.motion == 'D' ||
                ((game->keys.motion == '^' || game->keys.motion == '_') &&
                 game->pos_head.x)) &&
               (game->dir != SNAKE_DIR_R ||
                pos_cmp(game->pos_head, game->pos_tail))) {
        game->dir = SNAKE_DIR_L;
    } else if ((game->keys.motion == 'j' || game->keys.motion == 'B' ||
                (game->keys.motion == 'L' &&
                 game->pos_head.y < game->grid->height - 1)) &&
               (game->dir != SNAKE_DIR_U ||
                pos_cmp(game->pos_head, game->pos_tail))) {
        game->dir = SNAKE_DIR_D;
    } else if ((game->keys.motion == 'k' || game->keys.motion == 'A' ||
                (game->keys.motion == 'H' && game->pos_head.y)) &&
               (game->dir != SNAKE_DIR_D ||
                pos_cmp(game->pos_head, game->pos_tail))) {
        game->dir = SNAKE_DIR_U;
    } else if ((game->keys.motion == 'l' || game->keys.motion == 'C' ||
                (game->keys.motion == '$' &&
                 game->pos_head.x < game->grid->width - 1)) &&
               (game->dir != SNAKE_DIR_L ||
                pos_cmp(game->pos_head, game->pos_tail))) {
        game->dir = SNAKE_DIR_R;
    } else if (game->keys.motion == 'M') {
        uint8_t half_grid_height = (game->grid->height - 1) >> 1;
        if ((game->pos_head.y < half_grid_height) &&
            (game->dir != SNAKE_DIR_U ||
             pos_cmp(game->pos_head, game->pos_tail))) {
            game->dir = SNAKE_DIR_D;
        } else if ((game->pos_head.y > half_grid_height) &&
                   (game->dir != SNAKE_DIR_D ||
                    pos_cmp(game->pos_head, game->pos_tail))) {
            game->dir = SNAKE_DIR_U;
        } else {
            update_keys(&game->keys, 0);
        }
    } else if (game->keys_virtual.motion == '@') {
        game->keys.motion = ' ';
    }
}
