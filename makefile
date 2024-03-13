CC = gcc
DEPS = file.h

all: file.o main.o
	$(CC) -o Assignment5 file.o main.o

file.o:
	$(CC) file.c -c

main.o:
	$(CC) main.c -c

