#include "file.h"

BLOCK2* readP6PGM(FILE *inFile, int *width, int *height, int *max, char *outputFile) {
    FILE *outFile;
    int i, j, y, x;
    nWidth = (*width);
    nHeight = (*height);
    unsigned char *buffer = (unsigned char *)malloc(((*width) * (*height) * sizeof(unsigned char) * 3));
    unsigned char *red = (unsigned char *)malloc(((*width) * (*height) * sizeof(unsigned char)));
    unsigned char *green = (unsigned char *)malloc(((*width) * (*height) * sizeof(unsigned char)));
    unsigned char *blue = (unsigned char *)malloc(((*width) * (*height) * sizeof(unsigned char)));

    size_t read = fread(buffer, sizeof(unsigned char), (*width) * (*height) * 3, inFile);
    printf("Image Written!\n");
    fclose(inFile);

    j = 0;
    for (i = 0; i < (*width) * (*height) * 3; i += 3) {
        red[j] = buffer[i];
        j++;
    }

    j = 0;
    for (i = 1; i < (*width) * (*height) * 3; i += 3) {
        green[j] = buffer[i];
        j++;
    }
    j = 0;
    for (i = 2; i < (*width) * (*height) * 3; i += 3) {
        blue[j] = buffer[i];
        j++;
    }

    /* Creating new width and height both divisible by 8 */
    while (nWidth % 8 != 0)
        nWidth++;

    while (nHeight % 8 != 0)
        nHeight++;

    nBlocks = ((nWidth * nHeight) / 64);
    int nBlocks2 = 3 * nBlocks;
    printf("Width: %d, Height: %d\n", (*width), (*height));
    printf("nWidth: %d, nHeight: %d\n", nWidth, nHeight);
                        //nBlocks = 9;
    printf("Number of blocks: %d\n", nBlocks2);

    BLOCK2 *data = (BLOCK2 *)malloc(sizeof(BLOCK2) * ((N * N) * nBlocks2));
    BLOCK2 *datar = (BLOCK2 *)malloc(sizeof(BLOCK2) * ((N * N) * nBlocks));
    BLOCK2 *datag = (BLOCK2 *)malloc(sizeof(BLOCK2) * ((N * N) * nBlocks));
    BLOCK2 *datab = (BLOCK2 *)malloc(sizeof(BLOCK2) * ((N * N) * nBlocks));

    int count = 0;
    for (i = 0; i < nBlocks2; i++) {
        for (y = 0; y < N; y++) {
            for (x = 0; x < N; x++) {
                data[i].element[y][x] = buffer[count];
                    count++;
            }
        } 
    }
    unsigned char red_image[nHeight][nWidth];
    unsigned char green_image[nHeight][nWidth];
    unsigned char blue_image[nHeight][nWidth];
    unsigned char arr[nHeight*3][nWidth*3];

    for (i = 0; i < nHeight; i++) {
        for (j = 0; j < nWidth; j++) {
            red_image[i][j] = 0x0;
            green_image[i][j] = 0x0;
            blue_image[i][j] = 0x0;
        }
    }

    count = 0;
    for (i = 0; i < nHeight * 3; i++) {
        for (j = 0; j < nWidth * 3; j++) {
            arr[i][j] = buffer[count];
            count++;
        }
    }

    free(buffer);

    printf("Data filled with zeroes\n");
    int counter = 0;
    for (i = 0; i < (*height); i++) {
        for (j = 0; j < (*width); j++) {
            red_image[i][j] = red[counter];
            green_image[i][j] = green[counter];
            blue_image[i][j] = blue[counter];
            counter++;
        }
    }
    free(red);
    free(green);
    free(blue);
    printf("Created new image buffer\n");

    int nBlock = 0;
    //nBlocks = 1;
    for (i = 0; i <= nHeight; i += N) { // max: 360
        for (j = 0; j <= nWidth; j += N) { // max: 464
            //if (i >= N)
            //    i -= N;
            //printf("debug check for ternary");
            i = i >= N ? i - N : i;
            for (y = 0; y < N; y++, i++) {
                if (j % N == 0 && j != 0)
                    j -= N;
                for (x = 0; x < N; x++, j++) { // i and j are wrong
                    /* Check if boundaries are exceeded */
                    if (i > nHeight || j > nWidth)
                        continue;
                    datar[nBlock].element[y][x] = red_image[i][j];
                }
            }
                nBlock++;
        }
    }

    for (i = 0; i <= nHeight; i += N) {
        for (j = 0; j <= nWidth; j += N) {
            i = i >= N ? i - N : i;
            for (y = 0; y < N; y++, i++) {
                if (j % N == 0 && j != 0)
                    j -= N;
                for (x = 0; x < N; x++, j++) {
                    if (i > nHeight || j > nWidth)
                        continue;
                    datag[nBlock].element[y][x] = green_image[i][j];
                }
            }
            nBlock++;
        }
    }

    for (i = 0; i <= nHeight; i += N) {
        for (j = 0; j <= nWidth; j += N) {
            i = i >= N ? i - N : i;
            for (y = 0; y < N; y++, i++) {
                if (j % N == 0 && j != 0)
                    j -= N;
                for (x = 0; x < N; x++, j++) {
                    if (i > nHeight || j > nWidth)
                        continue;
                    datab[nBlock].element[y][x] = blue_image[i][j];
                }

            }
            nBlock++;
        }
    }

    print_first_channel(datar, nBlocks);

    free(datar);
    free(datag);
    free(datab);

    writeP6PGM(outputFile, width, height, max, data);
    return data;
}

BLOCK2* readP5PGM(FILE *fp, int *width, int *height, int *max, char *outputFile) {
    FILE *outFile;
    int i, j, y, x;
    nWidth = (*width);
    nHeight = (*height);
    unsigned char *buffer = (unsigned char *)malloc(((*width) * (*height) * sizeof(unsigned char)));

    size_t read = fread(buffer, sizeof(unsigned char), (*width) * (*height), fp);
    printf("P5 Image Written!\n");
    fclose(fp);

    while (nWidth % 8 != 0)
        nWidth++;
    while (nHeight % 8 != 0)
        nHeight++;

    nBlocks = ((nWidth * nHeight) / 64);

    BLOCK2 *data = (BLOCK2 *)malloc(sizeof(BLOCK2) * ((N * N) * nBlocks));

    unsigned char image[nHeight][nWidth];

    int counter = 0;
    for (i = 0; i < nHeight; i++) {
        for (j = 0; j < nWidth; j++) {
            image[i][j] = buffer[counter];
            counter++;
        }
    }

    int nBlock = 0;
    for (i = 0; i <= nHeight; i += N) {
        for (j = 0; j <= nWidth; j += N) {
            i = i >= N ? i - N : i;
            for (y = 0; y < N; y++, i++) {
                if (j % N == 0 && j != 0)
                    j -= N;
                for (x = 0; x < N; x++, j++) {
                    if (i > nHeight || j > nWidth)
                        continue;
                    data[nBlock].element[y][x] = image[i][j];
                }
            }
            nBlock++;
        }
    }

    free(buffer);
    return data;
}

void print_first_channel(BLOCK2 *blocks, int blockNum) {
    int i, j;
    int byte = 0;
    for (i = 0; i < N; i ++) {
        for (j = 0; j < N; j ++) {
            //if (byte == 0) {
            printf("%d ", blocks[0].element[i][j]);
        }
        printf("\n");
    }
    printf("Last Block ...\n");
    int blockNumber = blockNum - 3;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d ", blocks[blockNumber].element[i][j]);
                        
        }
        printf("\n");
    }
}

void print_first_last(BLOCK2 *blocks, int blockNum) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d ", blocks[0].element[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d ", blocks[blockNum - 1].element[i][j]);
        }
        printf("\n");
    }
}

void writeP5PGM(char *filename, int *width, int *height, int *max, BLOCK2 *data) {
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

     for (i = 0; i <= nHeight; i += N) {
        for (j = 0; j <= nWidth; j += N) {
            if (i >= N)
                i -= N;
            for (y = 0; y < N; y++, i++) {
                if (j % N == 0 && j != 0)
                    j -= N;
                for (x = 0; x < N; x++, j++) {
                    /* Check if boundaries are exceeded */
                    if (i > nHeight || j > nWidth)
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

void writeP6PGM(char *filename, int *width, int *height, int *max, BLOCK2 *data) {
    FILE *outFile;
    int i, j, x, y;
    if ((outFile = fopen(filename, "wb")) == NULL) {
        printf("Could not open file: %s\n", filename);
        return;
    }

    fprintf(outFile, "P6\n");
    fprintf(outFile, "%d %d\n", *width, *height);
    fprintf(outFile, "%d\n", *max);

    unsigned char *buffer = (unsigned char*)malloc((*width) * (*height) * sizeof(unsigned char) * 3);
    unsigned char red_i[*height][*width];
    unsigned char green_i[*height][*width];
    unsigned char blue_i[*height][*width];

    //print_first_last(blue, nBlocks);

    int nBlock = 0;

    int counter = 0;
    for (i = 0; i < nBlocks * 3; i++) {
        for (y = 0; y < N; y++) {
            for (x = 0; x < N; x++) {
                buffer[counter] = data[i].element[y][x];
                counter++;
            }
        }
    }

    fwrite(buffer, sizeof(unsigned char), (*width) * (*height) * 3, outFile);
    fclose(outFile);
    free(buffer);
}

BLOCK2* readP2PGM(FILE *fp, int *width, int *height, int *max, char* outputFile) {
    int i = 0, j = 0, x = 0, y = 0;                /* Navigational values */
    int k, l, m, n;
    nWidth = (*width);
    nHeight = (*height);     /* New width and height for padding */
    FILE *outFile;                   /* File to output image to */
    int value;
    //int **image = (unsigned char**)malloc((*width) * (*height) * sizeof(int *));
    unsigned char *buffer = (unsigned char *)malloc((*width) * (*height) * sizeof(unsigned char));

    //BLOCK2 *data = (BLOCK2 *)malloc(sizeof(BLOCK2) * (N * N) * (*width) * (*height));

    while (i < (*width) * (*height) * sizeof(unsigned char)) {
        fscanf(fp, "%d", &buffer[i]);
        //buffer[i] = value;
        i++;
    }
    // printf("%s", buffer);
    fclose(fp);

    /* Creating new width and height both divisible by 8 */
    while (nWidth % 8 != 0)
        nWidth++;

    while (nHeight % 8 != 0)
        nHeight++;

    nBlocks = (nWidth * nHeight) / 64;
    printf("Width: %d, Height: %d\n", (*width), (*height));
    //nBlocks = 9;
    printf("Number of blocks: %d\n", nBlocks);

    BLOCK2 *data = (BLOCK2 *)malloc(sizeof(BLOCK2) * ((N * N) * nBlocks));:
    /*unsigned char image[6][6] =     {{'0','0','0','0','0','0'},
                                     {'1','1','1','1','1','1'},
                                     {'2','2','2','2','2','2'},
                                     {'3','3','3','3','3','3'},
                                     {'4','4','4','4','4','4'},
                                     {'5','5','5','5','5','5'}};*/
    unsigned char image[nHeight][nWidth];
    //int iwidth = 6, iheight = 6;
    //unsigned char dividedImage[(360 * 464) / 64][8][8];
    //unsigned char nImage[2610][8][8];

    /* Pad image with zeroes */
    for (i = 0; i < nHeight; i++) {
        for (j = 0; j < nWidth; j++) {
            image[i][j] = 0x0;
        }
    }

    printf("Data filled with zeroes\n");
    int counter = 0;
    for (i = 0; i < (*height); i++) {
        for (j = 0; j < (*width); j++) {
            image[i][j] = buffer[counter];
            counter++;
        }
    }
    printf("Created new image buffer\n");

    int nBlock = 0;
    //nBlocks = 1;
    for (i = 0; i <= nHeight; i += N) { // max: 360
        for (j = 0; j <= nWidth; j += N) { // max: 464
            //if (i >= N)
            //    i -= N;
            i = i >= N ? i - N : i;
            for (y = 0; y < N; y++, i++) {
                if (j % N == 0 && j != 0) {
                    j -= N;
                }
                for (x = 0; x < N; x++, j++) { // i and j are wrong
                    /* Check if boundaries are exceeded */
                    if (i > nHeight || j > nWidth)
                        continue;
                    data[nBlock].element[y][x] = image[i][j];
                }
            }
            nBlock++;
        }
    }

    free(buffer);
    printf("i: %d j: %d\n", i, j);
    printf("Blocks transferred\n");

    return data;

    /* Print first and last block */
    //print_first_last(data, nBlocks);
    /*
    if ((outFile = fopen(outputFile, "wb")) == NULL) {
        printf("Could not open file: %s\n", outputFile);
    }

    fprintf(outFile, "P5\n");
    fprintf(outFile, "%d %d\n", *width, *height);
    fprintf(outFile, "%d\n", *max);

    fwrite(buffer, sizeof(unsigned char), (*width) * (*height), outFile);
    fclose(outFile); */
}

