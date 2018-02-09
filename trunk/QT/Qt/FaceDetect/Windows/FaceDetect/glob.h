#ifndef GLOB
#define GLOB

enum enumItemType {
    TYPE_ALL = 0xFF,
    TYPE_PIMPLES = 0x1,
    TYPE_BLACKHEADS = 0x2,
    TYPE_FACE_COLOR = 0x4,
    TYPE_PORE = 0x8,
    TYPE_COARSENESS = 0x10
};

#define g_tmpDirPimples "./tmp/pimples/"
#define g_tmpDirBlackheads "./tmp/blackheads/"
#define g_tmpDirFaceColor "./tmp/color/"
#define g_tmpDirPore "./tmp/pore/"
#define g_tmpDirCoarse "./tmp/coarse/"

#endif // GLOB

