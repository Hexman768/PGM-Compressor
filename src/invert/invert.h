#include<stdio.h>
#include<stdlib.h>

#define LEN 128

/*
 * Type structure that represents the PGM image header information
 *
 * width:  Image width
 * height: Image height
 * max:    Max grey value
 */
typedef struct _pgm_head_t {
    int width;
    int height;
    int max;
} pgm_head_t;

/*
 * Reads PGM header data from file into header struct.
 *
 * fp:   File pointer
 * head: Image header struct
 */
int read_pgm_head(FILE *fp, pgm_head_t *head);

