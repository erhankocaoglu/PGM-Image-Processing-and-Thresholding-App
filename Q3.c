#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// I set the function parameters as they given in the pdf file
// char *path: path of pgm file
// int *m: colums of pgm file
// int *n: rows of pgm file
void createRandomPGM(const char *path, int m, int n) {
    // Opens the file in writing mode
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("Error!! The pgm file could not opened"); // a condition for not being able to open the file
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

    // starts random number generater by srand function
    unsigned int current_time = (unsigned int)time(NULL);
    srand(current_time);


    // this loop is for creating completely random values
    for (int i = 0; i < n * m; i++) {
        int pixel = rand() % 256; // creates a random value between 0-255
         fprintf(file, "%d\n", pixel); // prints it with a newline for the correct pgm format
        }


    
    fclose(file); // closes the file
    // if the file is not closed that means a error happened
    printf("Random PGM file is created : %s\n", path);
}

int main() {
    // in the pdf, it is stated that we need to create a function that has random pixel values not columns and rows
    // and because of that I am setting columns and rows manually
    int m = 10; // number of columns
    int n = 6;  // number of rows
    const char *path = "output_of_createRandomPGM.pgm";

    createRandomPGM(path, m, n);

    return 0;
}
