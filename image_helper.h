#ifndef QMAGEDECODER_IMAGE_HELPER_H
#define QMAGEDECODER_IMAGE_HELPER_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
// Converts an image in RGB565 format to RGB888 format
void convertRGB565ToRGB888(const unsigned char* input, unsigned char* output, size_t amountPixels);
// Converts an image in RGBA5658 format to RGBA8888 format
void convertRGBA5658ToRGBA8888(const unsigned char* input, unsigned char* output, size_t amountPixels);
// Converts an image in ARGB8565 format to RGBA8888 format
void convertARGB8565ToRGBA8888(const unsigned char* input, unsigned char* output, size_t amountPixels);
// Converts an image in BGR888 format to RGB888 format
void convertBGR888ToRGB888(const unsigned char* input, unsigned char* output, size_t amountPixels);
// Converts an image in ARGB8888 format to RGBA8888 format
void convertARGB8888ToRGBA8888(const unsigned char* input, unsigned char* output, size_t amountPixels);
// Converts an image in BGRA8888 format to RGBA8888 format
void convertBGRA8888ToRGB888(const unsigned char* input, unsigned char* output, size_t amountPixels);
// Converts an image in RGBA8888 format to RGB888 format
void convertRGBA8888ToRGB888(const unsigned char* input, unsigned char* output, size_t amountPixels);
// Converts an image in RGB888 format to RGBA8888 format
void convertRGB888ToRGBA8888(const unsigned char* input, unsigned char* output, size_t amountPixels);
#ifdef __cplusplus
}
#endif

#endif /* QMAGEDECODER_IMAGE_HELPER_H */
