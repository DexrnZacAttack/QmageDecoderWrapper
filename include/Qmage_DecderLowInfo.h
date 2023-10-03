struct Qmage_DecderLowInfo {
    // reload bitch
    QM_BOOL Ver200_SPEED;
    QM_BOOL IS_ANIMATION;
    // this ain't java
    QM_BOOL UseExtraException;
    QM_BOOL tiny;
    // dyanmic
    QM_BOOL IsDyanmicTable;
    QM_BOOL IsOpaque;
    // loss
    QM_BOOL NearLossless;
    QMINT32 SIZE_SHIFT;
    // ani e
    QMINT32 ANI_RANGE;
    // why is this defined twice
    QMINT32 qp;
    QMINT32 mode_bit;
    QMINT32 header_len;
    QM_BOOL NotComp;
    QM_BOOL NotAlphaComp;
    QMINT32 alpha_decode_flag;
    QMINT32 depth;
    QMINT32 alpha_depth;
    // fuck another struct 😭
    Qmage_VDecoderVMODE_T mode;
    // no bitches
    Qmage_V_DecoderVersion vversion;
    Qmage_F_DecoderVersion fversion;
    Qmage_DecoderVersion qversion;
    QmageDecodeCodecType rgb_encoder_mode;
    QmageDecodeCodecType alpha_encoder_mode;
    QmageRawImageType out_type;
};