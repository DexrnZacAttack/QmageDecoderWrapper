#ifndef _QURAM_H
#define _QURAM_H
#include <qmg.hpp>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

size_t QuramQmage_Fwrite_0(const void *ptr, size_t size, size_t n, FILE* s) {
    return fwrite(ptr, size, n, s);
}

QMUINT32 QuramQmage_Fwrite(const void *buffer, QMUINT32 size, QMUINT32 count, QMINT32 *stream) {
    return QuramQmage_Fwrite_0(buffer, size, count, (FILE *)stream);
}

QMINT32 QuramQmage_Fclose(QMINT32 *stream) {
    return fclose((FILE *) stream);
}

/*QMINT32 * QuramQmage_Fopen(QMCHAR *filename, QMCHAR *mode) {
    FILE *file = fopen(filename, mode);
    return &file->_flags;
}*/

QMUINT32 QuramQmage_Fread(void *buffer, QMUINT32 size, QMUINT32 count, QMINT32 *stream) {
    return fread(buffer, size, count, (FILE *) stream);
}

void QuramQmage_Free(void *pMem) {
    free(pMem);
}

QMINT32 QuramQmage_Fseek(QMINT32 *stream, QMLONG offset, QMINT32 origin) {
    return fseek((FILE *) stream, offset, origin);
}

QMULONG QuramQmage_Ftell(QMINT32 *stream) {
    return ftell((FILE *) stream);
}

QMUINT32 QuramQmage_Fwrite(void *buffer, QMUINT32 size, QMUINT32 count, QMINT32 *stream) {
    return fwrite(buffer, size, count, (FILE *) stream);
}

void * QuramQmage_Malloc(QMINT32 size) {
    return malloc(size);
}

void * QuramQmage_Memcpy(void *dest, void *src, QMUINT32 count) {
    return memcpy(dest, src, count);
}

void * QuramQmage_Memmove(void *dest, void *source, QMUINT32 num) {
    return memmove(dest, source, num);
}

void * QuramQmage_Memset(void *dest, QMINT32 c, QMUINT32 count) {
    return memset(dest, c, count);
}

void * QuramQmage_ReAlloc(void *pMem, QMUINT32 size) {
    return realloc(pMem, size);
}

#endif