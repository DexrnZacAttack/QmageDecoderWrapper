#include <qmg.h>

struct QmageImageInfo {
    QMINT32 img_width;
    QMINT32 img_height;
    QMINT32 bpp;
    QM_BOOL transparency;
#if LIBRARY_REV > 11
    // TODO verify version this was introduced in
    QMINT32 padding;
    // TODO verify version this was introduced in
    QMINT32 PaddingValue;
#endif
    QmageRawImageType raw_type;
    QmageImageType img_type;
};
