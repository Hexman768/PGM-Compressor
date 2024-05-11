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
    unsigned char data[MAX][MAX];
} P2PGM;

struct zigzag {
	int row, col;
};

int img_enc_zz(Matrix m, unsigned char *data, int index);

Matrix img_enc_inv_zz(int *x);

int read_pgm_head(FILE *fp, P2PGM *img);

unsigned char* read_p2(FILE *fp, P2PGM *img);

#endif

