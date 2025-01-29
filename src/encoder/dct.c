#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define N 8     // Max block size
#define NN 64
#define MAX 800
#define LEN 128 // line length maximum

/*
 * Type structure that represents a block of
 * size 8x8 (NxN).
 *
 * element: unsigned byte array of pixel data
 */
typedef struct _pgm_mtx_t {
    unsigned char element[N][N];
} pgm_mtx_t;

/*
 * Type structure that represents the PGM image header information
 *
 * width:  Image width
 * height: Image height
 * max:    Max grey value
 */
typedef struct pgm_head_t {
    int width;
    int height;
    int max;
} pgm_head_t;

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

unsigned char* read_p2(FILE *fp, pgm_head_t *img) {
    int i;

    // allocate byte buffer
    unsigned char *buffer = (unsigned char*) malloc(sizeof(unsigned char) * (img->width * img->height));

    // read all data into buffer
    for (i = 0; i < (img->width * img->height * sizeof(unsigned char)); i++) {
        fscanf(fp, "%d", (unsigned char*) &buffer[i]);
    }

    printf("%s", buffer);

    //print_buffer(buffer, width, height);

    //allocate pointer array of pgm_mtx_t sizeof((N * N) * (information / 64))
    //int num_matrix = (img->width * img->height) / 64;
    //pgm_mtx_t* blocks = (pgm_mtx_t*) malloc(sizeof(pgm_mtx_t) * 64 * num_matrix);
    //unsigned char *byte_buffer = (unsigned char*) malloc(sizeof(unsigned char) * (img->width * img->height));

    //int index = 0;
    //for (i = 0; i < num_matrix; i++) {
    //    index = zz_order(blocks[i], byte_buffer, index);
    //}

    // free memory 
    //free(buffer);
    //free(blocks);
    return buffer;
}

void writeP5PGM(char *filename, int *width, int *height, int *max, unsigned char *data) {
    FILE *outFile;
    int i, j, x, y;
    if ((outFile = fopen(filename, "wb")) == NULL) {
        printf("Could not open file: %s\n", filename);
        return;
    }

    fprintf(outFile, "P5\n");
    fprintf(outFile, "%d %d\n", *width, *height);
    fprintf(outFile, "%d\n", *max);

    /*unsigned char *buffer = (unsigned char*)malloc((*width) * (*height) * sizeof(unsigned char));
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
                    //Check if boundaries are exceeded
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
    }*/

    fwrite(data, sizeof(unsigned char), (*width) * (*height), outFile);
    free(data);
    fclose(outFile);
}

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
    pgm_head_t *img = malloc(sizeof(*img));

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

