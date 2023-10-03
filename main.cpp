#include <qmg.h>
#include <iostream>
#include <filesystem>

// To the poor soul who looks at this
// I do not know C/C++ in my defense
int main() {
    loadLibrary();
    const std::filesystem::path exampleQmg = "../examples/example.qmg";
    
    int version = QmageDecCommon_GetVersion();
    int opaqueInfo = QmageDecCommon_GetOpaqueInfo(exampleQmg.c_str());
    std::cout << "QmageDecoder version:" << version << "\n";
    std::cout << "QmageDecoder opaqueInfo:" << opaqueInfo << "\n";
    
    long long int input_size = std::filesystem::file_size(exampleQmg.c_str());
    QmageDecoderInfo decoderInfo{};
    QM_BOOL hasDecoderInfo = QmageDecCommon_GetDecoderInfo(exampleQmg.c_str(), 16, &decoderInfo);
    
    if (hasDecoderInfo) {
        std::cout << "DecoderInfo mode: " << decoderInfo.mode << "\n";
        std::cout << "DecoderInfo bpp: " << decoderInfo.imageInfo.bpp << "\n";
    }
    
    QmageDecoderHeader headerInfo{};
    QM_BOOL hasHeaderInfo = QmageDecCommon_ParseHeader(exampleQmg.c_str(), QM_IO_FILE, 16, &headerInfo);
    
    if (hasHeaderInfo) {
        std::cout << "HeaderInfo width: " << headerInfo.width << "\n";
        std::cout << "HeaderInfo height: " << headerInfo.height << "\n";
    }

    unloadLibrary();
    return 0;
}