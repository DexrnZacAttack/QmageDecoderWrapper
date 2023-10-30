#include <qmg.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "stb_image_write.h"

#define MSF_GIF_IMPL
#include "msf_gif.h"

// Comment this out to only use public functions from libQmageDecoder
#define USE_INTERNAL_FUNCTIONS

// Returns the name of a encoder codec
static std::string getEncoderCodecName(QmageDecodeCodecType codec) {
    switch (codec) {
        case QMAGE_DEC_V16_SHORT_INDEX:
            return "QMAGE_DEC_V16_SHORT_INDEX";
        case QMAGE_DEC_W2_PASS:
            return "QMAGE_DEC_W2_PASS";
        case QMAGE_DEC_V16_BYTE_INDEX:
            return "QMAGE_DEC_V16_BYTE_INDEX";
        case QMAGE_DEC_W1_PASS:
            return "QMAGE_DEC_W1_PASS";
        case QMAGE_DEC_FCODEC:
            return "QMAGE_DEC_FCODEC";
        case QMAGE_DEC_W1_PASS_FROM_W_ADAPTIVE:
            return "QMAGE_DEC_W1_PASS_FROM_W_ADAPTIVE";
        case QMAGE_DEC_V24_SHORT_INDEX:
            return "QMAGE_DEC_V24_SHORT_INDEX";
        case QMAGE_DEC_W2_PASS_ONLY:
            return "QMAGE_DEC_W2_PASS_ONLY";
        case QMAGE_DEC_PV:
            return "QMAGE_DEC_PV";
        case QMAGE_DEC_SLV:
            return "QMAGE_DEC_SLV";
        case QMAGE_DEC_QV:
            return "QMAGE_DEC_QV";
        default:
            return std::to_string(codec);
    }
}

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

// Converts an image in BGRA8888 format to RGBA8888 format
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

// Converts an image in RGBA8888 format to RGB888 format
static void convertRGBA8888ToRGB888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
    for (size_t i = 0; i < amountPixels; i++) {
        *(output++) = *(input++);
        *(output++) = *(input++);
        *(output++) = *(input++);
        input++;
    }
}

// Converts an image in RGB888 format to RGBA8888 format
static void convertRGB888ToRGBA8888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
    for (size_t i = 0; i < amountPixels; i++) {
        *(output++) = *(input++);
        *(output++) = *(input++);
        *(output++) = *(input++);
        *(output++) = 0xff;
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

static std::string getDecodeErrorName(QmageDecoderError err) {
    switch (err) {
        case QM_DEC_ANI_END_OF_FRAME:
            return "QM_DEC_ANI_END_OF_FRAME";
        case QM_DEC_ANI_NOT_ENOUGH_DATA:
            return "QM_DEC_ANI_NOT_ENOUGH_DATA";
        case QM_DEC_ANI_FAIL:
            return "QM_DEC_ANI_FAIL";
        case QM_DEC_INVALID_SIZE:
            return "QM_DEC_INVALID_SIZE";
        case QM_DEC_INVALID_ADDRESS:
            return "QM_DEC_INVALID_ADDRESS";
        case QM_DEC_OUT_OF_MEMORY:
            return "QM_DEC_OUT_OF_MEMORY";
        case QM_DEC_UNSUPPORTED_VERSION:
            return "QM_DEC_UNSUPPORTED_VERSION";
        case QM_DEC_UNSUPPORTED_TYPE:
            return "QM_DEC_UNSUPPORTED_TYPE";
        case QM_DEC_UNSUPPORTED_FILE:
            return "QM_DEC_UNSUPPORTED_FILE";
        case QM_DEC_CORRUPTED_FILE:
            return "QM_DEC_CORRUPTED_FILE";
        case QM_DEC_FAIL:
            return "QM_DEC_FAIL";
        default:
            return std::to_string(err);
    }
}

void check_error(const char* message) {
    QmageDecoderError decoderError = QmageDecGetLastErr();
    if (decoderError != 0) {
        std::cerr << "[" << message << "] " << getDecodeErrorName(decoderError) << std::endl;
    }
}

enum ImageOutputFormat {
    RAW,
    PNG,
    JPG,
    TGA,
    BMP,
    GIF
};

static std::string getExtensionForOutputFormat(ImageOutputFormat format) {
    switch (format) {
        case PNG:
            return ".png";
        case JPG:
            return ".jpg";
        case TGA:
            return ".tga";
        case BMP:
            return ".bmp";
        case GIF:
            return ".gif";
        default:
            return ".raw";
    }
}

static bool writeBytesToFile(const char* fileName, char* data, size_t size) {
    // TODO handle errors
    std::ofstream fos;
    fos.open(fileName, std::ios::binary | std::ios::out);
    fos.write(data, size);
    fos.close();
    return true;
}

static bool writeImageToFile(std::string fileOutName, ImageOutputFormat format, int width, int height, int channels, char* imageData) {
    switch (format) {
        case GIF: {
            MsfGifState gifState = {};

            if (!msf_gif_begin(&gifState, width, height)) {
                return false;
            }

            uint8_t* gifBuffer;

            if (channels != 4) {
                gifBuffer = new uint8_t[width * height * 4];
                convertRGB888ToRGBA8888((const unsigned char*) imageData, (unsigned char*) gifBuffer, width * height);
            } else {
                gifBuffer = (uint8_t*) imageData;
            }

            bool returnVal = msf_gif_frame(&gifState, gifBuffer, 0, 16, width * 4);

            if ((char*) gifBuffer != imageData) {
                delete[] gifBuffer;
            }

            if (returnVal) {
                MsfGifResult result = msf_gif_end(&gifState);
                returnVal = result.data && writeBytesToFile(fileOutName.c_str(), (char*) result.data, result.dataSize);
                msf_gif_free(result);
            }

            return returnVal;
        }
        case PNG:
            return stbi_write_png(fileOutName.c_str(), width, height, channels, imageData, width * channels);
        case JPG:
            return stbi_write_jpg(fileOutName.c_str(), width, height, channels, imageData, 90);
        case TGA:
            return stbi_write_tga(fileOutName.c_str(), width, height, channels, imageData);
        case BMP:
            return stbi_write_bmp(fileOutName.c_str(), width, height, channels, imageData);
        default:
            return writeBytesToFile(fileOutName.c_str(), imageData, width * height * channels);
    }
}

enum Verbosity {
    REALLY_QUIET,
    QUIET,
    NORMAL
};

// Either add a frame to an animated gif, or write it to a file
static bool addFrame(QmageDecoderHeader headerInfo, size_t dimSize, size_t frameSize, int stride, bool needsConvert, QmageRawImageType convertedType, ImageOutputFormat outputFormat, MsfGifState* gifState, int delay, const char* frameBuffer, std::string fileOutName) {
    // Choose between raw decoded output or converting it if needed
    char* outBuffer;
    size_t outBufferSize;
    size_t channels;
    bool returnVal;

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

    if (outputFormat == GIF) {
        uint8_t* gifBuffer;

        if (channels != 4) {
            gifBuffer = new uint8_t[dimSize * 4];
            convertRGB888ToRGBA8888((const unsigned char*) outBuffer, (unsigned char*) gifBuffer, dimSize);
        } else {
            gifBuffer = (uint8_t*) outBuffer;
        }

        returnVal = msf_gif_frame(gifState, gifBuffer, delay, 16, headerInfo.width * 4);

        if ((char*) gifBuffer != outBuffer) {
            delete[] gifBuffer;
        }
    } else {
        returnVal = writeImageToFile(fileOutName, outputFormat, headerInfo.width, headerInfo.height, channels, outBuffer);
    }

    if (outBuffer != frameBuffer) {
        delete[] outBuffer;
    }

    return returnVal;
}

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
            std::cout << "  -f, --format: Outputs the frames in the format specified.\n  Supported formats are: raw, png (default), jpg, tga, bmp, and animated gif." << std::endl;
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
                } else if (format == "gif") {
                    outputFormat = GIF;
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
#ifdef USE_INTERNAL_FUNCTIONS
    int version = QmageDecCommon_GetVersion();
    int opaqueInfo = QmageDecCommon_GetOpaqueInfo(filename.c_str());

    if (verbosityLevel > REALLY_QUIET) {
        // Dexrn: Changed these so that it is more clear that we are talking about the library and not QmageDecoder.
        std::cout << "libQmageDecoder version:" << version << std::endl;
        std::cout << "libQmageDecoder opaqueInfo:" << opaqueInfo << std::endl;
    }
#endif

    QmageDecoderHeader headerInfo;
    QmageDecAniInfo* aniInfo = nullptr;
    const char* frameBuffer = nullptr;
    size_t dimSize = 0;
    size_t frameSize = 0;
    int stride = 0;
    MsfGifState gifState = {};

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

#ifdef USE_INTERNAL_FUNCTIONS
        QmageDecoderInfo decoderInfo;
        QM_BOOL hasDecoderInfo2 = QmageDecCommon_GetDecoderInfo(buffer, fileSize, &decoderInfo);
        check_error("QmageDecCommon_GetDecoderInfo");

        if (hasDecoderInfo2) {
            QmageImageInfo imageInfo = decoderInfo.imageInfo;
            std::cout << "\nQmageDecoderHeader" << std::endl;
            std::cout << "DecoderInfo imageInfo bpp: " << imageInfo.bpp << "\n";
            std::cout << "DecoderInfo imageInfo PaddingValue: " << imageInfo.PaddingValue << "\n";
            std::cout << "DecoderInfo imageInfo raw_type: " << getFormatName(imageInfo.raw_type) << "\n";
            std::cout << "DecoderInfo imageInfo img_type: " << imageInfo.img_type << "\n";
            std::cout << "DecoderInfo header_len: " << decoderInfo.header_len << "\n";
            std::cout << "DecoderInfo alpha_position: " << decoderInfo.alpha_position << "\n";
            std::cout << "DecoderInfo Animation: " << decoderInfo.Animation << "\n";
            std::cout << "DecoderInfo is_alpha_InOrgImage: " << decoderInfo.is_alpha_InOrgImage << "\n";
            std::cout << "DecoderInfo Use_chromakey: " << decoderInfo.Use_chromakey << "\n";
            std::cout << "DecoderInfo qp: " << decoderInfo.qp << "\n";
            std::cout << "DecoderInfo endian: " << decoderInfo.endian << "\n";
            std::cout << "DecoderInfo encoder_mode: " << getEncoderCodecName(decoderInfo.encoder_mode) << "\n";
            std::cout << "DecoderInfo pAniDecInfo: " << decoderInfo.pAniDecInfo << "\n";
            std::cout << "DecoderInfo AndroidSupport: " << decoderInfo.AndroidSupport << "\n";
        }
#endif
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

    if (outputFormat == GIF) {
        if (!msf_gif_begin(&gifState, headerInfo.width, headerInfo.height)) {
            std::cerr << "Error: Could not create gif encoding context" << std::endl;
            returnVal = 1;
            goto cleanup;
        }
    }

    {
        // NedTheNerd: TODO: Allow configuring this
        // Dexrn: Did it.
        QmageRawImageType convertedType = getConvertedType(headerInfo.raw_type);
        bool needsConvert = (outputFormat != RAW) && (headerInfo.raw_type != convertedType);

        if (needsConvert && (verbosityLevel > REALLY_QUIET)) {
            std::cout << "Image will be converted from " << getFormatName(headerInfo.raw_type) << " to " << getFormatName(convertedType) << std::endl;
        }

        if (aniInfo != nullptr) {
            for (int i = 0; i < headerInfo.totalFrameNumber; i++) {
                //NedTheNerd: Decode
                int aniDecodeReturn = QmageDecodeAniFrame(aniInfo, frameBuffer);
                check_error("QmageDecodeAniFrame");

                if (aniDecodeReturn < 0) {
                    std::cerr << "QmageDecodeAniFrame returned error " << getDecodeErrorName((QmageDecoderError) aniDecodeReturn) << std::endl;
                    returnVal = 1;
                } else if (aniDecodeReturn == 0 && verbosityLevel > QUIET) {
                    std::cout << "Last frame" << std::endl;
                }

                int fileNum = i + 1;
                std::string fileOutName = filename + "_frame-" + std::to_string(fileNum) + getExtensionForOutputFormat(outputFormat);

                if (!addFrame(headerInfo, dimSize, frameSize, stride, needsConvert, convertedType, outputFormat, &gifState, 0, frameBuffer, fileOutName)) {
                    if (outputFormat == GIF) {
                        std::cerr << "Error: Could not write frame " << fileNum << std::endl;
                    } else {
                        std::cerr << "Error: Could not write frame " << fileNum << " to file " << fileOutName << std::endl;
                    }
                    returnVal = 1;
                } else if (verbosityLevel > QUIET) {
                    if (outputFormat == GIF) {
                        std::cout << "Wrote frame " << fileNum << std::endl;
                    } else {
                        std::cout << "Wrote frame " << fileNum << " to \"" << fileOutName << "\"" << std::endl;
                    }
                }

                if (aniDecodeReturn == 0) {
                    break;
                }
            }
        } else {
            //NedTheNerd: Decode
            int decodeReturn = QmageDecodeFrame(buffer, fileSize, frameBuffer);
            check_error("QmageDecodeFrame");

            if (decodeReturn < 0) {
                std::cerr << "QmageDecodeFrame returned error " << getDecodeErrorName((QmageDecoderError) decodeReturn) << std::endl;
                returnVal = 1;
                goto cleanup;
            }

            int fileNum = 1;
            std::string fileOutName = filename + "_frame-" + std::to_string(fileNum) + getExtensionForOutputFormat(outputFormat);

            if (!addFrame(headerInfo, dimSize, frameSize, stride, needsConvert, convertedType, outputFormat, &gifState, 0, frameBuffer, fileOutName)) {
                if (outputFormat == GIF) {
                    std::cerr << "Error: Could not write frame " << fileNum << std::endl;
                } else {
                    std::cerr << "Error: Could not write frame " << fileNum << " to file " << fileOutName << std::endl;
                }
                returnVal = 1;
            } else if (verbosityLevel > QUIET) {
                if (outputFormat == GIF) {
                    std::cout << "Wrote frame " << fileNum << std::endl;
                } else {
                    std::cout << "Wrote frame " << fileNum << " to \"" << fileOutName << "\"" << std::endl;
                }
            }
        }
    }

    if (verbosityLevel > QUIET) {
        std::cout << "Successfully decoded " << headerInfo.totalFrameNumber << " frames from " << filename << std::endl;
    }

cleanup:

    if (outputFormat == GIF) {
        std::string outFileName = filename + ".gif";
        MsfGifResult result = msf_gif_end(&gifState);

        if (result.data && writeBytesToFile(outFileName.c_str(), (char*) result.data, result.dataSize)) {
            std::cout << "Successfully wrote gif to " << outFileName << std::endl;
        } else {
            std::cerr << "Error: Could not write gif to " << filename << ".gif" << std::endl;
            returnVal = 1;
        }

        msf_gif_free(result);
    }

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
