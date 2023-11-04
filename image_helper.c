#include "image_helper.h"

// Converts an image in RGB565 format to RGB888 format
void convertRGB565ToRGB888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
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
void convertRGBA5658ToRGBA8888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
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
void convertARGB8565ToRGBA8888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
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
void convertBGR888ToRGB888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
    for (size_t i = 0; i < amountPixels; i++) {
        unsigned char b = *(input++);
        unsigned char g = *(input++);
        *(output++) = *(input++);
        *(output++) = g;
        *(output++) = b;
    }
}

// Converts an image in ARGB8888 format to RGBA8888 format TODO test
void convertARGB8888ToRGBA8888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
    for (size_t i = 0; i < amountPixels; i++) {
        unsigned char a = *(input++);
        *(output++) = *(input++);
        *(output++) = *(input++);
        *(output++) = *(input++);
        *(output++) = a;
    }
}

// Converts an image in BGRA8888 format to RGBA8888 format
void convertBGRA8888ToRGB888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
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
void convertRGBA8888ToRGB888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
    for (size_t i = 0; i < amountPixels; i++) {
        *(output++) = *(input++);
        *(output++) = *(input++);
        *(output++) = *(input++);
        input++;
    }
}

// Converts an image in RGB888 format to RGBA8888 format
void convertRGB888ToRGBA8888(const unsigned char* input, unsigned char* output, size_t amountPixels) {
    for (size_t i = 0; i < amountPixels; i++) {
        *(output++) = *(input++);
        *(output++) = *(input++);
        *(output++) = *(input++);
        *(output++) = 0xff;
    }
}
