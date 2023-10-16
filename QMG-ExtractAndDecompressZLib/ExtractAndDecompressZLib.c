#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool runZlibFlate(const char *inputFile, const char *outputFile) {
    char command[256];
    snprintf(command, sizeof(command), "zlib-flate -uncompress < %s > %s", inputFile, outputFile);

    int status = system(command);

    if (status != 0) {
        fprintf(stderr, "Error running zlib-flate on %s\n", inputFile);
        return false;
    }

    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_filename>\n", argv[0]);
        return 1;
    }

    const char *inputFilename = argv[1];
    FILE *inputFile = fopen(inputFilename, "rb");

    if (inputFile == NULL) {
        perror("Error opening input file");
        return 2;
    }

    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);
    rewind(inputFile);

    // Read the entire file into memory
    unsigned char *fileBuffer = (unsigned char *)malloc(fileSize);
    if (fileBuffer == NULL) {
        perror("Error allocating memory for file buffer");
        fclose(inputFile);
        return 3;
    }

    size_t bytesRead = fread(fileBuffer, 1, fileSize, inputFile);
    if (bytesRead != fileSize) {
        perror("Error reading file");
        free(fileBuffer);
        fclose(inputFile);
        return 4;
    }

    fclose(inputFile);

    // Search for ZLib headers (78 DA)
    int headerCount = 0;
    for (long i = 0; i < fileSize - 1; i++) {
        if (fileBuffer[i] == 0x78 && fileBuffer[i + 1] == 0xDA) {
            headerCount++;
            // Create a new output file for each header
            char newFilename[strlen(inputFilename) + 16];
            snprintf(newFilename, sizeof(newFilename), "%s-zlib%d", inputFilename, headerCount);

            FILE *outputFile = fopen(newFilename, "wb");
            if (outputFile == NULL) {
                perror("Error creating output file");
                free(fileBuffer);
                return 5;
            }

            fwrite(fileBuffer + i, 1, fileSize - i, outputFile);
            fclose(outputFile);

            char uncompressedFilename[strlen(newFilename) + 13];
            snprintf(uncompressedFilename, sizeof(uncompressedFilename), "%s-uncompressed", newFilename);

            // Try running zlib-flate and continue on failure
            if (!runZlibFlate(newFilename, uncompressedFilename)) {
                // Handle the error here, if needed
            }

            printf("Extracted and uncompressed data to %s\n", uncompressedFilename);
        }
    }

    if (headerCount == 0) {
        printf("No ZLib headers found in the file.\n");
    } else {
        printf("Found and saved %d ZLib headers to separate files.\n", headerCount);
    }

    free(fileBuffer);
    return 0;
}
