CC=gcc
FILES= main.c
all:
	$(CC) -Wall -Werror -o sort_vis $(FILES) -lSDL2 -lSDL2_ttf -lm
