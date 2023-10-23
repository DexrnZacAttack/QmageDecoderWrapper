#include <qmg.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

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

// Converts an image in RGBA5658 format to RGBA8888 format TODO test
static void convertRGBA5658ToRGBA8888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
    for (size_t i = 0; i < amountPixels; i++) {
        unsigned short read = *(input++);
        read |= *(input++) << 8;
        unsigned long rgb = (read & 0x1f) << 3 | (read & 0xf800) << 8 | (read & 0x7e0) << 5;
        *(output++) = (rgb >> 16) & 0xff;
        *(output++) = (rgb >> 8) & 0xff;
        *(output++) = rgb & 0xff;
        *(output++) = *(input++);
    }
}

// Converts an image in ARGB8565 format to RGBA8888 format TODO test
static void convertARGB8565ToRGBA8888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
    for (size_t i = 0; i < amountPixels; i++) {
        unsigned char a = *(input++);
        unsigned short read = *(input++);
        read |= *(input++) << 8;
        unsigned long rgb = (read & 0x1f) << 3 | (read & 0xf800) << 8 | (read & 0x7e0) << 5;
        *(output++) = (rgb >> 16) & 0xff;
        *(output++) = (rgb >> 8) & 0xff;
        *(output++) = rgb & 0xff;
        *(output++) = a;
    }
}

// Converts an image in BGR888 format to RGB888 format TODO test
static void convertBGR888ToRGB888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
    for (size_t i = 0; i < amountPixels; i++) {
        unsigned char b = *(input++);
        unsigned char g = *(input++);
        *(output++) = *(input++);
        *(output++) = g;
        *(output++) = b;
    }
}

// Converts an image in ARGB8888 format to RGBA8888 format TODO test
static void convertARGB8888ToRGBA8888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
    for (size_t i = 0; i < amountPixels; i++) {
        unsigned char a = *(input++);
        *(output++) = *(input++);
        *(output++) = *(input++);
        *(output++) = *(input++);
        *(output++) = a;
    }
}

// Converts an image in BGRA8888 format to RGBA8888 format TODO test
static void convertBGRA8888ToRGB888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
    for (size_t i = 0; i < amountPixels; i++) {
        unsigned char b = *(input++);
        unsigned char g = *(input++);
        unsigned char r = *(input++);
        unsigned char a = *(input++);
        *(output++) = r;
        *(output++) = g;
        *(output++) = b;
        *(output++) = a;
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

static std::string getFormatName(QmageRawImageType type) {
    switch (type) {
        case QM_RAW_RGB565:
            return "RGB565";
        case QM_RAW_RGB888:
            return "RGB888";
        case QM_RAW_BGR888:
            return "BGR888";
        case QM_RAW_RGBA5658:
            return "RGBA5658";
        case QM_RAW_ARGB8565:
            return "ARGB8565";
        case QM_RAW_ARGB8888:
            return "ARGB8888";
        case QM_RAW_RGBA8888:
            return "RGBA8888";
        case QM_RAW_BGRA8888:
            return "BGRA8888";
        default:
            return "???";
    }
}

static QmageRawImageType getConvertedType(QmageRawImageType type) {
    switch (type) {
        case QM_RAW_RGB565:
        case QM_RAW_BGR888:
            return QM_RAW_RGB888;
        case QM_RAW_RGBA5658:
        case QM_RAW_ARGB8565:
        case QM_RAW_ARGB8888:
        case QM_RAW_BGRA8888:
            return QM_RAW_RGBA8888;
        default:
            return type;
    }
}

static void doConvert(QmageRawImageType from, const unsigned char* input, unsigned char* output, size_t amountPixels) {
    switch (from) {
        case QM_RAW_RGB565:
            convertRGB565ToRGB888(input, output, amountPixels);
            break;
        case QM_RAW_RGBA5658:
            convertRGBA5658ToRGBA8888(input, output, amountPixels);
            break;
        case QM_RAW_ARGB8565:
            convertARGB8565ToRGBA8888(input, output, amountPixels);
            break;
        case QM_RAW_BGR888:
            convertBGR888ToRGB888(input, output, amountPixels);
            break;
        case QM_RAW_ARGB8888:
            convertARGB8888ToRGBA8888(input, output, amountPixels);
            break;
        case QM_RAW_BGRA8888:
            convertBGRA8888ToRGB888(input, output, amountPixels);
            break;
        default:
            memcpy(output, input, amountPixels * (typeToBits(from) / 8));
            break;
    }
}

void check_error(const char* message) {
    QmageDecoderError decoderError = QmageDecGetLastErr();
    if (decoderError != 0) {
        std::cerr << "[" << message << "] " << decoderError << std::endl;
    }
}

enum ImageOutputFormat {
    RAW,
    PNG,
    JPG,
    TGA,
    BMP
};

std::string getExtensionForOutputFormat(ImageOutputFormat format) {
    switch (format) {
        case PNG:
            return ".png";
        case JPG:
            return ".jpg";
        case TGA:
            return ".tga";
        case BMP:
            return ".bmp";
        default:
            return ".raw";
    }
}

bool writeImageToFile(std::string fileOutName, ImageOutputFormat format, int width, int height, int channels, char* imageData) {
    switch (format) {
        case PNG:
            return stbi_write_png(fileOutName.c_str(), width, height, channels, imageData, width * channels);
        case JPG:
            return stbi_write_jpg(fileOutName.c_str(), width, height, channels, imageData, 90);
        case TGA:
            return stbi_write_tga(fileOutName.c_str(), width, height, channels, imageData);
        case BMP:
            return stbi_write_bmp(fileOutName.c_str(), width, height, channels, imageData);
        default:
            // TODO handle errors
            std::ofstream fos;
            fos.open(fileOutName, std::ios::binary | std::ios::out);
            fos.write(imageData, width * height * channels);
            fos.close();
            return true;
    }
}

enum Verbosity {
    REALLY_QUIET,
    QUIET,
    NORMAL
};

// Zero: To the poor soul who looks at this
// Zero: I do not know C/C++ in my defense
int main(int argc, char* argv[]) {
    std::string filename;
    // Dexrn: args
    ImageOutputFormat outputFormat = PNG;
    Verbosity verbosityLevel = NORMAL;

    // Dexrn: Commands! (hopefully)
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: " << argv[0] << " [options] <file>" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  -h, --help: Display this help message" << std::endl;
            std::cout << "  -f, --format: Outputs the frames in the format specified.\n  Supported formats are: raw, png (default), jpg, tga, bmp." << std::endl;
            std::cout << "  -q, --quiet: Doesn't stdcout every written file." << std::endl;  // Dexrn: I'm sure I could phrase this better lmao
            std::cout << "  -Q, --really-quiet: Doesn't stdcout anything." << std::endl;
            std::cout << "GitHub Repo: https://github.com/DexrnZacAttack/QmageDecoder" << std::endl;
            return 0;
            // Dexrn: yes I know this looks like hell...
        } else if (arg == "--format" || arg == "-f") {
            if (i + 1 < argc) {
                std::string format = argv[i + 1];
                if (format == "raw") {
                    outputFormat = RAW;
                } else if (format == "jpg" || format == "jpeg") {
                    outputFormat = JPG;
                } else if (format == "png") {
                    outputFormat = PNG;
                } else if (format == "tga") {
                    outputFormat = TGA;
                } else if (format == "bmp") {
                    outputFormat = BMP;
                } else {
                    std::cerr << "Error: Unknown format specified: " << format << std::endl;
                    return 1;
                }
                i++;
            }
        } else if (arg == "--quiet" || arg == "-q") {
            // Dexrn: maybe quiet should be more quiet?
            verbosityLevel = QUIET;
        } else if (arg == "--really-quiet" || arg == "-Q") {
            verbosityLevel = REALLY_QUIET;
        } else {
            // Dexrn: if no options are specified, assume it a file has been passed in.
            filename = arg;
        }
    }

    if (filename.empty()) {
        std::cerr << "Error: No file specified. Use --help for usage information." << std::endl;
        return 1;  // Exit with an error code
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

    if (verbosityLevel > REALLY_QUIET) {
        // Dexrn: Changed these so that it is more clear that we are talking about the library and not QmageDecoder.
        std::cout << "libQmageDecoder version:" << version << std::endl;
        std::cout << "libQmageDecoder opaqueInfo:" << opaqueInfo << std::endl;
    }

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

    if (verbosityLevel > REALLY_QUIET) {
        std::cout << "\nQmageDecoderInfo" << std::endl;
        std::cout << "HeaderInfo mode: " << headerInfo.mode << std::endl;
        std::cout << "HeaderInfo width: " << headerInfo.width << std::endl;
        std::cout << "HeaderInfo height: " << headerInfo.height << std::endl;
        std::cout << "HeaderInfo padding: " << headerInfo.padding << std::endl;
        std::cout << "HeaderInfo raw_type: " << getFormatName(headerInfo.raw_type) << std::endl;
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

        QmageDecoderInfo decoderInfo;
        QM_BOOL hasDecoderInfo2 = QmageDecCommon_GetDecoderInfo(buffer, fileSize, &decoderInfo);
        check_error("QmageDecCommon_GetDecoderInfo");

        if (hasDecoderInfo2) {
            QmageImageInfo imageInfo = decoderInfo.imageInfo;
            std::cout << "\nQmageDecoderHeader" << std::endl;
            std::cout << "DecoderInfo imageInfo bpp: " << imageInfo.bpp << "\n";
            std::cout << "DecoderInfo imageInfo PaddingValue: " << imageInfo.PaddingValue << "\n";
            std::cout << "DecoderInfo imageInfo raw_type: " << imageInfo.raw_type << "\n";
            std::cout << "DecoderInfo imageInfo img_type: " << imageInfo.img_type << "\n";
            std::cout << "DecoderInfo header_len: " << decoderInfo.header_len << "\n";
            std::cout << "DecoderInfo alpha_position: " << decoderInfo.alpha_position << "\n";
            std::cout << "DecoderInfo Animation: " << decoderInfo.Animation << "\n";
            std::cout << "DecoderInfo is_alpha_InOrgImage: " << decoderInfo.is_alpha_InOrgImage << "\n";
            std::cout << "DecoderInfo Use_chromakey: " << decoderInfo.Use_chromakey << "\n";
            std::cout << "DecoderInfo qp: " << decoderInfo.qp << "\n";
            std::cout << "DecoderInfo endian: " << decoderInfo.endian << "\n";
            std::cout << "DecoderInfo encoder_mode: " << decoderInfo.encoder_mode << "\n";
            std::cout << "DecoderInfo pAniDecInfo: " << decoderInfo.pAniDecInfo << "\n";
            std::cout << "DecoderInfo AndroidSupport: " << decoderInfo.AndroidSupport << "\n";
        }
    }

    // TODO What is the correct way to check for animation?
    if (headerInfo.mode != 0 || headerInfo.totalFrameNumber > 1) {
        if (verbosityLevel > REALLY_QUIET) {
            std::cout << "Image is animated" << std::endl;
        }

        // Set up the animation decoding context
        aniInfo = QmageDecCreateAniInfo((void*) buffer, QM_IO_BUFFER, fileSize);
        check_error("QmageDecCreateAniInfo");

        if (aniInfo == nullptr) {
            std::cerr << "Error: QmageDecCreateAniInfo failed" << std::endl;
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
        QmageRawImageType convertedType = getConvertedType(headerInfo.raw_type);
        bool needsConvert = (outputFormat != RAW) && (headerInfo.raw_type != convertedType);

        if (needsConvert && (verbosityLevel > REALLY_QUIET)) {
            std::cout << "Image will be converted from " << getFormatName(headerInfo.raw_type) << " to " << getFormatName(convertedType) << std::endl;
        }

        for (int i = 0; i < headerInfo.totalFrameNumber; i++) {
            //NedTheNerd: Decode TODO handle errors
            //int aniDecodeReturn = QmageDecodeAniFrame(aniInfo, frameBuffer);
            //std::cout << "QmageDecodeAniFrame return value: " << aniDecodeReturn << std::endl;
            QmageDecodeAniFrame(aniInfo, frameBuffer);
            check_error("QmageDecodeAniFrame");

            // Choose between raw decoded output or converting it if needed
            char* outBuffer;
            size_t outBufferSize;
            size_t channels;

            if (needsConvert) {
                channels = typeToBits(convertedType) / 8;
                outBufferSize = dimSize * channels;
                outBuffer = new char[outBufferSize];
                doConvert(headerInfo.raw_type, (const unsigned char*) frameBuffer, (unsigned char*) outBuffer, dimSize);
            } else {
                channels = stride;
                outBuffer = (char*) frameBuffer;
                outBufferSize = frameSize;
            }

            int fileNum = i + 1;
            std::string fileOutName = filename + "_frame-" + std::to_string(fileNum) + getExtensionForOutputFormat(outputFormat);

            if (!writeImageToFile(fileOutName, outputFormat, headerInfo.width, headerInfo.height, channels, outBuffer)) {
                returnVal = 1;
                std::cerr << "Error: Could not write frame " << fileNum << " to file " << fileOutName << std::endl;
            } else if (verbosityLevel > QUIET) {
                // Dexrn: had to escape the () otherwise it wouldn't be included in the output
                std::cout << "Wrote frame " << fileNum << " to \"" << fileOutName << "\"" << std::endl;
            }

            if (outBuffer != frameBuffer) {
                delete[] outBuffer;
            }
        }
    } else {
        std::cerr << "Error: TODO support for non-animated images" << std::endl;
        returnVal = 1;
        goto cleanup;
    }

    if (verbosityLevel > QUIET) {
        std::cout << "Successfully decoded " << headerInfo.totalFrameNumber << " frames from " << filename << std::endl;
    }

cleanup:

    if (aniInfo != nullptr) {
        QmageDecDestroyAniInfo(aniInfo);
    }

    if (buffer != nullptr) {
        delete[] buffer;
    }

    if (frameBuffer != nullptr) {
        delete[] frameBuffer;
    }

    return returnVal;
}
