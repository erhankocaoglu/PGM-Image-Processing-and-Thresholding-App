#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

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


void add_saltpepper(double pnoise, unsigned char *data, int ncols, int nrows, int maxval, unsigned saltpepper) {
    int totalPixels = ncols * nrows; 
    // calculates total pixel value which is equal to rows x columns

    int Noise_to_add = (int)(pnoise * totalPixels); 
    // by using pnoise value it calculates the image noise that will be added to pgm

    for (int i = 0; i < Noise_to_add; i++) {
        int randomRow, randomCol, idx;
        
        // Generate random row and column
        randomRow = rand() % nrows; 
        randomCol = rand() % ncols; 

        // Calculate the index
        idx = randomRow * ncols + randomCol; 

        if (saltpepper == 0) { 
            // Updates the pixels and adds salt (white pixel)
            data[idx] = maxval; 
        } else if (saltpepper == 1) { 
            // Updates the pixels and adds pepper (black pixel)
            data[idx] = 0; 
        } else { 
            // Updates the pixels and adds salt and pepper (white-black pixels)
            if (rand() % 2 == 0) {
                data[idx] = maxval; 
            } else {
                data[idx] = 0; 
            }
        }
    }
}




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
    // the input 
    char *path = "images/img0.pgm"; 
    int columns, rows, maxVal;
    unsigned char *image = readPGM(path, &columns, &rows, &maxVal);

    if (image) {
        printf("Dimensions: %d x %d\n", columns, rows);
        printf("Max pixel value: %d\n\n", maxVal);

        // Adding salt
        unsigned char *saltImage = (unsigned char *)malloc(columns * rows * sizeof(unsigned char));
        memcpy(saltImage, image, columns * rows);
        add_saltpepper(0.05, saltImage, columns, rows, maxVal, 0);
        writePGM("salt_output.pgm", saltImage, columns, rows);
        printf("Salt noise added and saved to salt_output.pgm\n");

        // Adding pepper
        unsigned char *pepperImage = (unsigned char *)malloc(columns * rows * sizeof(unsigned char));
        memcpy(pepperImage, image, columns * rows);
        add_saltpepper(0.05, pepperImage, columns, rows, maxVal, 1);
        writePGM("pepper_output.pgm", pepperImage, columns, rows);
        printf("Pepper noise added and saved to pepper_output.pgm\n");

        // Adding salt and pepper
        unsigned char *saltPepperImage = (unsigned char *)malloc(columns * rows * sizeof(unsigned char));
        memcpy(saltPepperImage, image, columns * rows);
        add_saltpepper(0.05, saltPepperImage, columns, rows, maxVal, 2);
        writePGM("saltpepper_output.pgm", saltPepperImage, columns, rows);
        printf("Salt and pepper noise added and saved to saltpepper_output.pgm\n");

        // free the allocated memory
        free(saltImage);
        free(pepperImage);
        free(saltPepperImage);
        free(image);
    } else {
        fprintf(stderr, "Error!! Failed to load the pgm file\n");
    }

    return 0;
}
