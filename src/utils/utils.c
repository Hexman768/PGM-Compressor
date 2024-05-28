#include "utils.h"

void echo_info(int format, P2PGM *img)
{
    printf("P%d\n", format);
    printf("W: %d, H: %d\n", img->width, img->height);
    printf("Max: %d\n", img->max);
}

void print_buffer(unsigned char *buffer, int *width, int *height)
{
    int i = 0;
    while (i < (sizeof(unsigned char) * ((*width) * (*height))))
    {
        printf("%c", buffer[i]);
        if (i == (*width))
        {
            printf("\n");
        }
        i++;
    }
}

void img_pad(unsigned char data[MAX][MAX])
{
    int i, j;
    for (i = 0; i < MAX; i++)
    {
        for (j = 0; j < MAX; j++)
        {
            data[i][j] = 0x0;
        }
    }
}

int pad_matrix(Matrix *img, int value)
{
    if (value > BYTE_MAX)
    {
        printf("[ERROR] pad_matrix(Matrix *img, int value) only allows a value of up to 255\nValue provided is %d\n", value);
        return 1;
    }

    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            img->element[i][j] = value;
        }
    }
    return 0;
}

void breakpoint()
{
    printf("Hit Breakpoint\n");
}

void print_matrix(Matrix *mtx)
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            printf("%d ", mtx->element[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

