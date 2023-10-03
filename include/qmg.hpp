#ifndef _QMG_H
#define _QMG_H
#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>

// switch this to typedef when i understand c/c++
#define QMULONG long
#define QMINT32 int
#define QMUINT32 unsigned int
// fix this type
#define QMUCHAR const char
#define QMCHAR const char
enum QM_BOOL {
    QM_BOOL_FALSE,
    QM_BOOL_TRUE
};
typedef void QmageDecAniInfo;

#include <QmageNinePatchedChunk.hpp>
#include <QmageRawImageType.hpp>
#include <QmageIOType.hpp>
#include <QmageDecoderHeader.hpp>
#include <Qmage_DecoderVersion.hpp>
#include <Qmage_F_DecoderVersion.hpp>
#include <Qmage_V_DecoderVersion.hpp>
#include <Qmage_VDecoderVMODE_T.hpp>
#include <QmageDecodeCodecType.hpp>
#include <Qmage_DecderLowInfo.hpp>
#include <QmageImageType.hpp>
#include <QmageImageInfo.hpp>
#include <QmageDecoderInfo.hpp>

void *handle;
typedef QMINT32 (*QmageDecCommon_GetVersionFunc)();
typedef QMINT32 (*QmageDecCommon_GetOpaqueInfoFunc)(QMUCHAR *pInputStream);
typedef QM_BOOL (*QmageDecCommon_GetDecoderInfoFunc)(QMUCHAR *pInputStream, QMINT32 input_size, QmageDecoderInfo *pDecoder_info);
typedef QM_BOOL (*QmageDecCommon_ParseHeaderFunc)(QMUCHAR *pInputStream, QmageIOType io_type, QMINT32 input_size, QmageDecoderHeader *pHeader_info);
typedef QM_BOOL (*QmageDecCommon_GetAniDecoderInfoFunc)(QmageDecAniInfo *pAniDecInfo, QmageDecoderInfo *decoder_info);

QmageDecCommon_GetVersionFunc QmageDecCommon_GetVersion;
QmageDecCommon_GetOpaqueInfoFunc QmageDecCommon_GetOpaqueInfo;
QmageDecCommon_GetDecoderInfoFunc QmageDecCommon_GetDecoderInfo;
QmageDecCommon_ParseHeaderFunc QmageDecCommon_ParseHeader;
QmageDecCommon_GetAniDecoderInfoFunc QmageDecCommon_GetAniDecoderInfo;

void unloadLibrary(void) {
    // Close the library
    if (handle != NULL) {
        dlclose(handle);
    }
}

void initializeFunctions(void) {
    QmageDecCommon_GetVersion = (QmageDecCommon_GetVersionFunc)dlsym(handle, "QmageDecCommon_GetVersion");
    if (QmageDecCommon_GetVersion == NULL) {
        fprintf(stderr, "Unable to get symbol\n");
        unloadLibrary();
    }
    QmageDecCommon_GetOpaqueInfo = (QmageDecCommon_GetOpaqueInfoFunc)dlsym(handle, "QmageDecCommon_GetOpaqueInfo");
    if (QmageDecCommon_GetOpaqueInfo == NULL) {
        fprintf(stderr, "Unable to get symbol\n");
        unloadLibrary();
    }
    QmageDecCommon_GetDecoderInfo = (QmageDecCommon_GetDecoderInfoFunc)dlsym(handle, "QmageDecCommon_GetDecoderInfo");
    if (QmageDecCommon_GetDecoderInfo == NULL) {
        fprintf(stderr, "Unable to get symbol\n");
        unloadLibrary();
    }
    QmageDecCommon_ParseHeader = (QmageDecCommon_ParseHeaderFunc)dlsym(handle, "QmageDecCommon_ParseHeader");
    if (QmageDecCommon_ParseHeader == NULL) {
        fprintf(stderr, "Unable to get symbol\n");
        unloadLibrary();
    }
    QmageDecCommon_GetAniDecoderInfo = (QmageDecCommon_GetAniDecoderInfoFunc)dlsym(handle, "QmageDecCommon_GetAniDecoderInfo");
    if (QmageDecCommon_GetAniDecoderInfo == NULL) {
        fprintf(stderr, "unable to get symbol\n");
        unloadLibrary();
    }
}

void loadLibrary(void) {
    handle = dlopen("/system/lib/libQmageDecoder.so", RTLD_NOW | RTLD_GLOBAL);
    if (handle == NULL) {
        fprintf(stderr, "Unable to open lib: %s\n", dlerror());
    }
    initializeFunctions();
}
#endif