#include "io_helper.h"

#include <stdio.h>

bool doesFileExist(const char* fileName) {
    FILE* output_file = fopen(fileName, "rb");

    if (output_file != NULL) {
        fclose(output_file);
        return true;
    }

    return false;
}

bool writeBytesToFile(const char* fileName, char* data, size_t size, bool overwrite) {
    if (!overwrite && doesFileExist(fileName)) {
        return false;
    }

    FILE* output_file = fopen(fileName, "wb");

    if (output_file == NULL) {
        return false;
    }

    bool returnVal = false;

    if (fwrite(data, size, 1, output_file) == 1) {
        returnVal = true;
    }

    fclose(output_file);

    return returnVal;
}
