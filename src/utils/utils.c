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

void breakpoint()
{
    printf("Hit Breakpoint\n");
}

