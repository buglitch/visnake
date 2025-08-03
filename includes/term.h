// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef TERM_H
#define TERM_H

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

struct keys {
    uint8_t count;
    char motion;
};

void set_term(bool init);
char get_key(useconds_t usec);

#endif /* !TERM_H */
