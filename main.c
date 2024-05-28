#include "src/encoder/dct.h"
#include "src/utils/utils.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
    FILE *fp             ;
    int format        = 0;

    /*if (argc < 3)
    {
        fprintf(stderr, "Usage: ./dct <input file path> <output file path>\n");
        return 1;
    }

    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        fprintf(stderr, "No such file or directory\n");
        return 2;
    }*/

    /* 8 * 8 test image */
    //Matrix *img = malloc(sizeof(*img));
    //pad_matrix(img, 255);

    float img[N][N] = {{64,56,56,57,70,84,84,59},
                     {66,64,35,36,87,45,21,58},
                     {66,66,66,59,35,87,26,104},
                     {35,75,76,45,81,37,34,35},
                     {45,96,125,107,31,15,107,90},
                     {88,89,88,78,64,57,85,81},
                     {62,59,68,113,144,104,66,73},
                     {107,121,89,21,35,64,65,65}};

    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%g ", img[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    /* level shift */
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            img[i][j] -= 128;
        }
    }

    //print_matrix(img);
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%g ", img[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    dct(img);

    //print_matrix(img);
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%.2f ", img[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    quantization(img);
    printf("\nAfter quantization...\n");

    //print_matrix(img);
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%.2f ", img[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    //P2PGM *img = malloc(sizeof(*img));

    /*if (read_pgm_head(fp, img) != 0)
    {
        fprintf(stderr, "Error reading pgm head\n");
        return 3;
    }*/

    //unsigned char *buffer = read_p2(fp, img);

    //fclose(fp);
    //free(img);
    //free(buffer);
    return 0;
}

