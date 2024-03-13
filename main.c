#include "file.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Invalid inputs detected, please enter input and output file!");
        return 1;
    }

    FILE *fp;                 /* file pointer     */
    char line[LEN], ch;
    int format, width, height, max;
    char *filename = argv[1]; /* filename to open */
    char *output   = argv[2]; /* output filename  */

    /* Open file for reading */
    if ((fp = fopen(filename, "r")) == NULL) {
        printf("Could not open file: %s!\n", filename); // We have the file opened
        return 1;
    }

    fgets(line, LEN, fp);

    if (line[1] == '2')
        format = 2;
    else if (line[1] == '5')
        format = 5;
    else
        format = 6;

    fgets(line, LEN, fp);
    while (line[0] == '#')
        fgets(line, LEN, fp);
    sscanf(line, "%d %d\n", &width, &height);
    fgets(line, LEN, fp);
    sscanf(line, "%d\n", &max);
    printf("Format: P%d\n", format);
    printf("Width: %d, Height %d\n", width, height);
    printf("Max Value: %d\n", max);

    BLOCK2 *img;
    if (format == 2) {
	    printf("format of 2");
	    img = readP2PGM(fp, &width, &height, &max, output);
	    print_first_last(img, nBlocks);
	    writeP5PGM(output, &width, &height, &max, img);
    }
    else if (format == 5) {
	    printf("Format of 5");
            img = readP5PGM(fp, &width, &height, &max, output);
	    print_first_last(img, nBlocks);
	    writeP5PGM(output, &width, &height, &max, img);
    }
    else {
	    printf("format of 6");
	    img = readP6PGM(fp, &width, &height, &max, output);
	    //print_first_channel(img, nBlocks * 3);
    }

    free(img);

    //print_first_last(img, 2610);
    //writeP5PGM(output, &nWidth, &nHeight, &max, img);



    return 0;
}
