CC       = gcc
LIBS     = -lm
CFLAGS   = -std=c99 -pedantic -Wall -Wextra -I$(PREFIX)/include
CFLAGS  += -D_POSIX_C_SOURCE=200112L
LDFLAGS  = -L$(PREFIX)/lib

PREFIX    ?= /usr/local
BINPREFIX  = $(PREFIX)/bin

SRC = $(wildcard *.c)
BIN = $(basename $(SRC))

comma  = ,
empty  =
space  = $(empty) $(empty)
BINLST = $(subst $(space),$(comma),$(BIN))

all: CFLAGS += -Os
all: LDFLAGS += -s
all: options $(BIN)

debug: CFLAGS += -O0 -g -DDEBUG
debug: options $(BIN)

options:
	@echo "Build options:"
	@echo "CC      = $(CC)"
	@echo "CFLAGS  = $(CFLAGS)"
	@echo "LDFLAGS = $(LDFLAGS)"
	@echo "LIBS    = $(LIBS)"
	@echo "PREFIX  = $(PREFIX)"

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) $(LIBS) -o $@ $@.c

clean:
	@echo "Cleaning."
	rm -f $(BIN)

install:
	@echo "Installing executable files to '$(DESTDIR)$(BINPREFIX)'."
	mkdir -p "$(DESTDIR)$(BINPREFIX)"
	cp $(BIN) "$(DESTDIR)$(BINPREFIX)"
	chmod 755 "$(DESTDIR)$(BINPREFIX)"/{$(BINLST)}

uninstall:
	@echo "Removing executable files from '$(DESTDIR)$(BINPREFIX)'."
	rm -f "$(DESTDIR)$(BINPREFIX)"/{$(BINLST)}

.PHONY: all debug options clean install uninstall
