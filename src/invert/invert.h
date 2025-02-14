#ifndef INVERT_H
#define INVERT_H

#include <stdio.h>
#include <stdlib.h>

#define LEN 128

#ifndef U8
#define U8 unsigned char
#endif

/*
 * Type structure that represents the PGM image header information
 *
 * width : Image width
 * height: Image height
 * max   : Max grey value
 */
typedef struct _pgm_head_t {
    int format;
    int width;
    int height;
    int max;
} pgm_head_t;

/*
 * Reads PGM header data from file into header struct.
 *
 * fp  : File pointer
 * head: Image header struct
 *
 * returns: pointer to pgm image header structure.
 */
pgm_head_t *read_pgm_head(FILE *fp);

/*
 * Inverts the colors of the given pgm image and writes them out using the 
 * file name given in the command line arguments.
 *
 * fp_in   : input file pointer
 * fp_out  : output file pointer
 * img_head: pgm image header structure
 */
void pgm_img_inv(FILE *fp_in, FILE *fp_out, pgm_head_t *img_head);

#endif

