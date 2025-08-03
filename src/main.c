// SPDX-License-Identifier: GPL-3.0-or-later
#include <signal.h>
#include <stdio.h>

#include "ansi.h"
#include "game.h"

void bad_quit(int sig) {
    (void)sig;
    puts("");
    puts(REDB BHWHT "E37: How do I exit Vim? (press q to override)" CRESET);
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    signal(SIGINT, bad_quit);
    int status = run_game();

    puts("");
    switch (status) {
    case 1:
        puts(BHRED "-- GAME OVER --" CRESET);
        break;
    case 2:
        puts(BHGRN "-- YOU WIN! & --" CRESET);
        break;
    default:
        puts(BHYEL "Thanks for flying Visnake" CRESET);
        break;
    }

    return 0;
}
