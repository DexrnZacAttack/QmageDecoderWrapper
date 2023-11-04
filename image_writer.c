#include "image_writer.h"

#include "image_helper.h"
#include "io_helper.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "msf_gif.h"
#include "stb_image_write.h"

const char* getExtensionForOutputFormat(ImageOutputFormat format) {
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

bool writeImageToFile(const char* fileOutName, ImageOutputFormat format, int width, int height, int channels, char* imageData, bool overwrite) {
    if (!overwrite && doesFileExist(fileOutName)) {
        return false;
    }

    switch (format) {
        case GIF: {
            MsfGifState gifState = {};

            if (!msf_gif_begin(&gifState, width, height)) {
                return false;
            }

            uint8_t* gifBuffer;

            if (channels != 4) {
                gifBuffer = (uint8_t*) malloc(width * height * 4);

                if (gifBuffer == NULL) {
                    return false;
                }

                convertRGB888ToRGBA8888((const unsigned char*) imageData, (unsigned char*) gifBuffer, width * height);
            } else {
                gifBuffer = (uint8_t*) imageData;
            }

            bool returnVal = msf_gif_frame(&gifState, gifBuffer, 0, 16, width * 4);

            if ((char*) gifBuffer != imageData) {
                free(gifBuffer);
            }

            if (returnVal) {
                MsfGifResult result = msf_gif_end(&gifState);
                returnVal = result.data && writeBytesToFile(fileOutName, (char*) result.data, result.dataSize, true);
                msf_gif_free(result);
            }

            return returnVal;
        }
        case PNG:
            return stbi_write_png(fileOutName, width, height, channels, imageData, width * channels);
        case JPG:
            return stbi_write_jpg(fileOutName, width, height, channels, imageData, 90);
        case TGA:
            return stbi_write_tga(fileOutName, width, height, channels, imageData);
        case BMP:
            return stbi_write_bmp(fileOutName, width, height, channels, imageData);
        default:
            return writeBytesToFile(fileOutName, imageData, width * height * channels, true);
    }
}
