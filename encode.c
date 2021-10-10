#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LEN 10
// Figure out a way to write the value 2 in 7 bits
// Also needs to write 5 in 10 bits

void WriteKBits(int value, int num_of_bits) {
    // Write the given value in binary using
    // the given number of bits
    int c, k;
    for (c = num_of_bits-1; c >= 0; c--) {
        k = value >> c;
        if (k & 1)
            printf("1");
        else
            printf("0");
    }
}

int ReadKBits(int num) {
    // Read given number of bits
    // Return the value of the bits
}

void parseLine(char *line, int value, int occurrence) {
    sscanf(line, "%d %d \n", &value, &occurrence);
    int sign = value >= 0 ? 0 : 1;
    value = sign == 1 ? value * -1 : value;
    WriteKBits(sign, 1);
    WriteKBits(occurrence - 1, 6);
    WriteKBits(value, 10);
    printf("\n");
}

int main(int argc, char **argv) {
    int value, occurrence;
    int bits = 9;
    char line[LEN], *temp;
    if (argc < 2) {
        printf("Please input filename\n");
        return 1;
    }
    char *filename = argv[1];
    FILE *fp;
    if ((fp = fopen(filename, "r")) == NULL) {
        printf("Could not open file %s\n", filename);
    }
    while (fgets(line, LEN, fp) != NULL) {
        //printf("string = %s\n", line);
        parseLine(line, value, occurrence);
        // Write binary values to buffer in decimal
        // Write buffer to file in binary
        // flush the buffer
    }
    printf("\n");
    fclose(fp);
    return 0;
}

