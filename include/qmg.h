#ifndef _QMG_H
#define _QMG_H
#include <stdio.h>
#include <stdint.h>
#include <dlfcn.h>

#ifdef QMAGE_STATIC_DEC
#define QmageDecGetLastErr QuramQmageDecGetLastErr
#define QmageDecodeFrame QuramQmageDecodeFrame
#define QmageDecodeAniFrame QuramQmageDecodeAniFrame
#define QmageDecCreateAniInfo QuramQmageDecCreateAniInfo
#define QmageDecDestroyAniInfo QuramQmageDecDestroyAniInfo
#define QmageDecGetNinePatchedInfo QuramQmageDecGetNinePatchedInfo
#define QmageDecGetVersion QuramQmageDecGetVersion
#define QmageDecOrgOpaqueInfo QuramQmageDecOrgOpaqueInfo
#define QmageDecParseHeader QuramQmageDecParseHeader
#define QmageMakeColorTable QuramQmageMakeColorTable
#else
#define QMAGE_ANI_SUPPORT
#endif

#ifndef LIBRARY_REV
#define LIBRARY_REV 13
#endif

typedef unsigned long QMULONG;
typedef long QMLONG;
typedef int QMINT32;
typedef unsigned int QMUINT32;
// fix this type
typedef const char QMUCHAR;
typedef char QMCHAR;
enum QM_BOOL {
    QM_BOOL_FALSE,
    QM_BOOL_TRUE
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
#include <QmageDecoderAniInfo.h>
#include <QmageDecoderInfo.h>
#include <QmageDecoderError.h>

#ifdef __cplusplus
extern "C" {
#endif
    QM_BOOL QmageDecVersionCheck(QMUCHAR *pInputStream);
    QmageDecoderError QmageDecGetLastErr(void);
    QMINT32 QmageDecodeFrame(QMUCHAR *pInputStream, QMINT32 input_size, QMUCHAR *pDecbuf);
#ifdef QMAGE_ANI_SUPPORT
    QMINT32 QmageDecodeAniFrame(QmageDecAniInfo *pAniDecInfo, QMUCHAR *pDecbuf);

    QmageDecAniInfo * QmageDecCreateAniInfo(void *pInput,QmageIOType input_io_type,QMINT32 input_size);
    void QmageDecDestroyAniInfo(QmageDecAniInfo *AniDecInfo);
#endif
    QM_BOOL QmageDecGetNinePatchedInfo(QMUCHAR *pInputStream,QMINT32 EncodedSize,QmageNinePatchedChunk *pChunk);
    void QmageDecGetVersion(QMUINT32 *pDecoderVer);
    QM_BOOL QmageDecOrgOpaqueInfo(QMUCHAR *pInputStream);
    QM_BOOL QmageDecParseHeader(QMUCHAR *pInputStream,QmageIOType io_type,QMINT32 input_size, QmageDecoderHeader *pHeader_info);
    QM_BOOL QmageMakeColorTable(QMUCHAR *pInputStream,QMINT32 input_size,QMUINT32 *pColorTable);

    QMINT32 QmageDecCommon_GetVersion();
    QMINT32 QmageDecCommon_GetOpaqueInfo(QMUCHAR *pInputStream);
    QM_BOOL QmageDecCommon_GetDecoderInfo(QMUCHAR *pInputStream, QMINT32 input_size, QmageDecoderInfo *pDecoder_info);
    // typedef QM_BOOL (*QmageDecCommon_ParseHeaderFunc)(QMUCHAR *pInputStream, QmageIOType io_type, QMINT32 input_size, QmageDecoderHeader *pHeader_info);
#ifdef QMAGE_ANI_SUPPORT
    QM_BOOL QmageDecCommon_GetAniDecoderInfo(QmageDecAniInfo *pAniDecInfo, QmageDecoderInfo *decoder_info);
#endif
    QM_BOOL QmageDecCommon_QGetDecoderInfo(QMUCHAR *pInputStream, QMINT32 input_size, QmageDecoderInfo *pDecoder_info);
    QM_BOOL QmageDecCommon_WGetDecoderInfo(QMUCHAR *pInputStream, QMINT32 input_size, QmageDecoderInfo *pDecoder_info);
    QM_BOOL QmageDecCommon_VGetDecoderInfo(QMUCHAR *pInputStream, QMINT32 input_size, QmageDecoderInfo *pDecoder_info);
    void QmageDecCommon_SetHeaderFromDecoderInfo(QmageDecoderHeader *pHeader_info, QmageDecoderInfo *pDecoder_info);
#ifdef __cplusplus
}
#endif

#endif
