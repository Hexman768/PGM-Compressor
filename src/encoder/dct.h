#ifndef DCT_H
#define DCT_H

#define N 8     // Max block size
#define MAX 800
#define LEN 128 // line length maximum

/*
 * Type structure that represents a block of
 * size 8x8 (NxN).
 *
 * element: unsigned byte array of pixel data
 */
typedef struct Matrix {
    unsigned char element[N][N];
} Matrix;

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
 *  Restructures the data buffer in zig-zag order for the
 *  purpose of quantization
 *
 *  m: Matrix to be used for zig-zag ordering
 *  data: Image pixel data
 *  index: Current index in data buffer
 *
 *  returns: TODO
 */
int zz_order(Matrix m, unsigned char *data, int index);

/*
 *  Reverses the zig-zag ordering process for the
 *  purpose of decoding
 *
 *  x: Integer pointer to image data array
 *
 *  returns: Matrix in original pixel order
 */
Matrix inverse_zz(int *x);

/*
 *  Reads the information from the head of a PGM image file
 *
 *  fp: File pointer
 *  img: PGM image data
 *
 *  returns: Status code
 */
int read_pgm_head(FILE *fp, pgm_head_t *img);

/*
 *  Reads the information from p2 PGM image
 *
 *  fp:  File pointer
 *  img: PGM image data
 *
 *  returns: Unsigned byte array of pixel data
 */
unsigned char* pgm_p2_read(FILE *fp, pgm_head_t *img);

#endif

