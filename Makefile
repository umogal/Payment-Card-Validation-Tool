CC ?= cc
CFLAGS_BASE := -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Werror
CFLAGS_REL  := $(CFLAGS_BASE) -O3 -DNDEBUG
CFLAGS_DBG  := $(CFLAGS_BASE) -O0 -g
CFLAGS_SAN  := $(CFLAGS_DBG) -fsanitize=address,undefined -fno-omit-frame-pointer

INCLUDES := -Iinclude
SRC := src/cardval.c
OBJ := $(SRC:.c=.o)

.PHONY: all clean test bench asan debug release

all: release

release: CFLAGS := $(CFLAGS_REL)
release: bin/cardval_cli bin/cardval_test bin/cardval_bench

debug: CFLAGS := $(CFLAGS_DBG)
debug: bin/cardval_cli bin/cardval_test bin/cardval_bench

asan: CFLAGS := $(CFLAGS_SAN)
asan: clean bin/cardval_test bin/cardval_cli
	./bin/cardval_test

test: release
	./bin/cardval_test

bench: release
	./bin/cardval_bench

bin:
	mkdir -p bin

bin/cardval_cli: examples/cli.c $(SRC) | bin
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

bin/cardval_test: tests/test_cardval.c $(SRC) | bin
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

bin/cardval_bench: bench/bench_cardval.c $(SRC) | bin
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^

clean:
	rm -rf bin
