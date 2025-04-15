#include <stdio.h>
// I set the function parameters as they given in the pdf file
// char *path: path of pgm file
// char *data: pixel values that are in array
// int *m: colums of pgm file
// int *n: rows of pgm file


void writePGM(char *path, unsigned char *data, int m, int n) {
    // opens the file in writing mode
    FILE *file = fopen(path, "w");
    if (!file) {
        perror("Error!! could not openned file");  // a condition for not being able to open the file
        return;
    }


    // Writes the PGM header
    fprintf(file, "P2\n");
    // Writes a comment 
    fprintf(file, "# Written by writePGM\n");
    // writes columns and rows
    fprintf(file, "%d %d\n", m, n); 
    // writes the maximum pixel value 
    fprintf(file, "255\n");         


    // Writes the pixel data
        for (int k = 0; k < n * m; k++) {
          fprintf(file, "%d\n", data[k]);}   
    // closes the file
    fclose(file);
}

int main() {
    // This is an example to write
    int m = 10, n = 10; // Dimension: rows-columns
    unsigned char data[] = { // some values that I determined
        0,1,0,222,250,0,120,0,1,0,
        0,1,0,222,250,0,120,0,1,0,
        0,1,0,222,250,0,120,0,1,0,
        0,1,0,222,250,0,120,0,1,0,
        0,1,0,222,250,0,120,0,1,0,
        0,1,0,222,250,0,120,0,1,0,
        0,1,0,222,250,0,120,0,1,0,
        0,1,0,222,250,0,120,0,1,0,
        0,1,0,222,250,0,120,0,1,0,
        0,1,0,222,250,0,120,0,1,0,
    };

    char *path = "output_of_writePGM.pgm";
    writePGM(path, data, m, n);

    printf("PGM file is written to : %s\n", path);
    return 0;
}
