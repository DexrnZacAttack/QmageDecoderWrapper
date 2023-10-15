#include <qmg.h>
#include <quram.h>
#include <iostream>
#include <cstring>

int QM_HEADER = 19793; // 'QM..'
int QW_HEADER = 22353; // 'QW..'
int IM_HEADER = 19785; // 'IM..'
int IFEG_HEADER = 1195722313; // 'IFEG..'
int PFR_HEADER = 1616004688; // 'PFR..'

int debug_QmageDecError = 0;

QM_BOOL QmageDecCommon_ParseHeader(QMUCHAR *pInputStream, QmageIOType io_type, QMINT32 input_size, QmageDecoderHeader *pHeader_info)
{
    if (!pInputStream || !pHeader_info)
    {
        debug_QmageDecError = -7;
        return QM_BOOL_FALSE;
    }

    std::memset(pHeader_info, 0, sizeof(QmageDecoderHeader));

    if (io_type == QM_IO_FILE)
    {
        if (input_size <= 0)
        {
            return QM_BOOL_FALSE;
        }

        int v12 = pInputStream[1];
        int v13 = pInputStream[0];
        int v14 = (v12 << 8) | v13;
        std::cout << v12 << " " << v13 << " " << v14 << std::endl;

        if (v14 == QM_HEADER)
        {
            QmageDecoderInfo decoder_info;
            if (QmageDecCommon_QGetDecoderInfo(pInputStream, input_size, &decoder_info))
            {
                QmageDecCommon_SetHeaderFromDecoderInfo(pHeader_info, &decoder_info);
                return QM_BOOL_TRUE;
            }
        }
        else if (v14 == IM_HEADER || (v14 == IFEG_HEADER) || (v13 == 73 && v12 == 84))
        {
            QmageDecoderInfo decoder_info;
            if (QmageDecCommon_VGetDecoderInfo(pInputStream, input_size, &decoder_info))
            {
                QmageDecCommon_SetHeaderFromDecoderInfo(pHeader_info, &decoder_info);
                return QM_BOOL_TRUE;
            }
            else
            {
                debug_QmageDecError = -3;
                return QM_BOOL_FALSE;
            }
        }
        else if (v14 == QW_HEADER)
        {
            QmageDecoderInfo decoder_info;
            if (QmageDecCommon_WGetDecoderInfo(pInputStream, input_size, &decoder_info))
            {
                QmageDecCommon_SetHeaderFromDecoderInfo(pHeader_info, &decoder_info);
                return QM_BOOL_TRUE;
            }
            else
            {
                debug_QmageDecError = -3;
                return QM_BOOL_FALSE;
            }
        }
        else if (v14 != PFR_HEADER)
        {
            std::cout << v14 << std::endl;
            return QM_BOOL_FALSE;
        }
        else
        {
            QmageDecoderInfo decoder_info;
            if (QmageDecCommon_QGetDecoderInfo(pInputStream, input_size, &decoder_info))
            {
                QmageDecCommon_SetHeaderFromDecoderInfo(pHeader_info, &decoder_info);
                return QM_BOOL_TRUE;
            }
            else
            {
                debug_QmageDecError = -3;
                return QM_BOOL_FALSE;
            }
        }
    }
    else
    {
        QMUCHAR Header[40] = { 0 };
        QMINT32 *stream = (QMINT32 *)QuramQmage_Fopen(pInputStream, "rb");
        if (stream)
        {
            if (input_size > 0)
            {
                QMINT32 v18;
                QuramQmage_Fseek(stream, 0, 2);
                v18 = QuramQmage_Ftell(stream);
                QuramQmage_Fseek(stream, 0, 0);

                if (v18 > 11)
                {
                    if (QuramQmage_Fread((void *)Header, 1u, 0x28u, stream) > 11)
                    {
                        pInputStream = Header;
                    }
                    else
                    {
                        QuramQmage_Fclose(stream);
                        debug_QmageDecError = -2;
                        return QM_BOOL_FALSE;
                    }
                }
                else
                {
                    QuramQmage_Fclose(stream);
                    debug_QmageDecError = -2;
                }
            }
            else
            {
                debug_QmageDecError = -2;
            }
        }
        else
        {
            debug_QmageDecError = -7;
        }
    }

    return QM_BOOL_FALSE;
}

int get_extra_error() {
    return debug_QmageDecError;
}