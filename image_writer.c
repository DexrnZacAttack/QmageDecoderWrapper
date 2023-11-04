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

typedef struct {
    bool error;
    size_t buff_size;
    char* buff;
} mem_image;

static void write_to_mem(void* context, void* data, int size) {
    mem_image* image = (mem_image*) context;

    if (image->error) {
        return;
    }

    image->buff = realloc(image->buff, image->buff_size + size);

    if (image->buff == NULL) {
        image->error = true;
        return;
    }

    memcpy(image->buff + image->buff_size, data, size);
    image->buff_size += size;
}

char* writeImageToBytes(size_t* outsize, ImageOutputFormat format, int width, int height, int channels, char* imageData) {
    switch (format) {
        case GIF: {
            char* gifOutput = NULL;
            MsfGifState gifState = {};

            if (!msf_gif_begin(&gifState, width, height)) {
                return NULL;
            }

            uint8_t* gifBuffer;

            if (channels != 4) {
                gifBuffer = (uint8_t*) malloc(width * height * 4);

                if (gifBuffer == NULL) {
                    return NULL;
                }

                convertRGB888ToRGBA8888((const unsigned char*) imageData, (unsigned char*) gifBuffer, width * height);
            } else {
                gifBuffer = (uint8_t*) imageData;
            }

            bool didEncode = msf_gif_frame(&gifState, gifBuffer, 0, 16, width * 4);

            if ((char*) gifBuffer != imageData) {
                free(gifBuffer);
            }

            if (didEncode) {
                MsfGifResult result = msf_gif_end(&gifState);

                if (result.data) {
                    gifOutput = (char*) malloc(result.dataSize);

                    if (gifOutput != NULL) {
                        *outsize = result.dataSize;
                        memcpy(gifOutput, result.data, result.dataSize);
                    }
                }

                msf_gif_free(result);
            }

            return gifOutput;
        }

        case PNG: {
            mem_image image = {};

            if (stbi_write_png_to_func(write_to_mem, &image, width, height, channels, imageData, width * channels) && !image.error) {
                *outsize = image.buff_size;
                return image.buff;
            }

            if (image.buff != NULL) {
                free(image.buff);
            }

            return NULL;
        }

        case JPG: {
            mem_image image = {};

            if (stbi_write_jpg_to_func(write_to_mem, &image, width, height, channels, imageData, 90) && !image.error) {
                *outsize = image.buff_size;
                return image.buff;
            }

            if (image.buff != NULL) {
                free(image.buff);
            }

            return NULL;
        }

        case TGA: {
            mem_image image = {};

            if (stbi_write_tga_to_func(write_to_mem, &image, width, height, channels, imageData) && !image.error) {
                *outsize = image.buff_size;
                return image.buff;
            }

            if (image.buff != NULL) {
                free(image.buff);
            }

            return NULL;
        }

        case BMP: {
            mem_image image = {};

            if (stbi_write_bmp_to_func(write_to_mem, &image, width, height, channels, imageData) && !image.error) {
                *outsize = image.buff_size;
                return image.buff;
            }

            if (image.buff != NULL) {
                free(image.buff);
            }

            return NULL;
        }

        default:
            return NULL;
    }
}
