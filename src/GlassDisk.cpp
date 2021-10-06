#include "main.h"
#include "Kaleidoscope.h"
#include "GlassDisk.h"

// custom created to emulate the airplane kaleidoscope that started it all
#define JEWEL_RED 0x800000
#define JEWEL_PUR 0x800080
#define JEWEL_BLU 0x000080
#define JEWEL_GRE 0x008000
#define JEWEL_YEL 0x808000
#define JEWEL_ORA 0x804000
#define JEWEL_LEA 0x27100a
const TProgmemRGBPalette16 JewelColors_p FL_PROGMEM =
    {
        JEWEL_RED, JEWEL_PUR, JEWEL_BLU, JEWEL_GRE,
        JEWEL_YEL, JEWEL_ORA, JEWEL_RED, JEWEL_PUR,
        JEWEL_BLU, JEWEL_GRE, JEWEL_YEL, JEWEL_ORA,
        JEWEL_RED, JEWEL_PUR, JEWEL_BLU, JEWEL_LEA};

// https://coolors.co/006ba6-0496ff-ffbc42-d81159-8f2d56
#define SAPPHIREBLUE 0x006BA6
#define DODGERBLUE 0x0496FF
#define MAXIMUMYELLOWRED 0xFFBC42
#define RUBY 0xD81159
#define QUINACRIDONEMAGENTA 0x8F2D56
const TProgmemRGBPalette16 BlueRed_p FL_PROGMEM =
    {SAPPHIREBLUE, SAPPHIREBLUE, SAPPHIREBLUE, SAPPHIREBLUE,
     DODGERBLUE, DODGERBLUE, DODGERBLUE, DODGERBLUE,
     RUBY, RUBY, RUBY, RUBY,
     QUINACRIDONEMAGENTA, QUINACRIDONEMAGENTA, QUINACRIDONEMAGENTA, QUINACRIDONEMAGENTA};

// https://coolors.co/001427-708d81-f4d58d-bf0603-8d0801
#define RICHBLACK 0x001427
#define XANADU 0x708D81
#define JASMINE 0xF4D58D
#define INTERNATIONALORANGEENGINEERING 0xBF0603
#define DARKRED 0x8D0801
const TProgmemRGBPalette16 RedAndBlue_p FL_PROGMEM =
    {RICHBLACK, XANADU, JASMINE, INTERNATIONALORANGEENGINEERING,
     DARKRED, RICHBLACK, XANADU, JASMINE,
     INTERNATIONALORANGEENGINEERING, DARKRED, RICHBLACK, XANADU,
     JASMINE, INTERNATIONALORANGEENGINEERING, DARKRED, RICHBLACK};

// https://coolors.co/ffbe0b-fb5607-ff006e-8338ec-3a86ff
#define MANGO 0xFFBE0B
#define ORANGEPANTONE 0xFB5607
#define WINTERSKY 0xFF006E
#define BLUEVIOLET 0x8338EC
#define AZURE 0x3A86FF
const TProgmemRGBPalette16 Tropical_p FL_PROGMEM =
    {MANGO, ORANGEPANTONE, WINTERSKY, BLUEVIOLET,
     AZURE, MANGO, ORANGEPANTONE, WINTERSKY, 
     BLUEVIOLET, AZURE, MANGO, ORANGEPANTONE,
     WINTERSKY, BLUEVIOLET, AZURE, MANGO};

// A palette reminiscent of large 'old-school' C9-size tree lights
// in the five classic colors: red, orange, green, blue, and white.
#define C9_Red 0xB80400
#define C9_Orange 0x902C02
#define C9_Green 0x046002
#define C9_Blue 0x070758
#define C9_White 0x606820
const TProgmemRGBPalette16 RetroC9_p FL_PROGMEM =
    {C9_Red, C9_Orange, C9_Green, C9_Blue,
     C9_White, C9_Red, C9_Orange, C9_Green,
     C9_Blue, C9_White, C9_Red, C9_Orange,
     C9_Green, C9_Blue, C9_White, C9_Red};

// https://coolors.co/001219-005f73-0a9396-94d2bd-e9d8a6-ee9b00-ca6702-bb3e03-ae2012-9b2226
#define RICHBLACKFOGRA29 0X001219
#define BLUESAPPHIRE 0X005F73
#define VIRIDIANGREEN 0x0A9396
#define MIDDLEBLUEGREEN 0x94D2BD
#define MEDIUMCHAMPAGNE 0xE9D8A6
#define GAMBOGE 0xEE9B00
#define ALLOYORANGE 0xCA6702
#define MAHOGANY 0xBB3E03
#define RUFOUS 0xAE2012
#define RUBYRED 0x9B2226
const TProgmemRGBPalette16 Warm_p FL_PROGMEM =
    {
        RICHBLACKFOGRA29, BLUESAPPHIRE, VIRIDIANGREEN, MIDDLEBLUEGREEN,
        MEDIUMCHAMPAGNE, GAMBOGE, ALLOYORANGE, MAHOGANY,
        RUFOUS, RUBYRED, RICHBLACKFOGRA29, BLUESAPPHIRE,
        VIRIDIANGREEN, MIDDLEBLUEGREEN, MEDIUMCHAMPAGNE, GAMBOGE};

// Add or remove palette names from this list to control which color
// palettes are used, and in what order.
const TProgmemRGBPalette16 *ActiveDiskPaletteList[] = {
    &JewelColors_p,
    &Tropical_p,
    &RetroC9_p,
    &BlueRed_p,
    &RainbowColors_p,
    &RedAndBlue_p,
    &PartyColors_p,
    &Warm_p};

// Advance to the next color palette in the list (above).
void chooseNextDiskPalette(CRGBPalette16 &pal)
{
    const uint8_t numberOfPalettes = sizeof(ActiveDiskPaletteList) / sizeof(ActiveDiskPaletteList[0]);
    static uint8_t whichPalette = -1;
    whichPalette = addmod8(whichPalette, 1, numberOfPalettes);

    pal = *(ActiveDiskPaletteList[whichPalette]);
}

CRGBPalette16 gCurrentDiskPalette;

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

GlassDisk TriangleDisk(&gCurrentDiskPalette, TriangleDiskArray, TRIANGLE_DISK_COLUMNS);

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

GlassDisk SquareDisk(&gCurrentDiskPalette, SquareDiskArray, SQUARE_DISK_COLUMNS);

#ifdef DEBUG
const CRGBPalette16 BlackAndWhiteColors_p FL_PROGMEM =
    {
        0x000000, 0xFFFFFF, 0x000000, 0xFFFFFF,
        0x000000, 0xFFFFFF, 0x000000, 0xFFFFFF,
        0x000000, 0xFFFFFF, 0x000000, 0xFFFFFF,
        0x000000, 0xFFFFFF, 0x000000, 0xFFFFFF};

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
