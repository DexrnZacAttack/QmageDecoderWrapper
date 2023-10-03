#include <stdio.h>
#include <qmg.hpp>

long int getFileSize(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Seek to the end of the file
    if (fseek(file, 0, SEEK_END) != 0) {
        perror("Error seeking to the end of the file");
        fclose(file);
        return -1;
    }

    long int fileSize = ftell(file);
    if (fileSize == -1) {
        perror("Error getting file size");
        fclose(file);
        return -1;
    }

    fclose(file);

    return fileSize;
}

// To the poor soul who looks at this
// I do not know C/C++ in my defense
int main(void) {
    printf("WE ARE SO FUCKING BALL!\n");    

    loadLibrary();
    const char* exampleQmg = "../examples/example.qmg";
    int version = QmageDecCommon_GetVersion();
    printf("QmageDecoder version: %d\n", version);
    int opaqueInfo = QmageDecCommon_GetOpaqueInfo(exampleQmg);
    printf("QmageDecoder opaqueInfo: %d\n", opaqueInfo);
    QmageDecoderInfo decoderInfo{};
    QM_BOOL hasDecoderInfo = QmageDecCommon_GetDecoderInfo(exampleQmg, getFileSize(exampleQmg), &decoderInfo);
    if (hasDecoderInfo) {
        printf("DecoderInfo mode: %d\n", decoderInfo.mode);
        printf("DecoderInfo bpp: %d\n", decoderInfo.imageInfo.bpp);
    }
    unloadLibrary();

    return 0;
}