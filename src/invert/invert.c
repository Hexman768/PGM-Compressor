#include "invert.h"

void print_pgm_head(pgm_head_t *head) {
    printf("header: %d\n", head->format);
    printf("width: %d\n", head->width);
    printf("height: %d\n", head->height);
    printf("max: %d\n", head->max);
}

pgm_head_t *read_pgm_head(FILE *fp) {
    int format;
    int width;
    int height;
    char line[LEN];

    pgm_head_t *head = malloc(sizeof(*head));

    fgets(line, LEN, fp);
    printf("format line: %s\n", line);
    sscanf(line, "P%d\n", &head->format);
    printf("header format: %d\n", head->format);

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

    // debug values
    printf("P%d\n %d %d\n %d\n", head->format, head->width, head->height, head->max);

    return head;
}

void pgm_img_inv(FILE *fp_in, FILE *fp_out, pgm_head_t *img_head) {
    int *buffer = malloc(sizeof(int) * img_head->width * img_head->height);

    // read all data into buffer
    int i, j = 0;
    for (i = 0; i < (img_head->width * img_head->height * sizeof(int)); i++) {
        fscanf(fp_in, "%d", &buffer[i]);
    }

    printf("%d", *buffer);

    fprintf(fp_out, "P2 \n %d %d \n %d \n", img_head->width, img_head->height, img_head->max);

    // Create negative PGM image using pixel value
    for (i = 0; i < img_head->height; i++) {
        for (j = 0; j < img_head->width; j++) 
            fprintf(fp_out, "%d ", (255 - buffer[i * img_head->width + j]));
        fprintf(fp_out, "\n");
    }

    free(buffer);
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

    pgm_head_t *img_head = read_pgm_head(fp_in);
    print_pgm_head(img_head);

    if (img_head == NULL) {
        fprintf(stderr, "Error reading pgm head\n");
        return 3;
    }

    // invert pgm image colors
    pgm_img_inv(fp_in, fp_out, img_head);

    // free all memory
    fclose(fp_in);
    fclose(fp_out);
    free(img_head);

    return 0;
}

