#include "image_writer.h"

#include <fstream>

#include "image_helper.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "msf_gif.h"
#include "stb_image_write.h"

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
        case GIF:
            return ".gif";
        default:
            return ".raw";
    }
}

bool writeBytesToFile(const char* fileName, char* data, size_t size) {
    // TODO handle errors
    std::ofstream fos;
    fos.open(fileName, std::ios::binary | std::ios::out);
    fos.write(data, size);
    fos.close();
    return true;
}

bool writeImageToFile(std::string fileOutName, ImageOutputFormat format, int width, int height, int channels, char* imageData) {
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
