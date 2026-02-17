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

/* Read PGM Image Contents */
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

/* Pad image dimensions to multiple of 8 */
static void pad_image(PGMImage *img, int *padded_w, int *padded_h) {
    *padded_w = ((img->width + BLOCK_SIZE - 1) / BLOCK_SIZE) * BLOCK_SIZE;
    *padded_h = ((img->height + BLOCK_SIZE - 1) / BLOCK_SIZE) * BLOCK_SIZE;
}

/* Compress and write to .dat file */
static int write_dat(const char *filename, PGMImage *img, int quality) {
    FILE *f = fopen(filename, "wb");
    if (!f) {
        perror("fopen");
        return -1;
    }

    int padded_w, padded_h;
    pad_image(img, &padded_w, &padded_h);

    int quant_table[64];
    build_quant_table(quant_table, quality);

    /* Header: magic, width, height, orig_width, orig_height, quality */
    const char magic[] = "JPGC";
    fwrite(magic, 1, 4, f);
    fwrite(&padded_w, sizeof(int), 1, f);
    fwrite(&padded_h, sizeof(int), 1, f);
    fwrite(&img->width, sizeof(int), 1, f);
    fwrite(&img->height, sizeof(int), 1, f);
    fwrite(&img->maxval, sizeof(int), 1, f);
    fwrite(&quality, sizeof(int), 1, f);
    fwrite(quant_table, sizeof(int), 64, f);

    int blocks_x = padded_w / BLOCK_SIZE;
    int blocks_y = padded_h / BLOCK_SIZE;

    for (int by = 0; by < blocks_y; by++) {
        for (int bx = 0; bx < blocks_x; bx++) {
            double block[BLOCK_SIZE][BLOCK_SIZE];

            for (int y = 0; y < BLOCK_SIZE; y++) {
                for (int x = 0; x < BLOCK_SIZE; x++) {
                    int px = bx * BLOCK_SIZE + x;
                    int py = by * BLOCK_SIZE + y;
                    if (px < img->width && py < img->height) {
                        block[y][x] = (double)img->pixels[py * img->width + px] - 128.0;
                    } else {
                        block[y][x] = -128.0;
                    }
                }
            }

            dct2d(block);

            /* Quantize and write as 4-byte integers */
            for (int i = 0; i < 64; i++) {
                int y = i / 8, x = i % 8;
                int q = (int)round(block[y][x] / quant_table[i]);
                fwrite(&q, sizeof(int), 1, f);
            }
        }
    }

    fclose(f);
    return 0;
}

static void free_pgm(PGMImage *img) {
    if (img) {
        free(img->pixels);
        free(img);
    }
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

    printf("Compressed %dx%d to %s (quality %d)\n", img->width, img->height, argv[2], quality);
    free(img);
    return 0;
}
