// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef CTRL_H
#define CTRL_H

#include <unistd.h>

#include "game.h"
#include "term.h"

void update_keys(struct keys *keys, useconds_t usec);
void update_dir(struct game *game);

#endif /* !CTRL_H */
