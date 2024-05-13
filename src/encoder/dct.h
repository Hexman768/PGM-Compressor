#ifndef DCT_H
#define DCT_H

#include <stdio.h>

#define N 8
#define MAX 800
#define LEN 128 // line length maximum

typedef struct Matrix {
    unsigned char element[N][N];
} Matrix;

typedef struct P2PGM {
    int width;
    int height;
    int max;
    int data[MAX][MAX];
} P2PGM;

struct zigzag {
	int row, col;
};

int zz_order(Matrix m, unsigned char *data, int index);

Matrix inverse_zz(int *x);

int read_pgm_head(FILE *fp, P2PGM *img);

unsigned char* read_p2(FILE *fp, P2PGM *img);

#endif

