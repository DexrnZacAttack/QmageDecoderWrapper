#include <qmg.h>

struct QmageDecoderInfo {
    QmageImageInfo imageInfo;
    QMINT32 header_len;
    QMINT32 alpha_position;
    // again
    QM_BOOL Animation;
    // tf is this
    QM_BOOL is_alpha_InOrgImage;
    QM_BOOL premultiplied;
    QMINT32 ColorCount;
    // bro I've about had it
    QM_BOOL Use_chromakey;
    QMINT32 mode;
    // I will steal your kneecaps
    QMINT32 qp;
    QM_BOOL rgba_order;
    // endian isn't true or false dumbass
    QM_BOOL endian;
    QMINT32 totalFrameNumber;
    QMINT32 currentFrameNumber;
    // nah fam
    QMINT32 Animation_delaytime;
    QMINT32 Animation_NoRepeat;
    QmageDecodeCodecType encoder_mode;
    Qmage_DecderLowInfo DecLowInfo;
    // this is commented out because fuck you
    // QmageDecAniInfo *pAniDecInfo;
    // Nine patch deez nuts
    QM_BOOL NinePatched;
    QMUCHAR *p9PatchData;
    QM_BOOL UseIndexedColor;
    QM_BOOL isGrayType;
    // does this not support android anywhere...?
    QM_BOOL AndroidSupport;
};