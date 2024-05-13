#include "dct.h"
#include "../utils/utils.h"
#include <math.h>
#include <stdlib.h>

struct zigzag ZigZag[N * N] = {
	{0,0},
	{0,1},{1,0},
	{2,0},{1,1},{0,2},
	{0,3},{1,2},{2,1},{3,0},
	{4,0},{3,1},{2,2},{1,3},{0,4},
	{0,5},{1,4},{2,3},{3,2},{4,1},{5,0},
	{6,0},{5,1},{4,2},{3,3},{2,4},{1,5},{0,6},
	{0,7},{1,6},{2,5},{3,4},{4,3},{5,2},{6,1},{7,0},
	{7,1},{6,2},{5,3},{4,4},{3,5},{2,6},{1,7},
	{2,7},{3,6},{4,5},{5,4},{6,3},{7,2},
	{7,3},{6,4},{5,5},{4,6},{3,7},
	{4,7},{5,6},{6,5},{7,4},
	{7,5},{6,6},{5,7},
	{6,7},{7,6},
	{7,7}
};

int zz_order(Matrix m, unsigned char *data, int index)
{
    int i;
    for (i = index; i < N * N; i++)
    {
        data[i] = m.element[ZigZag[i].row][ZigZag[i].col];
    }
    return i;
}

Matrix inverse_zz(int *x)
{
    int    i;
    Matrix m;
    for (i = 0; i < N * N; i++)
    {
        m.element[ZigZag[i].row][ZigZag[i].col] = x[i];
    }
    return m;
}

void dct(P2PGM *img)
{
    int i, j, k, l;

    float ci, cj, dct1, sum;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            if (i == 0)
            {
                ci = 1 / sqrt(N);
            }
            else
            {
                ci = sqrt(2) / sqrt(N);
            }

            if (j == 0)
            {
                cj = 1 / sqrt(N);
            }
            else
            {
                cj = sqrt(2) / sqrt(N);
            }

            sum = 0;
            for (k = 0; k < N; k++)
            {
                for (l = 0; l < N; l++)
                {
                    dct1 = img->data[k][l] * 
                        cos((2 * k + 1) * i * M_PI / (2 * N)) *
                        cos((2 * l + 1) * j * M_PI / (2 * N));
                    sum = sum + dct1;
                }
            }
            img->data[i][j] = ci * cj * sum;
        }
    }
}

int read_pgm_head(FILE *fp, P2PGM *img)
{
    int format    ;
    int width;
    int height;
    char line[LEN];

    fgets(line, LEN, fp); // read first line
    //sscanf(&line[1], "%d", &format);

    // skip comments
    fgets(line, LEN, fp);
    while (line[0] == '#')
    {
        fgets(line, LEN, fp);
    }

    // read width and height
    sscanf(line, "%d %d\n", &img->width, &img->height);

    // read max value 
    fgets(line, LEN, fp);
    sscanf(line, "%d\n", &img->max);
    echo_info(2, img);

    return 0;
}

unsigned char* read_p2(FILE *fp, P2PGM *img)
{
    int i;

    // allocate byte buffer
    unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char) * (img->width * img->height));

    // read all data into buffer
    for (i = 0; i < (img->width * img->height * sizeof(unsigned char)); i++)
    {
        fscanf(fp, "%d", (unsigned char*) &buffer[i]);
    }

    //print_buffer(buffer, width, height);

    //allocate pointer array of Matrix sizeof((N * N) * (information / 64))
    int num_matrix = (img->width * img->height) / 64;
    Matrix* blocks = (Matrix*) malloc(sizeof(Matrix) * 64 * num_matrix);
    unsigned char *byte_buffer = (unsigned char*) malloc(sizeof(unsigned char) * (img->width * img->height));

    int index = 0;
    for (i = 0; i < num_matrix; i++)
    {
        index = zz_order(blocks[i], byte_buffer, index);
    }

    // free memory 
    free(buffer);
    free(blocks);
    return byte_buffer;
}

