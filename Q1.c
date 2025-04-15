
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// In order to print a pgm file we need to set colors for the numbers that are in the pgm file type between 255-0
// this function prints pgm file with colors.
void print_colored_pixel(int value) {
    if (value == 0) {
        printf("\033[30m█\033[0m"); //  BLACK
    } else if (value >= 1 && value <= 49) {
        printf("\033[38;5;235m█\033[0m"); // REALLY DARK GREY
    } else if (value >= 50 && value <= 99) {
        printf("\033[38;5;236m█\033[0m"); //  DARK GREY
    } else if (value >= 100 && value <= 149) {
        printf("\033[38;5;241m█\033[0m"); // A LITTLE DARK GREY
    } else if (value >= 150 && value <= 199) {
        printf("\033[38;5;245m█\033[0m"); // GREY
    } else if (value >= 200 && value <= 249) {
        printf("\033[38;5;249m█\033[0m"); // LIGHT GREY
    } else if (value >= 250 && value <= 254) {
        printf("\033[38;5;252m█\033[0m"); // REALLY LIGHT GREY
    } else if (value == 255) {
        printf("\033[97m█\033[0m"); // WHITE
    } else {
        printf(" "); // a condition for unexpected terms
    }
}

// I set the function parameters as they given in the pdf file
// char *path: path of pgm file 
// int *m: colums of pgm file
// int *n: rows of pgm file
// int *v: maximum value of pixels


unsigned char* readPGM(char *path, int *m, int *n, int *v) {
    FILE *file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Error!! Could not open file :  %s\n", path); // a condition for not being able to open the file
        return NULL; // ends the process by returning NULL
    }

    // In here we are allocating enough memory for the pointer
    size_t buffer_size = 256;    
    char *line;                               
    line = (char *)malloc(buffer_size * sizeof(char)); 


    if (!line) {
        fprintf(stderr, "Error!! Memory allocation for *line failed!!\n"); // a condition for not being able to allocate memory
        fclose(file);
        return NULL; // ends the process by returning NULL
    }

    
    // A condition to read the first line of the pgm file where its type "P2" is written
    if (!fgets(line, 256, file)) {
        fprintf(stderr, "Error!! Failed to read header of the file\n"); // a condition for not being able to open the file
        free(line); // freeing the allocated memory to using it later
        fclose(file); // closing the file
        return NULL; // ends the process by returning NULL
    }
    if (strncmp(line, "P2", 2) != 0) {
        fprintf(stderr, "Error!! Unvalid PGM format. Only P2 is supported.\n"); // if type of pgm file is not P2 throws an error
        free(line); // freeing the allocated memory to using it later
        fclose(file); // closing the file
        return NULL; // ends the process by returning NULL
    }

    // due to second line of pgm file is allocated for comments, we are skipping it
    do {
        if (!fgets(line, 256, file)) {
            fprintf(stderr, "Error!! Unexpected situation!! \n"); // If file ends there throws an error
            free(line); // freeing the allocated memory to using it later
            fclose(file); // closing the file
            return NULL; // ends the process by returning NULL
        }
    } while (line[0] == '#'); // skips the line of comment(2. line)

    
    // we set m = columns and n = rows already
    // by using "m n" pattern we are reading the dimension of the pgm file
    if (sscanf(line, "%d %d", m, n) != 2) { 
        fprintf(stderr, "Error!! Failed to read dimensions of the image\n"); // If it fails to read the dimensions, throws an error
        free(line); // freeing the allocated memory to using it later
        fclose(file); // closing the file
        return NULL; // ends the process by returning NULL
    }

    // Reading the maximum value of pixels that is written on the fourth line
    if (!fgets(line, 256, file) || sscanf(line, "%d", v) != 1) { // reading the maximum pixel value that is an integer
        fprintf(stderr, "Error!! Failed to read max pixel value of the pgm file\n"); // If it fails to read the maximum pixel value, throws an error
        free(line); // freeing the allocated memory to using it later
        fclose(file); // closing the file
        return NULL; // ends the process by returning NULL
    }

    // Allocatimg enough memory for the image data
    size_t total_size = (*m) * (*n);
    size_t element_size;                           
    element_size = sizeof(unsigned char);         

    size_t total_allocation_size;                
    total_allocation_size = total_size * element_size; 

    unsigned char *imageData;                      
    imageData = (unsigned char *)malloc(total_allocation_size); 


    if (!imageData) {
        fprintf(stderr, "Error!! Memory allocation is failed\n"); // If it fails to allocate memoru, throws an error
        free(line); // freeing the allocated memory to using it later
        fclose(file); // closing the file
        return NULL; // ends the process by returning NULL
    }

    // Reading pixel data of file by loop
    for (int i = 0; i < (*m) * (*n); i++) {
        int pixel; // in order to read every pixel value it set a variable
        
        if (fscanf(file, "%d", &pixel) != 1) {
            fprintf(stderr, "Error!! Unexpected situation for pixel data\n"); // If pixel values has a unexpected value throws an
            free(imageData); // freeing the allocated memory to using it later
            free(line); // freeing the allocated memory to using it later
            fclose(file); // closing the file
            return NULL; // ends the process by returning NULL
        }
        if (pixel < 0 || pixel > *v) {
            fprintf(stderr, "Error!! Pixel value is out of the expected range\n"); // If pixel values are not between the exptected range, throws an error
            free(imageData); // freeing the allocated memory to using it later
            free(line); // freeing the allocated memory to using it later
            fclose(file); // closing the file
            return NULL; // ends the process by returning NULL
        }
        imageData[i] = (unsigned char)pixel; // Writes the pixel values to memory space
    }

    free(line); // freeing the allocated memory to using it later
    fclose(file); // closing the file
    return imageData; // Return the pixel values
}

int main() {
    char *path = "images/fen.pgm"; // Set path/name of the pgm file
    int m, n, v; // create variables for columns rows and maximum pixel value
    unsigned char *image = readPGM(path, &m, &n, &v); // PGM dosyasını oku ve verileri al

    if (image) { // if process is succesfull
        printf(" Dimensions : %d x %d\n", m, n); 
        printf(" Max pixel value is : %d\n\n", v); 

        // Print image data with colors and pixels
        for (int i = 0; i < n; i++) { // A loop for every rows
            for (int j = 0; j < m; j++) { // A loop for every columns
                int pixel = image[i * m + j]; // calculates the pixel value
                print_colored_pixel(pixel); 
                // writes the pixel value by the color that we determined in the beggining
            }
            printf("\n"); // starts a new line
        }

        free(image); // freeing the allocated memory to using it later
    }

    return 0; // Close the program
}



