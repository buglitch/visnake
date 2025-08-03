// SPDX-License-Identifier: GPL-3.0-or-later
#include "term.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "ansi.h"
#include "cell.h"
#include "game.h"

void set_term(bool init) {
    static struct termios oldt, newt;
    static int oldf;

    if (init) {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        printf(SMCUP CIVIS);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        printf(RMCUP CNORM);
    }
}

char get_key(useconds_t usec) {
    char c = 0;
    usleep(usec);
    if (read(STDIN_FILENO, &c, 1) < 0)
        c = 0;
    return c;
}
