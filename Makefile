.PHONY = all

CC=gcc
CFLAGS=--no-warnings
SRC=src/*.c
OUT=bin/xsh

all: setup

setup:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
clean:
	$(SUDO) rm -rf $(OUT)

