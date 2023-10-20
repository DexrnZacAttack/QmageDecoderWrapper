#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Dexrn -----
// TODO: Maybe make it output to a folder instead, maybe as a setting.
// TODO(maybe): Add more header types, if there are more found... which means I have to learn how to make it check for multiple, maybe ill pull a funny on line 69 (nice)
// also I had to remove/change some of the code comments as maybe having the ai try to add code comments is a horrible thing
// Someone says this code looks like trash when formatted by the AI, but they don't know what it used to look like :tf:
// I hate the variable name newFilename
// Maybe one day I'll get better at C/C++. Also this code was made at around 5 in the morning :)


bool FlateIt(const char *inputFile, const char *outputFile) {
    char command[256];
    // this runs the zlib-flate because for some reason it is the only thing that will actually flate' it, I've tried python to no avail lmao.
    snprintf(command, sizeof(command), "zlib-flate -uncompress < %s > %s", inputFile, outputFile);

    int status = system(command);

    if (status != 0) {
        // This is to be expected, unless we can find a fix for some files not being valid.
        fprintf(stderr, "zlib-flate encountered an error with file %s\n", inputFile);
        return false;
    }

    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_filename>\n", argv[0]);
        return 1;
    }
    // Was inputFile really a good name? I had the ai help with something and it created that so I just stuck with it.
    const char *inputFilename = argv[1];
    FILE *inputFile = fopen(inputFilename, "rb");

    if (inputFile == NULL) {
        perror("Error opening file");
        return 2;
    }

    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);
    rewind(inputFile);

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

    // Look for the header (78 DA), which is the header of interest here as QMG uses it for ZLib.
    int headerCount = 0;
    for (long i = 0; i < fileSize - 1; i++) {
        if (fileBuffer[i] == 0x78 && fileBuffer[i + 1] == 0xDA) {
            headerCount++;
            // this should cut out and create a file for every header found, seems like it works.
            char newFilename[strlen(inputFilename) + 16];
            snprintf(newFilename, sizeof(newFilename), "%s-zlib%d", inputFilename, headerCount);

            FILE *outputFile = fopen(newFilename, "wb");
            if (outputFile == NULL) {
                perror("Error creating file");
                free(fileBuffer);
                return 5;
            }

            fwrite(fileBuffer + i, 1, fileSize - i, outputFile);
            fclose(outputFile);

            char flateFilename[strlen(newFilename) + 13];
            snprintf(flateFilename, sizeof(flateFilename), "%s-flate", newFilename);
            if (!FlateIt(newFilename, flateFilename)) {
            }

            printf("Extracted + flate'd data to %s\n", flateFilename);
        }
    }

    if (headerCount == 0) {
        // Maybe we should add more header types?
        printf("No ZLib headers found.\n");
    } else {
        printf("Found %d ZLib headers.\n", headerCount);
    }

    free(fileBuffer);
    return 0;
}
