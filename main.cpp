#include <cstdio>
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
int main() {
    loadLibrary();
    const char* exampleQmg = "../examples/example.qmg";
    int version = QmageDecCommon_GetVersion();
    printf("QmageDecoder version: %d\n", version);
    int opaqueInfo = QmageDecCommon_GetOpaqueInfo(exampleQmg);
    printf("QmageDecoder opaqueInfo: %d\n", opaqueInfo);
    long int input_size = getFileSize(exampleQmg);
    QmageDecoderInfo decoderInfo{};
    QM_BOOL hasDecoderInfo = QmageDecCommon_GetDecoderInfo(exampleQmg, 16, &decoderInfo);
    if (hasDecoderInfo) {
        printf("DecoderInfo mode: %d\n", decoderInfo.mode);
        printf("DecoderInfo bpp: %d\n", decoderInfo.imageInfo.bpp);
    }
    QmageDecoderHeader headerInfo{};
    QM_BOOL hasHeaderInfo = QmageDecCommon_ParseHeader(exampleQmg, QM_IO_FILE, 16, &headerInfo);
    if (hasHeaderInfo) {
        printf("HeaderInfo width: %d\n", headerInfo.width);
        printf("HeaderInfo height: %d\n", headerInfo.height);
    }
    unloadLibrary();

    return 0;
}