CC = gcc

invert:
	$(CC) -o invert src/invert/invert.c

encode:
	$(CC) -g -o encode src/encoder/dct.c -lm

all:
	$(CC) -o main main.c -lm

