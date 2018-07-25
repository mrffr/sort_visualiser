CC=gcc
FILES= main.c
all:
	$(CC) -Wall -Werror $(FILES) -lSDL2 -lSDL2_ttf -lm
