struct QmageDecoderHeader {
    QMINT32 mode;
    QMINT32 width;
    QMINT32 height;
#if LIBRARY_REV > 11
    // TODO verify version this was introduced in
    QMINT32 padding;
#endif
    QmageRawImageType raw_type;
    QM_BOOL transparency;
#if LIBRARY_REV > 11
    // TODO verify version this was introduced in
    QM_BOOL NinePatched;
#endif
#if LIBRARY_REV > 13
    // TODO verify version this was introduced in
    // TODO is this placement correct?
    QM_BOOL Unknown;
#endif
    QM_BOOL IsOpaque;
#if LIBRARY_REV > 11
    // TODO verify version this was introduced in
    QM_BOOL premultiplied;
    // TODO verify version this was introduced in
    QMINT32 ColorCount;
    // TODO verify version this was introduced in
    QM_BOOL UseIndexedColor;
    // TODO verify version this was introduced in
    QM_BOOL isGrayColor;
#endif
    QM_BOOL rgba_order;
    QMINT32 totalFrameNumber;
    QMINT32 currentFrameNumber;
    QMINT32 Animation_delaytime;
    QMINT32 Animation_NoRepeat;
};
