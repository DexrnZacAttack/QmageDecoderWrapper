#include <qmg.hpp>
#include <iostream>

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
    std::cout << "QmageDecoder version:" << version "\n";
    int opaqueInfo = QmageDecCommon_GetOpaqueInfo(exampleQmg);
    std::cout << "QmageDecoder opaqueInfo:" << opaqueInfo "\n";
    long int input_size = getFileSize(exampleQmg);
    QmageDecoderInfo decoderInfo{};
    QM_BOOL hasDecoderInfo = QmageDecCommon_GetDecoderInfo(exampleQmg, 16, &decoderInfo);
    if (hasDecoderInfo) {
        std::cout << "DecoderInfo mode: " << decoderInfo.mode << "\n";
        std::cout << "DecoderInfo bpp: " << decoderInfo.imageInfo.bpp << "\n";
    }
    QmageDecoderHeader headerInfo{};
    QM_BOOL hasHeaderInfo = QmageDecCommon_ParseHeader(exampleQmg, QM_IO_FILE, 16, &headerInfo);
    if (hasHeaderInfo) {
        std::cout << "HeaderInfo width: " << headerInfo.width << "\n";
        std::cout << "HeaderInfo height: " << headerInfo.height << "\n";
    }
    unloadLibrary();

    return 0;
}