CC = gcc
CFLAGS = -lm

invert:
	$(CC) -o invert src/invert/invert.c

compress:
	$(CC) $(CFLAGS) -g -o compress src/compress/compress.c

all:
	$(CC) -o main main.c -lm

