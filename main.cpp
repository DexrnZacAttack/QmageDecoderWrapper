#include <qmg.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "stb_image_write.h"

// Converts an image in RGB565 format to RGB888 format
static void convertRGB565ToRGB888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
    for (size_t i = 0; i < amountPixels; i++) {
        unsigned short read = *(input++);
        read |= *(input++) << 8;
        unsigned long rgb = (read & 0x1f) << 3 | (read & 0xf800) << 8 | (read & 0x7e0) << 5;
        *(output++) = (rgb >> 16) & 0xff;
        *(output++) = (rgb >> 8) & 0xff;
        *(output++) = rgb & 0xff;
    }
}

// Returns the amount of bits needed to store an image of this type
static int typeToBits(QmageRawImageType type) {
    switch (type) {
        case QM_RAW_RGB565:
            return 16;
        case QM_RAW_RGB888:
        case QM_RAW_BGR888:
        case QM_RAW_RGBA5658:
        case QM_RAW_ARGB8565:
            return 24;
        case QM_RAW_ARGB8888:
        case QM_RAW_RGBA8888:
        case QM_RAW_BGRA8888:
            return 32;
        default:
            return 0;
    }
}

void check_error(const char* message) {
    QmageDecoderError decoderError = QmageDecGetLastErr();
    if (decoderError != 0) {
        std::cerr << "[" << message << "] " << decoderError << std::endl;
    }
}

// Zero: To the poor soul who looks at this
// Zero: I do not know C/C++ in my defense
int main(int argc, char* argv[]) {
    std::string filename;
    bool raw = false;
    bool quiet = false;

    // Dexrn: Commands! (hopefully)
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: " << argv[0] << " [options] <file>" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  -h, --help: Display this help message" << std::endl;
            std::cout << "  -r, --raw: Files are written as raw pixels instead of PNG, skipping the encoding process." << std::endl;
            std::cout << "  -q, --quiet: Doesn't stdcout every written file." << std::endl; // Dexrn: I'm sure I could phrase this better lmao
            std::cout << "GitHub Repo: https://github.com/DexrnZacAttack/QmageDecoder" << std::endl;
            return 0;
        } else if (arg == "--raw" || arg == "-r") {
            raw = true;
        } else if (arg == "--quiet" || arg == "-q") {
            quiet = true;
        } else {
            // if no options are specified, assume it a file has been passed in.
            filename = arg;
        }
    }

    if (filename.empty()) {
        std::cerr << "Error: No file specified. Use --help for usage information." << std::endl;
        return 1; // Exit with an error code
    }


    int returnVal = 0;
    // const char* exampleQmg = "../examples/bootsamsung.qmg";

    std::ifstream file(filename, std::ios::binary);

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

    // long long int input_size = std::filesystem::file_size(filename);
    int version = QmageDecCommon_GetVersion();
    int opaqueInfo = QmageDecCommon_GetOpaqueInfo(filename.c_str());
    std::cout << "QmageDecoder version:" << version << std::endl;
    std::cout << "QmageDecoder opaqueInfo:" << opaqueInfo << std::endl;

    QmageDecoderHeader headerInfo;
    QmageDecAniInfo* aniInfo = nullptr;
    const char* frameBuffer = nullptr;
    size_t dimSize = 0;
    size_t frameSize = 0;
    int stride = 0;

    // Read the header
    QM_BOOL hasHeaderInfo = QmageDecParseHeader(buffer, QM_IO_BUFFER, fileSize, &headerInfo);
    check_error("QmageDecParseHeader");

    if (!hasHeaderInfo) {
        std::cerr << "Error: QmageDecParseHeader failed" << std::endl;
        returnVal = 1;
        goto cleanup;
    }

    std::cout << "HeaderInfo mode: " << headerInfo.mode << std::endl;
    std::cout << "HeaderInfo width: " << headerInfo.width << std::endl;
    std::cout << "HeaderInfo height: " << headerInfo.height << std::endl;
    std::cout << "HeaderInfo padding: " << headerInfo.padding << std::endl;
    std::cout << "HeaderInfo raw_type: " << headerInfo.raw_type << std::endl;
    std::cout << "HeaderInfo transparency: " << headerInfo.transparency << std::endl;
    std::cout << "HeaderInfo NinePatched: " << headerInfo.NinePatched << std::endl;
    std::cout << "HeaderInfo IsOpaque: " << headerInfo.IsOpaque << std::endl;
    std::cout << "HeaderInfo premultiplied: " << headerInfo.premultiplied << std::endl;
    std::cout << "HeaderInfo ColorCount: " << headerInfo.ColorCount << std::endl;
    std::cout << "HeaderInfo UseIndexedColor: " << headerInfo.UseIndexedColor << std::endl;
    std::cout << "HeaderInfo isGrayColor: " << headerInfo.isGrayColor << std::endl;
    std::cout << "HeaderInfo rgba_order: " << headerInfo.rgba_order << std::endl;
    std::cout << "HeaderInfo totalFrameNumber: " << headerInfo.totalFrameNumber << std::endl;
    std::cout << "HeaderInfo currentFrameNumber: " << headerInfo.currentFrameNumber << std::endl;
    std::cout << "HeaderInfo Animation_delaytime: " << headerInfo.Animation_delaytime << std::endl;
    std::cout << "HeaderInfo Animation_NoRepeat: " << headerInfo.Animation_NoRepeat << std::endl;

    // TODO What is the correct way to check for animation?
    if (headerInfo.mode != 0 || headerInfo.totalFrameNumber > 1) {
        std::cout << "Image is animated" << std::endl;;

        // Set up the animation decoding context
        aniInfo = QmageDecCreateAniInfo((void*) buffer, QM_IO_BUFFER, fileSize);
        check_error("QmageDecCreateAniInfo");

        if (aniInfo == nullptr) {
            std::cerr << "Error: QmageDecCreateAniInfo failed" << std::endl;;
            returnVal = 1;
            goto cleanup;
        }
    }

    // Create buffer to decode into
    dimSize = headerInfo.width * headerInfo.height;
    stride = typeToBits(headerInfo.raw_type) / 8;
    frameSize = dimSize * stride;
    frameBuffer = new char[frameSize];

    if (aniInfo != nullptr) {
        // NedTheNerd: TODO: Allow configuring this
        // Dexrn: Did it.
#ifndef RAW_OUTPUT
        bool convert565to888 = !raw && (headerInfo.raw_type == QM_RAW_RGB565);

        if (convert565to888) {
            std::cout << "Image will be converted from to RGB565 to RGB888" << std::endl;
        }
#endif

        for (int i = 0; i < headerInfo.totalFrameNumber; i++) {
            // NedTheNerd: Decode TODO handle errors
            //int aniDecodeReturn = QmageDecodeAniFrame(aniInfo, frameBuffer);
            //std::cout << "QmageDecodeAniFrame return value: " << aniDecodeReturn << std::endl;
            QmageDecodeAniFrame(aniInfo, frameBuffer);
            check_error("QmageDecodeAniFrame");

            // Choose between raw decoded output or converting it if needed
            char* outBuffer;
            size_t outBufferSize;

            if (convert565to888) {
                outBufferSize = dimSize * 3;
                outBuffer = new char[outBufferSize];
                convertRGB565ToRGB888((const unsigned char*) frameBuffer, (unsigned char*) outBuffer, dimSize);
            } else {
                outBuffer = (char*) frameBuffer;
                outBufferSize = frameSize;
            }

            int fileNum = i + 1;
            std::string fileOutName;

            if (raw) {
                std::ofstream fos;
                fileOutName = "frame-" + std::to_string(fileNum) + ".raw";
                fos.open(fileOutName, std::ios::binary | std::ios::out);
                fos.write(outBuffer, outBufferSize);
                fos.close();
            } else {
                fileOutName = "frame-" + std::to_string(fileNum) + ".png";
                int channels = convert565to888 ? 3 : stride;
                stbi_write_png(fileOutName.c_str(), headerInfo.width, headerInfo.height, channels, outBuffer, headerInfo.width * channels);
            }

    if (!quiet) {
        std::cout << "Wrote frame " << fileNum << " to " << fileOutName << std::endl;
    }
    

        }
    } else {
        std::cerr << "Error: TODO support for non-animated images" << std::endl;;
        returnVal = 1;
        goto cleanup;
    }

        std::cout << "Successfully decoded " << headerInfo.totalFrameNumber << " frames from " << filename << std::endl;

cleanup:

    if (aniInfo != nullptr) {
        QmageDecDestroyAniInfo(aniInfo);
    }

    return returnVal;
}
