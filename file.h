#ifndef _FILE_H
#define _FILE_H

#include <stdio.h>
#include <stdlib.h>

#define N 8
#define LEN 128

int nBlocks;
int nWidth;
int nHeight;

typedef struct _Block2 {
    unsigned char element[N][N];
} BLOCK2;

/* Read P6 image and output P6 image */
BLOCK2* readP6PGM(FILE *inFile, int *width, int *height, int *max, char *outputFile);

/* Read P5 image and output P5 image */
BLOCK2* readP5PGM(FILE *inFile, int *width, int *height, int *max, char *outputFile);

/* Read P2 image and output P5 image */
BLOCK2* readP2PGM(FILE *fp, int *width, int *height, int *max, char* outputFile);

void print_first_channel(BLOCK2 *blocks, int blockNum);

void print_first_last(BLOCK2 *blocks, int blockNum);

void writeP5PGM(char *filename, int *width, int *height, int *max, BLOCK2 *data);

void writeP6PGM(char *filename, int *width, int *height, int *max, BLOCK2 *data);

#endif
