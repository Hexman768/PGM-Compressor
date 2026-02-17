#ifndef DECOMPRESS_H
#define DECOMPRESS_H

/*
 * Constructs and writes a P5 PGM file to the given directory location.
 */
void writeP5PGM(char *filename, int *width, int *height, int *max, Matrix *data);

#endif

