#pragma once

#define DISK_ROWS 10 // the height of the 'viewport' triangle

class GlassDisk
{
public:
    GlassDisk(TProgmemRGBPalette16 const *disk_pal, uint8_t const *disk_array, uint8_t disk_cols, uint8_t disk_offset = 0)
    {
        pal = disk_pal;
        array = disk_array;
        columns = disk_cols;
        offset = disk_offset;
    }

    // Define prefix increment operator.
    GlassDisk &operator++()
    {
        ++offset = offset % columns;
        return *this;
    }

    // Define postfix increment operator.
    GlassDisk operator++(int)
    {
        GlassDisk temp = *this;
        ++*this;
        return temp;
    }

    // Define prefix decrement operator.
    GlassDisk &operator--()
    {
        if (offset == 0)
            offset = columns;
        --offset;
        return *this;
    }

    // Define postfix decrement operator.
    GlassDisk operator--(int)
    {
        GlassDisk temp = *this;
        --*this;
        return temp;
    }

    // TODO: make these private and add getter/setter methods for any that are needed
    TProgmemRGBPalette16 const *pal;
    uint8_t const *array;
    uint8_t columns;
    uint8_t offset;
};

// some default pallets and glass disks
extern const TProgmemRGBPalette16 JewelColors_p;
extern GlassDisk TriangleDisk;
extern GlassDisk SquareDisk;
#ifdef DEBUG
extern const TProgmemRGBPalette16 BlackAndWhiteColors_p;
extern GlassDisk TestDisk;
#endif
