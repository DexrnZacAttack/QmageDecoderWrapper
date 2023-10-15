#include <qmg.h>
#include <extra.h>
#include <iostream>
#include <filesystem>

void check_error(const char* message) {
    QmageDecoderError decoderError = QmageDecGetLastErr();
    if (decoderError != 0) {
        std::cout << "[" << message << "] " << decoderError << std::endl;
    }
}

// To the poor soul who looks at this
// I do not know C/C++ in my defense
int main() {
    loadLibrary();
    const char* exampleQmg = "../examples/example.qmg";
    
    long long int input_size = std::filesystem::file_size(exampleQmg);
    int version = QmageDecCommon_GetVersion();
    int opaqueInfo = QmageDecCommon_GetOpaqueInfo(exampleQmg);
    std::cout << "QmageDecoder version:" << version << "\n";
    std::cout << "QmageDecoder opaqueInfo:" << opaqueInfo << "\n";
    
    QmageDecoderInfo decoderInfo;
    QM_BOOL hasDecoderInfo = QmageDecCommon_GetDecoderInfo(exampleQmg, 0, &decoderInfo);
    check_error("DecoderInfo");
    
    if (hasDecoderInfo) {
        std::cout << "DecoderInfo mode: " << decoderInfo.mode << "\n";
        std::cout << "DecoderInfo bpp: " << decoderInfo.imageInfo.bpp << "\n";
    }
    
    QmageDecoderHeader headerInfo;
    QM_BOOL hasHeaderInfo = QmageDecCommon_ParseHeader(exampleQmg, QM_IO_FILE, input_size, &headerInfo);
    check_error("HeaderInfo");
    std::cout << get_extra_error() << std::endl;
    
    if (hasHeaderInfo) {
        std::cout << "HeaderInfo width: " << headerInfo.width << "\n";
        std::cout << "HeaderInfo height: " << headerInfo.height << "\n";
    }

    unloadLibrary();
    return 0;
}