#include <stdio.h>
#include <qmg.hpp>

// To the poor soul who looks at this
// I do not know C/C++ in my defense
int main(void) {
    printf("WE ARE SO FUCKING BALL!\n");    

    loadLibrary();
    int version = QmageDecCommon_GetVersion();
    printf("QmageDecoder version: %d\n", version);
    int opaqueInfo = QmageDecCommon_GetOpaqueInfo("example.qmg");
    printf("QmageDecoder opaqueInfo: %d\n", opaqueInfo);
    // QmageDecCommon_GetDecoderInfo("example.qmg", input_size, );
    unloadLibrary();

    return 0;
}