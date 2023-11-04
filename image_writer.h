#ifndef QMAGEDECODER_IMAGE_WRITER_H
#define QMAGEDECODER_IMAGE_WRITER_H

#include <string>

enum ImageOutputFormat {
    NONE = -1,
    RAW,
    PNG,
    JPG,
    TGA,
    BMP,
    GIF
};

std::string getExtensionForOutputFormat(ImageOutputFormat format);

bool writeBytesToFile(const char* fileName, char* data, size_t size);

bool writeImageToFile(std::string fileOutName, ImageOutputFormat format, int width, int height, int channels, char* imageData);

#endif /* QMAGEDECODER_IMAGE_WRITER_H */
