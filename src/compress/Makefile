CC = gcc
CFLAGS = -Wall -Wextra -O2 -lm

all: compress

compress: compress.c
	$(CC) $(CFLAGS) -o compress compress.c

clean:
	rm -f compress *.o *.dat

.PHONY: all clean