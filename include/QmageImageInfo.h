struct QmageImageInfo {
    QMINT32 img_width;
    QMINT32 img_height;
    QMINT32 bpp;
    QM_BOOL transparency;
    QMINT32 padding;
    // why is this capitalized
    QMINT32 PaddingValue;
    QmageRawImageType raw_type;
    QmageImageType img_type;
};