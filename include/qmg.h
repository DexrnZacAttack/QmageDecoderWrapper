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
#include <QmageDecAniInfo.h>
#include <QmageDecoderInfo.h>
#include <QmageDecoderError.h>

extern "C" {
    extern QMINT32 QmageDecCommon_GetVersion();
    extern QMINT32 QmageDecCommon_GetOpaqueInfo(QMUCHAR *pInputStream);
    extern QM_BOOL QmageDecCommon_GetDecoderInfo(QMUCHAR *pInputStream, QMINT32 input_size, QmageDecoderInfo *pDecoder_info);
    // typedef QM_BOOL (*QmageDecCommon_ParseHeaderFunc)(QMUCHAR *pInputStream, QmageIOType io_type, QMINT32 input_size, QmageDecoderHeader *pHeader_info);
    extern QM_BOOL QmageDecCommon_GetAniDecoderInfo(QmageDecAniInfo *pAniDecInfo, QmageDecoderInfo *decoder_info);
    extern QM_BOOL QmageDecVersionCheck(QMUCHAR *pInputStream);
    extern QmageDecoderError QmageDecGetLastErr(void);
    extern QMINT32 QmageDecodeFrame(QMUCHAR *pInputStream, QMINT32 input_size, QMUCHAR *pDecbuf);
    extern QM_BOOL QmageDecCommon_QGetDecoderInfo(QMUCHAR *pInputStream, QMINT32 input_size, QmageDecoderInfo *pDecoder_info);
    extern QM_BOOL QmageDecCommon_WGetDecoderInfo(QMUCHAR *pInputStream, QMINT32 input_size, QmageDecoderInfo *pDecoder_info);
    extern QM_BOOL QmageDecCommon_VGetDecoderInfo(QMUCHAR *pInputStream, QMINT32 input_size, QmageDecoderInfo *pDecoder_info);
    extern void QmageDecCommon_SetHeaderFromDecoderInfo(QmageDecoderHeader *pHeader_info, QmageDecoderInfo *pDecoder_info);
    extern QMINT32 QmageDecodeAniFrame(QmageDecAniInfo *pAniDecInfo, QMUCHAR *pDecbuf);
}

#endif