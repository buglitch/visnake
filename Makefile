# SPDX-License-Identifier: GPL-3.0-or-later
BIN:=visnake
SRCS:=$(wildcard src/*.c)
IDIR:=includes
CC:=gcc
MAKEFLAGS+=--no-print-directory
CC_STATIC+= -static -static-libgcc
CC_OPTS+=-O2 -march=native
CC_DEBUG+=-g
CFLAGS+=-I$(IDIR) -Wall -Werror -Wextra -pedantic $(CC_OPTS)
TARGET=$(shell uname -s | tr A-Z a-z)-$(shell uname -m)
VERSION=0.4.2
BIN_FULL=$(BIN)-$(VERSION)-$(TARGET)
PATH_RELEASE:=release
LOG=@echo -e "[make]"

.PHONY: $(BIN) all debug static debug-static release clean run check \
	__internal_check __internal_run

$(BIN):
	$(LOG) "\x1b[1;93m"./$(BIN)"\x1b[0m"
	$(CC) $(CFLAGS) $(SRCS) -o $@ || $(MAKE) __internal_check
	@$(MAKE) __internal_done

all: release $(BIN)

debug:
	$(LOG) "\x1b[1;93m"$@: ./$(BIN)"\x1b[0m"
	$(CC) $(CFLAGS) $(CC_DEBUG) $(SRCS) -o $(BIN) || $(MAKE) __internal_check
	@$(MAKE) __internal_done

static:
	$(LOG) "\x1b[1;93m"$@: ./$(BIN)"\x1b[0m"
	$(CC) $(CFLAGS) $(CC_STATIC) $(SRCS) -o $(BIN) || $(MAKE) __internal_check
	@$(MAKE) __internal_done

debug-static:
	$(LOG) "\x1b[1;93m"$@: ./$(BIN)"\x1b[0m"
	$(CC) $(CFLAGS) $(CC_STATIC) $(CC_DEBUG) $(SRCS) -o $(BIN) \
		|| $(MAKE) __internal_check
	@$(MAKE) __internal_done

release: clean
	$(LOG) "\x1b[1;93m"./release/"\x1b[0m"
	mkdir release
	$(LOG) "\x1b[1;93m\t└ "$(BIN_FULL)"\x1b[0m"
	$(CC) $(CFLAGS) $(SRCS) \
		-o $(PATH_RELEASE)/$(BIN_FULL) || $(MAKE) __internal_check
	$(LOG) "\x1b[1;93m\t└ "$(BIN_FULL)-debug"\x1b[0m"
	$(CC) $(CFLAGS) $(CC_DEBUG) $(SRCS) \
		-o $(PATH_RELEASE)/$(BIN_FULL)-debug || $(MAKE) __internal_check
	$(LOG) "\x1b[1;93m\t└ "$(BIN_FULL)-static"\x1b[0m"
	$(CC) $(CFLAGS) $(CC_STATIC) $(SRCS) \
		-o $(PATH_RELEASE)/$(BIN_FULL)-static || $(MAKE) __internal_check
	$(LOG) "\x1b[1;93m\t└ "$(BIN_FULL)-static-debug"\x1b[0m"
	$(CC) $(CFLAGS) $(CC_STATIC) $(CC_DEBUG) $(SRCS) \
		-o $(PATH_RELEASE)/$(BIN_FULL)-static-debug || $(MAKE) __internal_check
	@$(MAKE) __internal_done

clean:
	$(LOG) "\x1b[1;93m"$@"\x1b[0m"
	$(RM) $(BIN) || $(MAKE) __internal_check
	$(RM) -r $(PATH_RELEASE) || $(MAKE) __internal_check
	@$(MAKE) __internal_done

run:
	$(LOG) "\x1b[1;93m"run: ./$(BIN)"\x1b[0m"
	@./$(BIN) || $(MAKE) __internal_check
	@$(MAKE) __internal_done

check: $(BIN) run

__internal_check:
	$(LOG) "\x1b[0;31m"failed"\x1b[0m"

__internal_done:
	$(LOG) "\x1b[1;92m"done"\x1b[0m"
