#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#ifdef _WIN32
    #include <direct.h>
    #define mkdir(path, mode) _mkdir(path)
#else
#include <unistd.h>
#endif

void create_directory(const char *path)
{
    if (mkdir(path, 0777) != 0 && errno != EEXIST) {
        perror("Failed to create directory");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <height> <width>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Convert arguments to integers
    unsigned int height = (unsigned int) atoi(argv[1]);
    unsigned int width = (unsigned int) atoi(argv[2]);

    // Create a directory
    const char *directory = "imagegen";
    create_directory(directory);

    // Generate the filename
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/image_%u_%u.txt", directory, height, width);

    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    // Initialize random number generator
    srand(time(NULL));

    // Write to the file
    fprintf(file, "%u %u\n", height, width);

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            fprintf(file, "%d ", rand() % 2);
        }
        fprintf(file, "\n");
    }

    fclose(file);

    printf("Image '%s' created successfully.\n", filename);
    return EXIT_SUCCESS;
}