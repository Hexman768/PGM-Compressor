#include "decode.h"

void writeP5PGM(char *filename, int *width, int *height, int *max, Matrix *data) {
    FILE *outFile;
    int i, j, x, y;
    if ((outFile = fopen(filename, "wb")) == NULL) {
        printf("Could not open file: %s\n", filename);
        return;
    }

    fprintf(outFile, "P5\n");
    fprintf(outFile, "%d %d\n", *width, *height);
    fprintf(outFile, "%d\n", *max);

    unsigned char *buffer = (unsigned char*)malloc((*width) * (*height) * sizeof(unsigned char));
    unsigned char image[*height][*width];
    int nBlock = 0;

     for (i = 0; i <= *height; i += N) {
        for (j = 0; j <= *width; j += N) {
            if (i >= N)
                i -= N;
            for (y = 0; y < N; y++, i++) {
                if (j % N == 0 && j != 0)
                    j -= N;
                for (x = 0; x < N; x++, j++) {
                    /* Check if boundaries are exceeded */
                    if (i > *height || j > *width)
                        continue;
                    //data[nBlock].element[y][x] = image[i][j];
                    image[i][j] = data[nBlock].element[y][x];
                }
            }
            nBlock++;
        }
    }
    int counter = 0;
    for (i = 0; i < *height; i++) {
        for (j = 0; j < *width; j++) {
            buffer[counter] = image[i][j];
            counter++;
        }
    }

    fwrite(buffer, sizeof(unsigned char), (*width) * (*height), outFile);
    free(buffer);
    fclose(outFile);
}
