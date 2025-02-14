#include "utils.h"

void echo_info(int format, pgm_head_t *img)
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

