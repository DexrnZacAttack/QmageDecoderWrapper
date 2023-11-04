#ifndef QMAGEDECODER_IMAGE_WRITER_H
#define QMAGEDECODER_IMAGE_WRITER_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    NONE = -1,
    RAW,
    PNG,
    JPG,
    TGA,
    BMP,
    GIF
} ImageOutputFormat;

const char* getExtensionForOutputFormat(ImageOutputFormat format);

bool writeBytesToFile(const char* fileName, char* data, size_t size, bool overwrite);

bool writeImageToFile(const char* fileOutName, ImageOutputFormat format, int width, int height, int channels, char* imageData, bool overwrite);
#ifdef __cplusplus
}
#endif

#endif /* QMAGEDECODER_IMAGE_WRITER_H */
