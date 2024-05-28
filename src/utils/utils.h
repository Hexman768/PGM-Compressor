#ifndef UTILS_H
#define UTILS_H

#include "../encoder/dct.h"

void echo_info(int format, P2PGM *img);

void print_buffer(unsigned char *buffer, int *width, int *height);

void img_pad(unsigned char data[MAX][MAX]);

int pad_matrix(Matrix *img, int value);

void breakpoint();

void print_matrix(Matrix *mtx);

#endif

