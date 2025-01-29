#include "invert.h"

int read_pgm_head(FILE *fp, pgm_head_t *head) {
    int format;
    int width;
    int height;
    char line[LEN];

    fgets(line, LEN, fp); // read first line

    // skip comments
    fgets(line, LEN, fp);
    while (line[0] == '#') {
        fgets(line, LEN, fp);
    }

    // read width and height
    sscanf(line, "%d %d\n", &head->width, &head->height);

    // read max value 
    fgets(line, LEN, fp);
    sscanf(line, "%d\n", &head->max);

    return 0;
}

int main(int argc, char **argv) { 
    FILE *fp_in;
    FILE *fp_out;

    if (argc < 3) {
        fprintf(stderr, "Usage: ./dct <input file path> <output file path>\n");
        return 1;
    }

    if ((fp_in = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "No such file or directory\n");
        return 2;
    }

    if ((fp_out = fopen(argv[2], "wb")) == NULL) {
        fprintf(stderr, "No such file or directory\n");
        return 2;
    }

    pgm_head_t *img_head = malloc(sizeof(*img_head));

    if (read_pgm_head(fp_in, img_head) != 0) {
        fprintf(stderr, "Error reading pgm head\n");
        return 3;
    }

    int *buffer = malloc(sizeof(int) * img_head->width * img_head->height);

    // read all data into buffer
    int i, j = 0;
    for (i = 0; i < (img_head->width * img_head->height * sizeof(int)); i++) {
        fscanf(fp_in, "%d", &buffer[i]);
    }

    printf("%d", *buffer);

    fprintf(fp_out, "P2 \n %d %d \n %d \n", img_head->width, img_head->height, img_head->max);
     
    // Create PGM image using pixel value
    for (i = 0; i < img_head->height; i++) {
        for (j = 0; j < img_head->width; j++)
            fprintf(fp_in, "%d ", buffer[i * img_head->width + j]);
        fprintf(fp_in, "\n");
    }
     
    // Create negative PGM image using pixel value
    for (i = 0; i < img_head->height; i++) {
        for (j = 0; j < img_head->width; j++) 
            fprintf(fp_out, "%d ", (255 - buffer[i * img_head->width + j]));
        fprintf(fp_out, "\n");
    }

    // free all memory
    fclose(fp_in);
    fclose(fp_out);
    free(img_head);
    free(buffer);

    return 0;
}

