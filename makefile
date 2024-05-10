CC = gcc

all:
	$(CC) -o main src/encoder/dct.c src/utils/utils.c main.c -lm

