#ifndef QMAGEDECODER_QMAGE_HELPER_H
#define QMAGEDECODER_QMAGE_HELPER_H

#include <qmg.h>

#include <string>

// Returns the name of a encoder codec
std::string getEncoderCodecName(QmageDecodeCodecType codec);
// Returns the name of a decoder version
std::string getDecoderVersionName(Qmage_DecoderVersion version);
// Returns the name of a V decoder version
std::string getVDecoderVersionName(Qmage_V_DecoderVersion version);
// Returns the name of a F decoder version
std::string getFDecoderVersionName(Qmage_F_DecoderVersion version);
// Returns the amount of bits needed to store an image of this type
int typeToBits(QmageRawImageType type);

std::string getFormatName(QmageRawImageType type);

QmageRawImageType getConvertedType(QmageRawImageType type);

void doConvert(QmageRawImageType from, const unsigned char* input, unsigned char* output, size_t amountPixels);

std::string getDecodeErrorName(QmageDecoderError err);

#endif /* QMAGEDECODER_QMAGE_HELPER_H */
