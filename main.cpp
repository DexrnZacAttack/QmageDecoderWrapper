#include <qmg.h>
#include <extra.h>
#include <iostream>
#include <fstream>
#include <vector>
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
    const char* exampleQmg = "../examples/bootsamsung.qmg";

    std::ifstream file(exampleQmg, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return 1;
    }

    // Get the length of the file
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Allocate a buffer to hold the file data
    const char* buffer = new char[fileSize];

    // Read the file into the buffer
    file.read(const_cast<char*>(buffer), fileSize);

    // Close the file
    file.close();

    // long long int input_size = std::filesystem::file_size(exampleQmg);
    int version = QmageDecCommon_GetVersion();
    int opaqueInfo = QmageDecCommon_GetOpaqueInfo(exampleQmg);
    std::cout << "QmageDecoder version:" << version << "\n";
    std::cout << "QmageDecoder opaqueInfo:" << opaqueInfo << "\n";
    
    QmageDecoderInfo decoderInfo;
    QM_BOOL hasDecoderInfo = QmageDecCommon_GetDecoderInfo(buffer, fileSize, &decoderInfo);
    check_error("DecoderInfo");
    
    if (hasDecoderInfo) {
        std::cout << "DecoderInfo mode: " << decoderInfo.mode << "\n";
        std::cout << "DecoderInfo bpp: " << decoderInfo.imageInfo.bpp << "\n";
    }
    
    QmageDecoderHeader headerInfo;
    QM_BOOL hasHeaderInfo = QmageDecCommon_ParseHeader(buffer, QM_IO_FILE, 16, &headerInfo);
    check_error("HeaderInfo");
    std::cout << get_extra_error() << std::endl;
    
    if (hasHeaderInfo) {
        std::cout << "HeaderInfo width: " << headerInfo.width << "\n";
        std::cout << "HeaderInfo height: " << headerInfo.height << "\n";
    }

    return 0;
}