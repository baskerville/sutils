CC      ?= gcc
LIBS     = -lm -lasound
CFLAGS  += -std=c99 -pedantic -Wall -Wextra -I$(PREFIX)/include
CFLAGS  += -D_POSIX_C_SOURCE=200112L
LDFLAGS += -L$(PREFIX)/lib

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
all: $(BIN)

include Sourcedeps

debug: CFLAGS += -O0 -g -DDEBUG
debug: $(BIN)

$(BIN): Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $@.c $(LIBS)

install:
	mkdir -p "$(DESTDIR)$(BINPREFIX)"
	cp -p $(BIN) "$(DESTDIR)$(BINPREFIX)"

uninstall:
	rm -f "$(DESTDIR)$(BINPREFIX)"/{$(BINLST)}

clean:
	rm -f $(BIN)

.PHONY: all debug clean install uninstall
