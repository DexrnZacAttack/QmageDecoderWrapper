#include <qmg.h>

struct QmageDecoderAniInfo {
  QMUCHAR *pPreImage16;
  QMUCHAR *pPreAlphaImage;
  QMUCHAR *pInputBuffer;
  QMUCHAR *pReadbuffer;
  void *pInput;
  QmageIOType input_type;
  QMINT32 input_size;
  QMINT32 *filehandler;
  QMINT32 filepos;
  QMUINT32 bytesInBuffer;
  QMUINT32 bufferpoint;
  QMUINT32 iBufferCount;
  QMUINT32 MINMUM_READ_SIZE;
};