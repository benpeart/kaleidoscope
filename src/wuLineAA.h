#ifndef WULINEAA_H
#define WULINEAA_H

// make sure these function signatures match your XY()
typedef uint16_t (*XY_Func)(uint16_t, uint16_t);
uint16_t YX(uint16_t y, uint16_t x)
{
    return XY(x, y);
}

// blend between two colours by amount/255ths
// updates the first colour in-place
void crossfade(CRGB *a, const CRGB *b, uint8_t amount)
{
    uint8_t rev = 255 - amount;
    a->red = (a->red * amount + b->red * rev) >> 8;
    a->green = (a->green * amount + b->green * rev) >> 8;
    a->blue = (a->blue * amount + b->blue * rev) >> 8;
}

void wuLineAA(saccum78 x1, saccum78 y1, saccum78 x2, saccum78 y2, CRGB *col)
{
    saccum78 grad, xd;
    saccum78 xend, yend, yf;
    int8_t ix1, ix2;
    fract8 xgap, coverage;

    // reject trivially off-screen lines
    if (x1 < -255 && x2 < -255)
        return;
    if (y1 < -255 && y2 < -255)
        return;
    if ((x1 >> 8) >= WIDTH && (x2 >> 8) >= WIDTH)
        return;
    if ((y1 >> 8) >= HEIGHT && (y2 >> 8) >= HEIGHT)
        return;

    XY_Func xyfunc = XY; // use regular XY() mapper when X is the major axis
    if (abs(x2 - x1) < abs(y2 - y1))
    {
        // Y is major axis: swap X and Y and switch to the YX() mapper
        xyfunc = YX;
        saccum78 tmp;
        tmp = x1;
        x1 = y1;
        y1 = tmp;
        tmp = x2;
        x2 = y2;
        y2 = tmp;
    }

    if (x2 < x1)
    {
        // line is backwards: reverse it
        saccum78 tmp;
        tmp = x1;
        x1 = x2;
        x2 = tmp;
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }

    xd = x2 - x1;
    // Treat very short lines as unit length
    if (xd < 25)
    {
        x2 = x1 + 128;
        x1 -= 128;
        grad = 0;
        // Serial.println("Vshort");
    }
    else
    {
        grad = ((saccum1516)(y2 - y1) << 8) / xd;

        // if line length is less than 1, extend it to 1
        if (xd < 256)
        {
            // Serial.println("Short");
            // find mid point of line
            saccum78 xm = (x1 + x2) / 2;
            saccum78 ym = (y1 + y2) / 2;

            // recalculate end points so that xd=1
            x1 = xm - 128;
            x2 = xm + 128;
            y1 = ym - (grad / 2);
            y2 = ym + (grad / 2);

            xd = 256;
            grad = 0;
        }
    }

    // project to find coordinates of endpoint 1
    xend = (x1 + 128) & 0xff00;
    yend = y1 + ((grad * (xend - x1)) >> 8);

    // distance from beginning of line to next pixel boundary
    xgap = 255 - ((x1 /*+ 128*/) & 0xff);

    // calc pixel intensities
    coverage = ((yend & 0xff) * xgap) >> 8;
    ix1 = xend >> 8;
    // *col = 0xff0000;
    crossfade(&leds[xyfunc(ix1, (yend >> 8))], col, coverage);
    // *col = 0x00ff00;
    crossfade(&leds[xyfunc(ix1, (yend >> 8) + 1)], col, 255 - coverage);

    ix1++;
    yf = yend + grad;

    // project to find coordinates of endpoint 2
    xend = (x2 + 128) & 0xff00;
    yend = y2 + ((grad * (xend - x2)) >> 8);

    // distance from end of line to previous pixel boundary
    xgap = (x2 /*- 128*/) & 0xff;

    // calc pixel intensities
    coverage = ((yend & 0xff) * xgap) >> 8;

    ix2 = xend >> 8;
    // *col = 0x0000ff;
    crossfade(&leds[xyfunc(ix2, (yend >> 8))], col, coverage);
    // *col = 0xff00ff;
    crossfade(&leds[xyfunc(ix2, (yend >> 8) + 1)], col, 255 - coverage);
    // *col = 0xffffff;

    while (ix1 < ix2)
    {
        coverage = yf & 0xff;
        crossfade(&leds[xyfunc(ix1, yf >> 8)], col, coverage);
        crossfade(&leds[xyfunc(ix1, (yf >> 8) + 1)], col, 255 - coverage);
        yf += grad;
        ix1++;
    }
}

#endif
