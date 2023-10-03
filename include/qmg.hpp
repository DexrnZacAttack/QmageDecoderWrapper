#ifndef _QMG_H
#define _QMG_H
#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>

#define QMULONG long
#define QMINT32 int
#define QMUCHAR const char
enum QM_BOOL {
    QM_BOOL_FALSE,
    QM_BOOL_TRUE
};

#include "Qmage_DecoderVersion.hpp"
#include "Qmage_F_DecoderVersion.hpp"
#include "Qmage_V_DecoderVersion.hpp"
#include "Qmage_VDecoderVMODE_T.hpp"
#include "QmageRawImageType.hpp"
#include "QmageDecodeCodecType.hpp"
#include "Qmage_DecderLowInfo.hpp"
#include "QmageImageType.hpp"
#include "QmageImageInfo.hpp"
#include "QmageDecoderInfo.hpp"

void *handle;
typedef QMINT32 (*QmageDecCommon_GetVersionFunc)();
typedef QMINT32 (*QmageDecCommon_GetOpaqueInfoFunc)(QMUCHAR *pInputStream);
typedef QM_BOOL (*QmageDecCommon_GetDecoderInfoFunc)(QMUCHAR *pInputStream, QMINT32 input_size, QmageDecoderInfo *pDecoder_info);

QmageDecCommon_GetVersionFunc QmageDecCommon_GetVersion;
QmageDecCommon_GetOpaqueInfoFunc QmageDecCommon_GetOpaqueInfo;
QmageDecCommon_GetDecoderInfoFunc QmageDecCommon_GetDecoderInfo;

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
}

void loadLibrary(void) {
    handle = dlopen("./libQmageDecoder.so", RTLD_NOW | RTLD_GLOBAL);
    if (handle == NULL) {
        fprintf(stderr, "Unable to open lib: %s\n", dlerror());
    }
    initializeFunctions();
}
#endif