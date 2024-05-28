#ifndef DCT_H
#define DCT_H

#include <stdio.h>

#define N 8
#define MAX 800
#define BYTE_MAX 255
#define LEN 128 // line length maximum

typedef struct Matrix {
    int element[N][N];
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

void dct(float img[N][N]);

void quantization(float img[N][N]);

#endif

