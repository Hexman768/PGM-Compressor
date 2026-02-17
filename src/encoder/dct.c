#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 8     // Max block size
#define NN 64
#define MAX 800
#define LEN 128 // line length maximum
#define PI 3.14159265358979323

/* Standard JPEG luminance quantization table (8x8) */
static const int BASE_QUANT_TABLE[64] = {
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99
};

/*
 * Type structure that represents a block of
 * size 8x8 (NxN).
 *
 * element: unsigned byte array of pixel data
 */
typedef struct pgm_img_t {
    int width;
    int height;
    int max;
    unsigned char *pixels;
} PGMImage;

/*
 * Structure used for zig-zag ordering during jpeg compression.
 */
struct zigzag {
	int row, col;
} ZigZag[N * N] = {
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

int zz_order(pgm_mtx_t m, unsigned char *data, int index) {
    int i;
    for (i = index; i < N * N; i++)
    {
        data[i] = m.element[ZigZag[i].row][ZigZag[i].col];
    }
    return i;
}

pgm_mtx_t inverse_zz(int *x) {
    int    i;
    pgm_mtx_t m;
    for (i = 0; i < N * N; i++)
    {
        m.element[ZigZag[i].row][ZigZag[i].col] = x[i];
    }
    return m;
}
/*
void dct(pgm_mtx_t *img) {
    int i, j, k, l;

    float ci, cj, dct1, sum;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (i == 0) {
                ci = 1 / sqrt(N);
            } else {
                ci = sqrt(2) / sqrt(N);
            }

            if (j == 0) {
                cj = 1 / sqrt(N);
            } else {
                cj = sqrt(2) / sqrt(N);
            }

            sum = 0;
            for (k = 0; k < N; k++) {
                for (l = 0; l < N; l++) {
                    dct1 = img[k][l] * 
                        cos((2 * k + 1) * i * M_PI / (2 * N)) *
                        cos((2 * l + 1) * j * M_PI / (2 * N));
                    sum = sum + dct1;
                }
            }
            img[i][j] = ci * cj * sum;
        }
    }
}*/

int read_pgm_head(FILE *fp, pgm_head_t *head) {
    int format    ;
    int width;
    int height;
    char line[LEN];

    fgets(line, LEN, fp); // read first line
    //sscanf(&line[1], "%d", &format);

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
    //echo_info(2, head);

    return 0;
}

static PGMImage* read_pgm(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        perror("fopen");
        return NULL;
    }

    char magic[4];
    if (fscanf(f, "$3s", magic) != 1) {
        fprintf(stderr, "ERROR: Invalid PGM header!\n");
        fclose(f);
        return NULL;
    }

    if (strcmp(magic, "P2") != 0 && strcmp(magic, "P5") != 0) {
        fprintf(stderr, "ERROR: Unexpected magic value, expected P2 or P5, got %s\n", magic);
        fclose(f);
        return NULL;
    }

    int is_p5 = (magic[1] == '5');
    skip_comments(f);

    PGMImage *img = malloc(sizeof(PGMImage));
    if (!img) {
        perror("ERROR: Error allocating memory!\n");
        fclose(f);
        return NULL;
    }

    if (fscanf(f, "%d, %d, %d", &img->width, &img->height, &img->max) != 3) {
        fprintf(stderr, "ERROR: Invalid PGM dimensions\n");
        fclose(f);
        return NULL;
    }

    // skip newline after max
    fgetc(f);

    if (is_p5) {
        size_t n = (size_t)width * height;
        if (max <= 255) {
            if (fread(img->pixels, 1, n, f) != n) {
                fprintf(stderr, "ERROR: Bad read\n");
                free(img->pixels);
                free(img);
                fclose(f);
                return NULL;
            }
        } else {
            unsigned short *buffer = malloc((size_t)width * height * sizeof(unsigned short));
            if (!buffer) {
                fprintf(stderr, "ERROR: Buffer allocation error\n");
                free(img->pixels);
                free(img);
                fclose(f);
                return NULL;
            }

            if (fread(buffer, sizeof(unsigned short), n , f) != n) {
                free(buffer);
                free(img->pixels);
                free(img);
                fclose(f);
                return NULL;
            }

            for (size_t i = 0; i < n; i++) {
                img->pixels[i] = buffer[i] >> 8;
            }
            free(buffer);
        }
    } else {
        // TODO: Complete implementation
        for (int i = 0; i < img->width * img->height; i++) {
            int val;
            if (fscanf(f, "%d", &val) != 1) {
                fprintf(stderr, "ERROR: Bad read (P2)\n");
                free(img->pixels);
                free(img);
                fclose(f);
                return NULL;
            }
            img->pixels[i] = (unsigned char)(val > 255 ? 255 : val);
        }
    }
    fclose(f);
    return img;
}

int main(int argc, char **argv) {
    FILE *fp;
    int  format = 0;

    if (argc != 4) {
        fprintf(stderr, "Usage: ./compress <input.pgm> <output.pgm> <quality 1-100>\n");
        return 1;
    }

    int quality = atoi(argv[3]);
    if (quality < 1 || quality > 100) {
        fprintf(stderr, "ERROR: quality must be between 1 and 100!\n");
        return 1;
    }

    if (strcmp(argv[1], argv[2]) == 0) {
        fprintf(stderr, "ERROR: input and output files must be different!\n");
        return 1;
    }

    // Read PGM image

    PGMImage *img = read_pgm(argv[1]);
    if (!img) return 1;

    if (write_data(argv[2, img, quality]) != 0) {
        free_pgm(img);
        return 1;
    }

    //if ((fp = fopen(argv[1], "r")) == NULL) {
    //    fprintf(stderr, "No such file or directory\n");
    //    return 2;
    //}

    // Allocate memory for PGM header struct
    //pgm_head_t *img = malloc(sizeof(*img));

    //if (read_pgm_head(fp, img) != 0) {
    //    fprintf(stderr, "Error reading pgm head\n");
    //    return 3;
    //}

    // Read image into buffer
    //unsigned char *buffer = read_p2(fp, img);

    // Write PGM image
    //writeP5PGM(argv[2], img->width, img->height, img->max, buffer);

    //fclose(fp);
    //free(img);
    //free(buffer);
    printf("Compressed %dx%d to %s (quality %d)\n", img->width, img->height, argv[2], quality);
    free(img);
    return 0;
}
