struct QmageDecoderHeader {
    QMINT32 mode;
    QMINT32 width;
    QMINT32 height;
    QMINT32 padding;
    QmageRawImageType raw_type;
    QM_BOOL transparency;
    QM_BOOL NinePatched;
    QM_BOOL IsOpaque;
    QM_BOOL premultiplied;
    QMINT32 ColorCount;
    QM_BOOL UseIndexedColor;
    QM_BOOL isGrayColor;
    QM_BOOL rgba_order;
    QMINT32 totalFrameNumber;
    QMINT32 currentFrameNumber;
    QMINT32 Animation_delaytime;
    QMINT32 Animation_NoRepeat;
};