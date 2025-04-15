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


void calculateHistogram(unsigned char *image, int size, int *histogram) {
    // creates an empty histogram(by empty I mean every value is equal to 0)
    for (int i = 0; i < 256; i++) {
        histogram[i] = 0;}
        

    for (int i = 0; i < size; i++) {
        // if pixel values are not between 0-255 throws error
        if (!(image[i] >= 0 && image[i] <= 255)) {
            printf("Error: Pixel value out of range at index %d\n", i);}
            // increases histogram value by one if it exists
            int x = image[i];
            histogram[x]++;}
        }




int otsuThreshold(const int* histogram, int totalPixels) {
    // total sum of weight of ALL pixel values 255,i=0∑(i×histogram[i])
    float sum = 0;
    // total sum of weight of pixel values until i: t,i=0∑(i×histogram[i])
    float sumB = 0;
    // total sum of pixel values until t, t,i=0∑(histogram[i])
    int q1 = 0;
    // q2=totalPixels−q1
    int q2 = 0;
    // q1 x q2 x (m1-m2)^2
    float varMax = 0;
    int threshold = 0;

    // a loop to calculate sum
    for (int i = 0; i <= 255; i++) {
        sum += i * histogram[i];
    }

    // a loop to calculate sumB,q1,q2,m1,m2,varMax,threshold
    int t=0;
    while (t <= 255) { 
        // if pixel values are not between 0-255 throws error
        q1 += histogram[t];
        if (q1 == 0) { 
            t++;
            continue;
        }

        q2 = totalPixels - q1; 
        if (q2 == 0) { 
            break;
        }

        sumB += (float)(t * histogram[t]); 

        // m1 = sumb/q1
        float m1 = sumB / q1;

        // m2 = (sum-sumb)/q2
        float m2 = (sum - sumB) / q2;

        // q1 x q2 x (m1-m2)^2
        float varBetween = (float)q1 * (float)q2 * (m1 - m2) * (m1 - m2);

        if (varBetween > varMax) { 
            varMax = varBetween;
            threshold = t;
        }

        t++; 
    }


    return threshold;
}


// in the previous function we calculated and returned threshold, in this function it is being implementing
void otsu(unsigned char* path_in, unsigned char* path_out, int m, int n, int threshold) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int index = i * m + j;
            //turns pixel values 15 or 0
            path_out[index] = (path_in[index] > threshold) ? 15 : 0;
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
    // I put table 1.2 from the pdf file inside x.pgm to test it and see if result is equal to table 1.3 from the pdf
    char *path = "images/x.pgm";
    int m;
    int n;
    int v;
    unsigned char *image = readPGM(path, &m, &n, &v);

        // a condition for if it reads the image sucessfully
    if (image) { 
        printf("Dimensions are  : %d x %d\n", m, n); 
        printf("Max pixel value is : %d\n\n", v); 
        // creates a histogram that where it will hide frequency of pixel values
        int histogram[256]; 
        calculateHistogram(image, m * n, histogram); 
        int threshold = otsuThreshold(histogram, m * n); 
        // a condition for if otsu threshold value of the image is valid(!= -1)
        if (threshold != -1) { 
            printf("The otsu Threshold Value of The Image: %d\n", threshold); 
            unsigned char *path_out;  
            size_t size = m * n * sizeof(unsigned char);  
            path_out = (unsigned char *)malloc(size);

            if (!path_out) { 

                fprintf(stderr, "Error: Memory allocation failed\n"); 
                // if allocating memory fails throws an error
                free(image); 
                // frees the allocated memoru
                return -1; 
                // ends the process by returnin -1
            }



            otsu(image, path_out, m, n, threshold);
            writePGM("thresholded_output.pgm", path_out, m, n);
            printf("Thresholded image saved as thresholded_output.pgm\n");
            free(path_out);
        }
        free(image);
    } else {
        fprintf(stderr, "Error: Failed to load image\n");
    }

    return 0;
}
