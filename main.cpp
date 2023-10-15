#include <qmg.h>
#include <iostream>
#include <filesystem>

void check_error() {
    QmageDecoderError decoderError = QmageDecGetLastErr();
    if (decoderError != 0) {
        std::cout << decoderError << std::endl;
    }
}

// To the poor soul who looks at this
// I do not know C/C++ in my defense
int main() {
    loadLibrary();
    const char* exampleQmg = "../examples/bugdroid.qmg";
    
    long long int input_size = std::filesystem::file_size(exampleQmg);
    int version = QmageDecCommon_GetVersion();
    int opaqueInfo = QmageDecCommon_GetOpaqueInfo(exampleQmg);
    std::cout << "QmageDecoder version:" << version << "\n";
    std::cout << "QmageDecoder opaqueInfo:" << opaqueInfo << "\n";
    
    QmageDecoderInfo decoderInfo{};
    QM_BOOL hasDecoderInfo = QmageDecCommon_GetDecoderInfo(exampleQmg, 16, &decoderInfo);
    check_error();
    
    if (hasDecoderInfo) {
        std::cout << "DecoderInfo mode: " << decoderInfo.mode << "\n";
        std::cout << "DecoderInfo bpp: " << decoderInfo.imageInfo.bpp << "\n";
    }
    
    QmageDecoderHeader headerInfo{};
    QM_BOOL hasHeaderInfo = QmageDecCommon_ParseHeader(exampleQmg, QM_IO_BUFFER, input_size, &headerInfo);
    check_error();
    
    if (hasHeaderInfo) {
        std::cout << "HeaderInfo width: " << headerInfo.width << "\n";
        std::cout << "HeaderInfo height: " << headerInfo.height << "\n";
    }

    unloadLibrary();
    return 0;
}