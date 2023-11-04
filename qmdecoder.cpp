#include <qmg.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

#include "image_helper.h"
#include "image_writer.h"
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
            return "Unknown encoder codec " + std::to_string(codec);
    }
}

// Returns the name of a decoder version
static std::string getDecoderVersionName(Qmage_DecoderVersion version) {
    switch (version) {
        case QM_DEC_QMAGE_VER_1_43_LESS:
            return "QM_DEC_QMAGE_VER_1_43_LESS";
        case QM_DEC_QMAGE_VER_1_43_MORE:
            return "QM_DEC_QMAGE_VER_1_43_MORE";
        case QM_DEC_QMAGE_VER_1_70:
            return "QM_DEC_QMAGE_VER_1_70";
        case 14:
        case 15:
            return "Known unnamed q version " + std::to_string(version);
        default:
            return "Unknown q version " + std::to_string(version);
    }
}

// Returns the name of a V decoder version
static std::string getVDecoderVersionName(Qmage_V_DecoderVersion version) {
    switch (version) {
        case QM_DEC_VCODEC_VER_2_00:
            return "QM_DEC_VCODEC_VER_2_00";
        case QM_DEC_VCODEC_VER_2_00_T:
            return "QM_DEC_VCODEC_VER_2_00_T";
        case QM_DEC_VCODEC_VER_3_00_TAG_LESS_5:
            return "QM_DEC_VCODEC_VER_3_00_TAG_LESS_5";
        case QM_DEC_VCODEC_VER_3_00_TAG_MORE_5:
            return "QM_DEC_VCODEC_VER_3_00_TAG_MORE_5";
        case QM_DEC_VCODEC_VER_3_00_11_EXCLUSIVE_OR:
            return "QM_DEC_VCODEC_VER_3_00_11_EXCLUSIVE_OR";
        case QM_DEC_VCODEC_VER_3_01_TAG_LESS_5:
            return "QM_DEC_VCODEC_VER_3_01_TAG_LESS_5";
        case QM_DEC_VCODEC_VER_3_01_TAG_MORE_5:
            return "QM_DEC_VCODEC_VER_3_01_TAG_MORE_5";
        case QM_DEC_VCODEC_VER_3_01_QMAGE_1_01:
            return "QM_DEC_VCODEC_VER_3_01_QMAGE_1_01";
        default:
            return "Unknown v version " + std::to_string(version);
    }
}

// Returns the name of a F decoder version
static std::string getFDecoderVersionName(Qmage_F_DecoderVersion version) {
    switch (version) {
        case QM_DEC_FCODEC_VER_1_00:
            return "QM_DEC_FCODEC_VER_1_00";
        case QM_DEC_FCODEC_VER_1_00_QMAGE_1_01:
            return "QM_DEC_FCODEC_VER_1_00_QMAGE_1_01";
        default:
            return "Unknown f version " + std::to_string(version);
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
        // TODO handle unknown formats better
        default:
            return 32;
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
            return "Unknown pixel format " + std::to_string(type);
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
            return "Unknown error code " + std::to_string(err);
    }
}

void check_error(const char* message) {
    QmageDecoderError decoderError = QmageDecGetLastErr();
    if (decoderError != 0) {
        std::cerr << "[" << message << "] " << getDecodeErrorName(decoderError) << std::endl;
    }
}

enum Verbosity {
    REALLY_QUIET,
    QUIET,
    NORMAL,
    VERBOSE
};

// Either add a frame to an animated gif, or write it to a file
static bool addFrame(QmageDecoderHeader headerInfo, size_t dimSize, size_t frameSize, int stride, bool needsConvert, QmageRawImageType convertedType, ImageOutputFormat outputFormat, MsfGifState* gifState, int delay, const char* frameBuffer, std::string fileOutName) {
    if (outputFormat == NONE) {
        return true;
    }

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
        returnVal = writeImageToFile(fileOutName.c_str(), outputFormat, headerInfo.width, headerInfo.height, channels, outBuffer, true);
    }

    if (outBuffer != frameBuffer) {
        delete[] outBuffer;
    }

    return returnVal;
}

#ifdef USE_INTERNAL_FUNCTIONS
static void printDecInfo(QmageDecoderInfo decoderInfo) {
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

    Qmage_DecderLowInfo lowInfo = decoderInfo.DecLowInfo;

    std::cout << "DecoderInfo DecLowInfo qversion: " << getDecoderVersionName(lowInfo.qversion) << "\n";
    std::cout << "DecoderInfo DecLowInfo vversion: " << getVDecoderVersionName(lowInfo.vversion) << "\n";
    std::cout << "DecoderInfo DecLowInfo fversion: " << getFDecoderVersionName(lowInfo.fversion) << "\n";
#ifdef PRINT_MORE
    std::cout << "DecoderInfo DecLowInfo Ver200_SPEED: " << lowInfo.Ver200_SPEED << "\n";
    std::cout << "DecoderInfo DecLowInfo IS_ANIMATION: " << lowInfo.IS_ANIMATION << "\n";
    std::cout << "DecoderInfo DecLowInfo UseExtraException: " << lowInfo.UseExtraException << "\n";
    std::cout << "DecoderInfo DecLowInfo tiny: " << lowInfo.tiny << "\n";
    std::cout << "DecoderInfo DecLowInfo IsDyanmicTable: " << lowInfo.IsDyanmicTable << "\n";
    std::cout << "DecoderInfo DecLowInfo IsOpaque: " << lowInfo.IsOpaque << "\n";
    std::cout << "DecoderInfo DecLowInfo NearLossless: " << lowInfo.NearLossless << "\n";
    std::cout << "DecoderInfo DecLowInfo SIZE_SHIFT: " << lowInfo.SIZE_SHIFT << "\n";
    std::cout << "DecoderInfo DecLowInfo ANI_RANGE: " << lowInfo.ANI_RANGE << "\n";
    std::cout << "DecoderInfo DecLowInfo qp: " << lowInfo.qp << "\n";
    std::cout << "DecoderInfo DecLowInfo mode_bit: " << lowInfo.mode_bit << "\n";
    std::cout << "DecoderInfo DecLowInfo header_len: " << lowInfo.header_len << "\n";
    std::cout << "DecoderInfo DecLowInfo NotComp: " << lowInfo.NotComp << "\n";
    std::cout << "DecoderInfo DecLowInfo NotAlphaComp: " << lowInfo.NotAlphaComp << "\n";
    std::cout << "DecoderInfo DecLowInfo alpha_decode_flag: " << lowInfo.alpha_decode_flag << "\n";
    std::cout << "DecoderInfo DecLowInfo depth: " << lowInfo.depth << "\n";
    std::cout << "DecoderInfo DecLowInfo alpha_depth: " << lowInfo.alpha_depth << "\n";
    std::cout << "DecoderInfo DecLowInfo rgb_encoder_mode: " << getEncoderCodecName(lowInfo.rgb_encoder_mode) << "\n";
    std::cout << "DecoderInfo DecLowInfo alpha_encoder_mode: " << getEncoderCodecName(lowInfo.alpha_encoder_mode) << "\n";
    std::cout << "DecoderInfo DecLowInfo out_type: " << getFormatName(lowInfo.out_type) << "\n";

    Qmage_VDecoderVMODE_T mode = lowInfo.mode;

    std::cout << "DecoderInfo DecLowInfo mode color: " << std::to_string(mode.color) << "\n";
    std::cout << "DecoderInfo DecLowInfo mode mode: " << std::to_string(mode.mode) << "\n";
    std::cout << "DecoderInfo DecLowInfo mode animation: " << std::to_string(mode.animation) << "\n";
    std::cout << "DecoderInfo DecLowInfo mode qp: " << std::to_string(mode.qp) << "\n";
#endif
}
#endif

// Print info about the Qmage header at the start of the buffer
static bool printHeaderInfo(const char* buffer, std::streampos size, QmageDecAniInfo* aniInfo) {
    QmageDecoderHeader headerInfo;
    // Read the header
    QM_BOOL hasHeaderInfo = QmageDecParseHeader(buffer, QM_IO_BUFFER, size, &headerInfo);
    check_error("QmageDecParseHeader");

    if (!hasHeaderInfo) {
        std::cerr << "Error: QmageDecParseHeader failed" << std::endl;
        return false;
    }

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

    if (aniInfo == nullptr) {
        QM_BOOL hasDecoderInfo2 = QmageDecCommon_GetDecoderInfo(buffer, size, &decoderInfo);
        check_error("QmageDecCommon_GetDecoderInfo");

        if (!hasDecoderInfo2) {
            std::cerr << "Error: QmageDecCommon_GetDecoderInfo failed" << std::endl;
            return false;
        }
    } else {
        QM_BOOL hasDecoderInfo2 = QmageDecCommon_GetAniDecoderInfo(aniInfo, &decoderInfo);
        check_error("QmageDecCommon_GetAniDecoderInfo");

        if (!hasDecoderInfo2) {
            std::cerr << "Error: QmageDecCommon_GetAniDecoderInfo failed" << std::endl;
            return false;
        }
    }

    printDecInfo(decoderInfo);
#endif

    return true;
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
            std::cout << "  -v, --verbose: Print additional information about each frame during decoding." << std::endl;
            std::cout << "GitHub Repo: https://github.com/DexrnZacAttack/QmageDecoder" << std::endl;
            return 0;
            // Dexrn: yes I know this looks like hell...
        } else if (arg == "--format" || arg == "-f") {
            if (i + 1 < argc) {
                std::string format = argv[i + 1];
                if (format == "none") {
                    outputFormat = NONE;
                } else if (format == "raw") {
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
        } else if (arg == "--verbose" || arg == "-v") {
            verbosityLevel = VERBOSE;
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

    if (verbosityLevel > REALLY_QUIET) {
        printHeaderInfo(buffer, fileSize, aniInfo);
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
        bool needsConvert = (outputFormat > RAW) && (headerInfo.raw_type != convertedType);

        if (needsConvert && (verbosityLevel > REALLY_QUIET)) {
            std::cout << "Image will be converted from " << getFormatName(headerInfo.raw_type) << " to " << getFormatName(convertedType) << std::endl;
        }

        if (aniInfo != nullptr) {
            std::streampos filePos = 0;

            for (int i = 0; i < headerInfo.totalFrameNumber; i++) {
                if (verbosityLevel >= VERBOSE) {
                    printHeaderInfo(buffer + filePos, fileSize - filePos, aniInfo);
                }

                //NedTheNerd: Decode
                int aniDecodeReturn = QmageDecodeAniFrame(aniInfo, frameBuffer);
                check_error("QmageDecodeAniFrame");

                if (aniDecodeReturn < 0) {
                    std::cerr << "QmageDecodeAniFrame returned error " << getDecodeErrorName((QmageDecoderError) aniDecodeReturn) << std::endl;
                    returnVal = 1;
                } else if (aniDecodeReturn == 0 && verbosityLevel > QUIET) {
                    std::cout << "Last frame" << std::endl;
                } else if (aniDecodeReturn > 0) {
                    filePos += aniDecodeReturn;
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
                } else if (verbosityLevel > QUIET && outputFormat != NONE) {
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
            } else if (verbosityLevel > QUIET && outputFormat != NONE) {
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

        if (result.data && writeBytesToFile(outFileName.c_str(), (char*) result.data, result.dataSize, true)) {
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
