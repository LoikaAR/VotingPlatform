#include <stdio.h>

#include <stdlib.h>

unsigned char* readPPM(const char* fileName, int* width, int* height) {
    
}

int main(){
    FILE* image = fopen("./images/blackAndWhiteP1File.ppm", "r");
    char header[3];
    int width, height, max_color; // max_color = #channels?
    fscanf(image,"%s", header);
    fscanf(image, "%s %s", &width, &height);
    fscanf(image, "%s", &max_color);


    // check header format
    if (header[0] != 'P' || header[1] != '3') {
        printf("Invalid file format.\n");
        return 1;
    }

    

    int size = width * height * 3;

}