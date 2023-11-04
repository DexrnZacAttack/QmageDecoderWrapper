#ifndef QMAGEDECODER_IO_HELPER_H
#define QMAGEDECODER_IO_HELPER_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
bool doesFileExist(const char* fileName);

bool writeBytesToFile(const char* fileName, char* data, size_t size, bool overwrite);
#ifdef __cplusplus
}
#endif

#endif /* QMAGEDECODER_IO_HELPER_H */
