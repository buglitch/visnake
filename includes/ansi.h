// SPDX-License-Identifier: GPL-3.0-or-later
#ifndef ANSI_H
#define ANSI_H

#define CLEAR "\x1b[2J"
#define CUP00 "\x1b[H"

#define SMCUP "\x1b[?1049h"
#define RMCUP "\x1b[?1049l"

#define CIVIS "\x1b[?25l"
#define CNORM "\x1b[?25h"

#define RED "\x1b[0;31m"
#define GRN "\x1b[0;32m"
#define YEL "\x1b[0;33m"

#define REDB "\x1b[41m"
#define GRNB "\x1b[42m"
#define YELB "\x1b[43m"

#define HBLK "\x1b[;90m"

#define BHRED "\x1b[1;91m"
#define BHGRN "\x1b[1;92m"
#define BHYEL "\x1b[1;93m"

#define BHWHT "\x1b[1;97m"

#define CRESET "\x1b[0m"

#endif /* ANSI_H */
