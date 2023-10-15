#ifndef _QMG_H
#define _QMG_H
#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>

// switch this to typedef when i understand c/c++
#define QMULONG unsigned long
#define QMLONG long
#define QMINT32 int
#define QMUINT32 unsigned int
// fix this type
#define QMUCHAR const char
#define QMCHAR char
enum QM_BOOL : int32_t {
  QM_BOOL_FALSE = 0x0,
  QM_BOOL_TRUE = 0x1,
};
typedef void QmageDecAniInfo;

#include <quram.h>
#include <QmageNinePatchedChunk.h>
#include <QmageRawImageType.h>
#include <QmageIOType.h>
#include <QmageDecoderHeader.h>
#include <Qmage_DecoderVersion.h>
#include <Qmage_F_DecoderVersion.h>
#include <Qmage_V_DecoderVersion.h>
#include <Qmage_VDecoderVMODE_T.h>
#include <QmageDecodeCodecType.h>
#include <Qmage_DecderLowInfo.h>
#include <QmageImageType.h>
#include <QmageImageInfo.h>
#include <QmageDecoderInfo.h>
#include <QmageDecoderError.h>

void *handle;
typedef QMINT32 (*QmageDecCommon_GetVersionFunc)();
typedef QMINT32 (*QmageDecCommon_GetOpaqueInfoFunc)(QMUCHAR *pInputStream);
typedef QM_BOOL (*QmageDecCommon_GetDecoderInfoFunc)(QMUCHAR *pInputStream, QMINT32 input_size, QmageDecoderInfo *pDecoder_info);
// typedef QM_BOOL (*QmageDecCommon_ParseHeaderFunc)(QMUCHAR *pInputStream, QmageIOType io_type, QMINT32 input_size, QmageDecoderHeader *pHeader_info);
typedef QM_BOOL (*QmageDecCommon_GetAniDecoderInfoFunc)(QmageDecAniInfo *pAniDecInfo, QmageDecoderInfo *decoder_info);
typedef QM_BOOL (*QmageDecVersionCheckFunc)(QMUCHAR *pInputStream);
typedef QmageDecoderError (*QmageDecGetLastErrFunc)(void);
typedef QMINT32 (*QmageDecodeFrameFunc)(QMUCHAR *pInputStream, QMINT32 input_size, QMUCHAR *pDecbuf);
typedef QM_BOOL (*QmageDecCommon_QGetDecoderInfoFunc)(QMUCHAR *pInputStream, QMINT32 input_size, QmageDecoderInfo *pDecoder_info);
typedef QM_BOOL (*QmageDecCommon_WGetDecoderInfoFunc)(QMUCHAR *pInputStream, QMINT32 input_size, QmageDecoderInfo *pDecoder_info);
typedef QM_BOOL (*QmageDecCommon_VGetDecoderInfoFunc)(QMUCHAR *pInputStream, QMINT32 input_size, QmageDecoderInfo *pDecoder_info);
typedef void (*QmageDecCommon_SetHeaderFromDecoderInfoFunc)(QmageDecoderHeader *pHeader_info, QmageDecoderInfo *pDecoder_info);

QmageDecCommon_GetVersionFunc QmageDecCommon_GetVersion;
QmageDecCommon_GetOpaqueInfoFunc QmageDecCommon_GetOpaqueInfo;
QmageDecCommon_GetDecoderInfoFunc QmageDecCommon_GetDecoderInfo;
// QmageDecCommon_ParseHeaderFunc QmageDecCommon_ParseHeader;
QmageDecCommon_GetAniDecoderInfoFunc QmageDecCommon_GetAniDecoderInfo;
QmageDecVersionCheckFunc QmageDecVersionCheck;
QmageDecGetLastErrFunc QmageDecGetLastErr;
QmageDecodeFrameFunc QmageDecodeFrame;
QmageDecCommon_QGetDecoderInfoFunc QmageDecCommon_QGetDecoderInfo;
QmageDecCommon_WGetDecoderInfoFunc QmageDecCommon_WGetDecoderInfo;
QmageDecCommon_VGetDecoderInfoFunc QmageDecCommon_VGetDecoderInfo;
QmageDecCommon_SetHeaderFromDecoderInfoFunc QmageDecCommon_SetHeaderFromDecoderInfo;

void unloadLibrary(void) {
    // Close the library
    if (handle != NULL) {
        dlclose(handle);
    }
}

void initializeFunctions() {
    QmageDecCommon_GetVersion = (QmageDecCommon_GetVersionFunc)dlsym(handle, "QmageDecCommon_GetVersion");
    if (QmageDecCommon_GetVersion == nullptr) {
        fprintf(stderr, "Unable to get symbol\n");
        unloadLibrary();
    }
    QmageDecCommon_GetOpaqueInfo = (QmageDecCommon_GetOpaqueInfoFunc)dlsym(handle, "QmageDecCommon_GetOpaqueInfo");
    if (QmageDecCommon_GetOpaqueInfo == nullptr) {
        fprintf(stderr, "Unable to get symbol\n");
        unloadLibrary();
    }
    QmageDecCommon_GetDecoderInfo = (QmageDecCommon_GetDecoderInfoFunc)dlsym(handle, "QmageDecCommon_GetDecoderInfo");
    if (QmageDecCommon_GetDecoderInfo == nullptr) {
        fprintf(stderr, "Unable to get symbol\n");
        unloadLibrary();
    }
    /*QmageDecCommon_ParseHeader = (QmageDecCommon_ParseHeaderFunc)dlsym(handle, "QmageDecCommon_ParseHeader");
    if (QmageDecCommon_ParseHeader == nullptr) {
        fprintf(stderr, "Unable to get symbol\n");
        unloadLibrary();
    }*/
    QmageDecCommon_GetAniDecoderInfo = (QmageDecCommon_GetAniDecoderInfoFunc)dlsym(handle, "QmageDecCommon_GetAniDecoderInfo");
    if (QmageDecCommon_GetAniDecoderInfo == nullptr) {
        fprintf(stderr, "unable to get symbol\n");
        unloadLibrary();
    }
    QmageDecVersionCheck = (QmageDecVersionCheckFunc)dlsym(handle, "QmageDecVersionCheck");
    if (QmageDecVersionCheck == nullptr) {
        fprintf(stderr, "unable to get symbol\n");
        unloadLibrary();
    }
    QmageDecGetLastErr = (QmageDecGetLastErrFunc)dlsym(handle, "QmageDecGetLastErr");
    if (QmageDecGetLastErr == nullptr) {
        fprintf(stderr, "unable to get symbol\n");
        unloadLibrary();
    }
    QmageDecodeFrame = (QmageDecodeFrameFunc)dlsym(handle, "QmageDecodeFrame");
    if (QmageDecodeFrame == nullptr) {
        fprintf(stderr, "unable to get symbol\n");
        unloadLibrary();
    }
    QmageDecCommon_QGetDecoderInfo = (QmageDecCommon_QGetDecoderInfoFunc)dlsym(handle, "QmageDecCommon_QGetDecoderInfo");
    if (QmageDecCommon_QGetDecoderInfo == nullptr) {
        fprintf(stderr, "unable to get symbol\n");
        unloadLibrary();
    }
    QmageDecCommon_WGetDecoderInfo = (QmageDecCommon_WGetDecoderInfoFunc)dlsym(handle, "QmageDecCommon_WGetDecoderInfo");
    if (QmageDecCommon_WGetDecoderInfo == nullptr) {
        fprintf(stderr, "unable to get symbol\n");
        unloadLibrary();
    }
    QmageDecCommon_VGetDecoderInfo = (QmageDecCommon_VGetDecoderInfoFunc)dlsym(handle, "QmageDecCommon_VGetDecoderInfo");
    if (QmageDecCommon_VGetDecoderInfo == nullptr) {
        fprintf(stderr, "unable to get symbol\n");
        unloadLibrary();
    }
    QmageDecCommon_SetHeaderFromDecoderInfo = (QmageDecCommon_SetHeaderFromDecoderInfoFunc)dlsym(handle, "QmageDecCommon_SetHeaderFromDecoderInfo");
    if (QmageDecCommon_SetHeaderFromDecoderInfo == nullptr) {
        fprintf(stderr, "unable to get symbol\n");
        unloadLibrary();
    }
}

void loadLibrary() {
    handle = dlopen("/system/lib/libQmageDecoder.so", RTLD_NOW | RTLD_GLOBAL);
    if (handle == nullptr) {
        fprintf(stderr, "Unable to open lib: %s\n", dlerror());
        exit(-1);
    }
    initializeFunctions();
}
#endif