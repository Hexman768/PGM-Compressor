CC = gcc

all:
	$(CC) -o main src/encoder/dct.c src/decoder/decode.c src/utils/utils.c main.c -lm

