#include "qmage_helper.h"

#include "image_helper.h"

// Returns the name of a encoder codec
std::string getEncoderCodecName(QmageDecodeCodecType codec) {
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
std::string getDecoderVersionName(Qmage_DecoderVersion version) {
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
std::string getVDecoderVersionName(Qmage_V_DecoderVersion version) {
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
std::string getFDecoderVersionName(Qmage_F_DecoderVersion version) {
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
int typeToBits(QmageRawImageType type) {
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

std::string getFormatName(QmageRawImageType type) {
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

QmageRawImageType getConvertedType(QmageRawImageType type) {
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

void doConvert(QmageRawImageType from, const unsigned char* input, unsigned char* output, size_t amountPixels) {
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

std::string getDecodeErrorName(QmageDecoderError err) {
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
