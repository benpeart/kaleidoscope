#include "main.h"
#include "Kaleidoscope.h"
#include "GlassDisk.h"

#define JEWEL_RED 0x800000
#define JEWEL_PUR 0x800080
#define JEWEL_BLU 0x000080
#define JEWEL_GRE 0x008000
#define JEWEL_YEL 0x808000
#define JEWEL_ORA 0x804000
#define JEWEL_LEA 0x27100a

const TProgmemRGBPalette16 JewelColors_p FL_PROGMEM =
    {
        JEWEL_RED,
        JEWEL_RED,
        JEWEL_PUR,
        JEWEL_PUR,

        JEWEL_BLU,
        JEWEL_BLU,
        JEWEL_GRE,
        JEWEL_GRE,

        JEWEL_YEL,
        JEWEL_YEL,
        JEWEL_ORA,
        JEWEL_ORA,

        JEWEL_RED,
        JEWEL_RED,
        JEWEL_LEA,
        JEWEL_LEA};

// These disks define color as an index into a CRGBPalette16 rather than an absolute RGB value.
// This takes less Flash to store and gives us flexability to change the palette separate from
// the disk structure.
#define TRIANGLE_DISK_COLUMNS 9
static const PROGMEM uint8_t TriangleDiskArray[DISK_ROWS * TRIANGLE_DISK_COLUMNS] =
    {
        11,  1,  2,  3,  3,  3,  0,  4,  0,
         1,  1,  1,  2,  3,  0,  4,  4,  4,
         6,  1,  2,  2,  2,  3,  0,  4,  6,
         6,  6,  7,  2,  3,  3,  3,  4,  0,
         6,  7,  7,  7,  8,  3,  4,  4,  4,
         0, 12,  7,  8,  8,  8,  9,  4,  5,
        12, 12, 12, 13,  8,  9,  9,  9, 10,
         1, 12, 13, 13, 13, 14,  9, 10, 10,
         1,  1,  2, 13, 14, 14, 14, 15, 10,
         1,  2,  2,  2,  3, 14, 15, 15, 15};

GlassDisk TriangleDisk(&JewelColors_p, TriangleDiskArray, TRIANGLE_DISK_COLUMNS);

#define SQUARE_DISK_COLUMNS 16
static const PROGMEM uint8_t SquareDiskArray[DISK_ROWS * SQUARE_DISK_COLUMNS] =
    {
          0,  0,  0,  6,  6,  6,  8,  8,  8,  2,  2,  2,  4,  4,  4, 15,
          0,  0,  0,  6,  6,  6,  8,  8,  8,  2,  2,  2,  4,  4,  4, 15,
          0,  0,  0,  6,  6,  6,  8,  8,  8,  2,  2,  2,  4,  4,  4, 15,
          6,  6,  6, 10, 10, 10,  4,  4,  4,  8,  8,  8,  0,  0,  0, 15,
          6,  6,  6, 10, 10, 10,  4,  4,  4,  8,  8,  8,  0,  0,  0, 15,
          6,  6,  6, 10, 10, 10,  4,  4,  4,  8,  8,  8,  0,  0,  0, 15,
         10, 10, 10,  2,  2,  2,  6,  6,  6, 10, 10, 10,  0,  0,  0, 15,
         10, 10, 10,  2,  2,  2,  6,  6,  6, 10, 10, 10,  0,  0,  0, 15,
         10, 10, 10,  2,  2,  2,  6,  6,  6, 10, 10, 10,  0,  0,  0, 15,
         10, 10, 10,  2,  2,  2,  6,  6,  6, 10, 10, 10,  0,  0,  0, 15};

GlassDisk SquareDisk(&JewelColors_p, SquareDiskArray, SQUARE_DISK_COLUMNS);

#ifdef DEBUG
const TProgmemRGBPalette16 BlackAndWhiteColors_p FL_PROGMEM =
    {
        0x000000,
        0xFFFFFF,
        0x000000,
        0xFFFFFF,
        0x000000,
        0xFFFFFF,
        0x000000,
        0xFFFFFF,
        0x000000,
        0xFFFFFF,
        0x000000,
        0xFFFFFF,
        0x000000,
        0xFFFFFF,
        0x000000,
        0xFFFFFF};

#define TEST_DISK_COLUMNS 4
static const PROGMEM uint8_t TestDiskArray[DISK_ROWS * TEST_DISK_COLUMNS] =
    {
         0,  1,  0,  1,
         0,  1,  0,  1,
         0,  1,  0,  1,
         0,  1,  0,  1,
         0,  1,  0,  1,
         0,  1,  0,  1,
         0,  1,  0,  1,
         0,  1,  0,  1,
         0,  1,  0,  1,
         0,  1,  0,  1};

GlassDisk TestDisk(&BlackAndWhiteColors_p, TestDiskArray, TEST_DISK_COLUMNS);
#endif
