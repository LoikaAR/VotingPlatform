#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "image.h"
#include "texture_synthesis.h"

void print_usage(const char *bin_name) {
    printf("Usage: %s -i sample.ppm -o output.ppm -s size\n", bin_name);
}

int main(int argc, char **argv) {
    char *sample_path = NULL;
    char *output_path = NULL;
    int size = 0;

    int opt = 0;
    while ((opt = getopt(argc, argv, "i:o:s:")) != -1) {
        switch (opt) {
            case 'i':
                sample_path = optarg;
                break;
            case 'o':
                output_path = optarg;
                break;
            case 's':
                size = atoi(optarg);
                break;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    if (!sample_path || !output_path || size <= 0) {
        fprintf(stderr, "Missing required arguments.\n");
        print_usage(argv[0]);
        return 1;
    }

    clock_t start, end;

    start = clock();

    Image sample = load_ppm_image(sample_path);

    Image Gs = texture_synthesis(sample, size, 5);

    save_ppm_image(Gs, output_path);

    end = clock();
    double cpu_time_elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;

    int hours = (int)(cpu_time_elapsed / 3600);
    int minutes = (int)((cpu_time_elapsed - (hours * 3600)) / 60);
    int seconds = (int)(cpu_time_elapsed - (hours * 3600) - (minutes * 60));

    if (hours > 0) {
        printf("\nAll done :) Time taken: %dh %dm %ds.\n", hours, minutes,
               seconds);
    } else {
        printf("\nAll done :) Time taken: %dm %ds.\n", minutes, seconds);
    }

    free(sample.data);  // Free input image memory
    free(Gs.data);      // Free synthesized image memory

    return 0;
}