#include "src/encoder/dct.h"
#include <stdlib.h>

int main(int argc, char **argv)
{
    FILE *fp             ;
    int format        = 0;

    if (argc < 3)
    {
        fprintf(stderr, "Usage: ./dct <input file path> <output file path>\n");
        return 1;
    }

    if ((fp = fopen(argv[1], "r")) == NULL)
    {
        fprintf(stderr, "No such file or directory\n");
        return 2;
    }

    P2PGM *img = malloc(sizeof(*img));

    if (read_pgm_head(fp, img) != 0)
    {
        fprintf(stderr, "Error reading pgm head\n");
        return 3;
    }

    unsigned char *buffer = read_p2(fp, img);

    fclose(fp);
    free(img);
    free(buffer);
    return 0;
}

