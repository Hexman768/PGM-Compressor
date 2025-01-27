#include "src/encoder/dct.h"
#include "src/decoder/decode.h"
#include <stdlib.h>

int main(int argc, char **argv) {
    FILE *fp             ;
    int format        = 0;

    if (argc < 3) {
        fprintf(stderr, "Usage: ./dct <input file path> <output file path>\n");
        return 1;
    }

    if ((fp = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "No such file or directory\n");
        return 2;
    }

    // Allocate memory for PGM header struct
    PGMHEAD *img = malloc(sizeof(*img));

    if (read_pgm_head(fp, img) != 0) {
        fprintf(stderr, "Error reading pgm head\n");
        return 3;
    }

    // Read image into buffer
    unsigned char *buffer = read_p2(fp, img);

    // Write PGM image
    writeP5PGM(argv[2], img->width, img->height, img->max, buffer);

    fclose(fp);
    free(img);
    free(buffer);
    return 0;
}

