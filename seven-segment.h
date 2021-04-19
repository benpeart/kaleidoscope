#ifndef SEVEN_SEGMENT_H
#define SEVEN_SEGMENT_H

#include "LEDStrips.h"

/************************************************************
 * The following is functions for each of the segments
 * in a 7 segment clock
 * the ones and tens place for minutes plus the ones place
 * for hours will have 7 segments each and the tens place
 * for the hours will only ever be a one so i'll skip on
 * coding it for now. 
 * 
 * --------------------------------------------------------
 * 
 *        1
 *     -------
 *  2 |       | 3
 *    |       |
 *  4 *-------*     the '*' is triangleL & triangleR
 *    |       |     used for making the lines straighter
 *  5 |       | 6
 *     -------
 *        7
 * 
 * *********************************************************/

void drawHourTensOne(uint32_t color) //this will only ever be a 1
{
    leds.strip[0][18] = color;
    leds.strip[0][19] = color;
    leds.strip[0][20] = color;
    leds.strip[0][21] = color;
    leds.strip[0][22] = color;
    leds.strip[0][23] = color;
    leds.strip[0][24] = color;
    leds.strip[0][25] = color;
    leds.strip[0][26] = color;
    leds.strip[0][27] = color;
}

////////////////////////////////////////////

//HourOnes
void drawHourOnesOne(uint32_t color)
{
    leds.strip[1][88] = color;
    leds.strip[1][87] = color;
    leds.strip[1][86] = color;
    leds.strip[1][85] = color;
    leds.strip[1][84] = color;
    leds.strip[1][83] = color;
    leds.strip[1][82] = color;
    leds.strip[1][81] = color;
    leds.strip[1][80] = color;
    leds.strip[1][79] = color;
}

void drawHourOnesSegmentOne(uint32_t color)
{
    leds.strip[0][57] = color;
    leds.strip[0][87] = color;
    leds.strip[0][88] = color;
    leds.strip[0][102] = color;
}

void drawHourOnesSegmentTwo(uint32_t color)
{
    leds.strip[0][58] = color;
    leds.strip[0][59] = color;
    leds.strip[0][60] = color;
    leds.strip[0][61] = color;
}

void drawHourOnesSegmentThree(uint32_t color)
{
    leds.strip[0][103] = color;
    leds.strip[0][104] = color;
    leds.strip[0][105] = color;
    leds.strip[0][106] = color;
}

void drawHourOnesSegmentFour(uint32_t color)
{
    leds.strip[0][84] = color;
    leds.strip[0][85] = color;
}

void drawHourOnesSegmentFive(uint32_t color)
{
    leds.strip[0][62] = color;
    leds.strip[0][63] = color;
    leds.strip[0][64] = color;
    leds.strip[0][65] = color;
}

void drawHourOnesSegmentSix(uint32_t color)
{
    leds.strip[0][107] = color;
    leds.strip[0][108] = color;
    leds.strip[0][109] = color;
}

void drawHourOnesSegmentSeven(uint32_t color)
{
    leds.strip[0][79] = color;
    leds.strip[0][80] = color;
    leds.strip[0][110] = color;
}
////////////////////////////////////// HourOnes^

//MinTens
void drawMinTensOne(uint32_t color)
{
    leds.strip[1][55] = color;
    leds.strip[1][56] = color;
    leds.strip[1][57] = color;
    leds.strip[1][58] = color;
    leds.strip[1][59] = color;
    leds.strip[1][60] = color;
    leds.strip[1][61] = color;
    leds.strip[1][62] = color;
    leds.strip[1][63] = color;
    leds.strip[1][64] = color;
}

void drawMinTensSegmentOne(uint32_t color)
{
    leds.strip[1][41] = color;
    leds.strip[1][55] = color;
    leds.strip[1][56] = color;
    leds.strip[1][86] = color;
}

void drawMinTensSegmentTwo(uint32_t color)
{
    leds.strip[1][40] = color;
    leds.strip[1][39] = color;
    leds.strip[1][38] = color;
    leds.strip[1][37] = color;
}

void drawMinTensSegmentThree(uint32_t color)
{
    leds.strip[1][85] = color;
    leds.strip[1][84] = color;
    leds.strip[1][83] = color;
    leds.strip[1][82] = color;
}

void drawMinTensSegmentFour(uint32_t color)
{
    leds.strip[1][59] = color;
    leds.strip[1][60] = color;
}

void drawMinTensSegmentFive(uint32_t color)
{
    leds.strip[1][36] = color;
    leds.strip[1][35] = color;
    leds.strip[1][34] = color;
    leds.strip[1][33] = color;
}

void drawMinTensSegmentSix(uint32_t color)
{
    leds.strip[1][81] = color;
    leds.strip[1][80] = color;
    leds.strip[1][79] = color;
}

void drawMinTensSegmentSeven(uint32_t color)
{
    leds.strip[1][63] = color;
    leds.strip[1][74] = color;
    leds.strip[1][78] = color;
}
////////////////////////////////////// MinTens^

//MinOnes
void drawMinOnesOne(uint32_t color)
{
    leds.strip[1][125] = color;
    leds.strip[1][124] = color;
    leds.strip[1][123] = color;
    leds.strip[1][122] = color;
    leds.strip[1][121] = color;
    leds.strip[1][120] = color;
    leds.strip[1][119] = color;
    leds.strip[1][118] = color;
    leds.strip[1][117] = color;
    leds.strip[1][116] = color;
}

void drawMinOnesSegmentOne(uint32_t color)
{

    leds.strip[1][98] = color;
    leds.strip[1][124] = color;
    leds.strip[1][125] = color;
    leds.strip[1][131] = color;
}

void drawMinOnesSegmentTwo(uint32_t color)
{
    leds.strip[1][99] = color;
    leds.strip[1][100] = color;
    leds.strip[1][101] = color;
    leds.strip[1][102] = color;
}

void drawMinOnesSegmentThree(uint32_t color)
{
    leds.strip[1][132] = color;
    leds.strip[1][133] = color;
    leds.strip[1][134] = color;
    leds.strip[1][135] = color;
}

void drawMinOnesSegmentFour(uint32_t color)
{
    leds.strip[1][120] = color;
    leds.strip[1][121] = color;
}

void drawMinOnesSegmentFive(uint32_t color)
{
    leds.strip[1][103] = color;
    leds.strip[1][104] = color;
    leds.strip[1][105] = color;
    leds.strip[1][106] = color;
}

void drawMinOnesSegmentSix(uint32_t color)
{
    leds.strip[1][136] = color;
    leds.strip[1][137] = color;
    leds.strip[1][138] = color;
}

void drawMinOnesSegmentSeven(uint32_t color)
{
    leds.strip[1][116] = color;
    leds.strip[1][117] = color;
    leds.strip[1][139] = color;
}
////////////////////////////////////// MinOnes

void handleSegmentsMinOnes(int num, uint32_t color)
{
    switch (num)
    {
    case 0:
        drawMinOnesSegmentOne(color);
        drawMinOnesSegmentTwo(color);
        drawMinOnesSegmentThree(color);
        drawMinOnesSegmentFive(color);
        drawMinOnesSegmentSix(color);
        drawMinOnesSegmentSeven(color);
        break;
    case 1:
        drawMinOnesOne(color);
        break;
    case 2:
        drawMinOnesSegmentOne(color);
        drawMinOnesSegmentThree(color);
        drawMinOnesSegmentFour(color);
        drawMinOnesSegmentFive(color);
        drawMinOnesSegmentSeven(color);
        break;
    case 3:
        drawMinOnesSegmentOne(color);
        drawMinOnesSegmentThree(color);
        drawMinOnesSegmentFour(color);
        drawMinOnesSegmentSix(color);
        drawMinOnesSegmentSeven(color);
        break;
    case 4:
        drawMinOnesSegmentOne(color);
        drawMinOnesSegmentFour(color);
        drawMinOnesSegmentThree(color);
        drawMinOnesSegmentSix(color);
        break;
    case 5:
        drawMinOnesSegmentOne(color);
        drawMinOnesSegmentTwo(color);
        drawMinOnesSegmentFour(color);
        drawMinOnesSegmentSix(color);
        drawMinOnesSegmentSeven(color);
        break;
    case 6:
        drawMinOnesSegmentOne(color);
        drawMinOnesSegmentTwo(color);
        drawMinOnesSegmentFour(color);
        drawMinOnesSegmentFive(color);
        drawMinOnesSegmentSix(color);
        drawMinOnesSegmentSeven(color);
        break;
    case 7:
        drawMinOnesSegmentOne(color);
        drawMinOnesSegmentThree(color);
        drawMinOnesSegmentSix(color);
        break;
    case 8:
        drawMinOnesSegmentOne(color);
        drawMinOnesSegmentTwo(color);
        drawMinOnesSegmentThree(color);
        drawMinOnesSegmentFive(color);
        drawMinOnesSegmentSix(color);
        drawMinOnesSegmentSeven(color);
        break;
    case 9:
        drawMinOnesSegmentOne(color);
        drawMinOnesSegmentTwo(color);
        drawMinOnesSegmentThree(color);
        drawMinOnesSegmentFour(color);
        drawMinOnesSegmentSix(color);
    default:
        break;
    }
}

void handleSegmentsMinTens(int num, uint32_t color)
{
    switch (num)
    {
    case 0:
        drawMinTensSegmentOne(color);
        drawMinTensSegmentTwo(color);
        drawMinTensSegmentThree(color);
        drawMinTensSegmentFive(color);
        drawMinTensSegmentSix(color);
        drawMinTensSegmentSeven(color);
        break;
    case 1:
        drawMinTensOne(color);
        break;
    case 2:
        drawMinTensSegmentOne(color);
        drawMinTensSegmentThree(color);
        drawMinTensSegmentFour(color);
        drawMinTensSegmentFive(color);
        drawMinTensSegmentSeven(color);
        break;
    case 3:
        drawMinTensSegmentOne(color);
        drawMinTensSegmentThree(color);
        drawMinTensSegmentFour(color);
        drawMinTensSegmentSix(color);
        drawMinTensSegmentSeven(color);
        break;
    case 4:
        drawMinTensSegmentOne(color);
        drawMinTensSegmentFour(color);
        drawMinTensSegmentThree(color);
        drawMinTensSegmentSix(color);
        break;
    case 5:
        drawMinTensSegmentOne(color);
        drawMinTensSegmentTwo(color);
        drawMinTensSegmentFour(color);
        drawMinTensSegmentSix(color);
        drawMinTensSegmentSeven(color);
        break;
    case 6:
        drawMinTensSegmentOne(color);
        drawMinTensSegmentTwo(color);
        drawMinTensSegmentFour(color);
        drawMinTensSegmentFive(color);
        drawMinTensSegmentSix(color);
        drawMinTensSegmentSeven(color);
        break;
    case 7:
        drawMinTensSegmentOne(color);
        drawMinTensSegmentThree(color);
        drawMinTensSegmentSix(color);
        break;
    case 8:
        drawMinTensSegmentOne(color);
        drawMinTensSegmentTwo(color);
        drawMinTensSegmentThree(color);
        drawMinTensSegmentFive(color);
        drawMinTensSegmentSix(color);
        drawMinTensSegmentSeven(color);
        break;
    case 9:
        drawMinTensSegmentOne(color);
        drawMinTensSegmentTwo(color);
        drawMinTensSegmentThree(color);
        drawMinTensSegmentFour(color);
        drawMinTensSegmentSix(color);
    default:
        break;
    }
}

void handleSegmentsHourOnes(int num, uint32_t color)
{
    switch (num)
    {
    case 0:
        drawHourOnesSegmentOne(color);
        drawHourOnesSegmentTwo(color);
        drawHourOnesSegmentThree(color);
        drawHourOnesSegmentFive(color);
        drawHourOnesSegmentSix(color);
        drawHourOnesSegmentSeven(color);
        break;
    case 1:
        drawHourOnesOne(color);
        break;
    case 2:
        drawHourOnesSegmentOne(color);
        drawHourOnesSegmentThree(color);
        drawHourOnesSegmentFour(color);
        drawHourOnesSegmentFive(color);
        drawHourOnesSegmentSeven(color);
        break;
    case 3:
        drawHourOnesSegmentOne(color);
        drawHourOnesSegmentThree(color);
        drawHourOnesSegmentFour(color);
        drawHourOnesSegmentSix(color);
        drawHourOnesSegmentSeven(color);
        break;
    case 4:
        drawHourOnesSegmentOne(color);
        drawHourOnesSegmentFour(color);
        drawHourOnesSegmentThree(color);
        drawHourOnesSegmentSix(color);
        break;
    case 5:
        drawHourOnesSegmentOne(color);
        drawHourOnesSegmentTwo(color);
        drawHourOnesSegmentFour(color);
        drawHourOnesSegmentSix(color);
        drawHourOnesSegmentSeven(color);
        break;
    case 6:
        drawHourOnesSegmentOne(color);
        drawHourOnesSegmentTwo(color);
        drawHourOnesSegmentFour(color);
        drawHourOnesSegmentFive(color);
        drawHourOnesSegmentSix(color);
        drawHourOnesSegmentSeven(color);
        break;
    case 7:
        drawHourOnesSegmentOne(color);
        drawHourOnesSegmentThree(color);
        drawHourOnesSegmentSix(color);
        break;
    case 8:
        drawHourOnesSegmentOne(color);
        drawHourOnesSegmentTwo(color);
        drawHourOnesSegmentThree(color);
        drawHourOnesSegmentFive(color);
        drawHourOnesSegmentSix(color);
        drawHourOnesSegmentSeven(color);
        break;
    case 9:
        drawHourOnesSegmentOne(color);
        drawHourOnesSegmentTwo(color);
        drawHourOnesSegmentThree(color);
        drawHourOnesSegmentFour(color);
        drawHourOnesSegmentSix(color);
    default:
        break;
    }
}

void drawDigitalClock(uint32_t color) // Emulates a seven segment clock to display the time
{
    //the easiest way to draw the clock is to break the time into individual numbers and draw them 1 by 1;
    DateTime now = rtc.now();
    int min = now.minute();
    int hour = now.hour();

    int minOnes = min % 10;
    int hourOnes = hour % 10;

    int minTens;

    //Unsure if there is a better way to do this but this works for now
    if (min >= 0 && min < 10)
        minTens = 0;
    if (min >= 10 && min < 20)
        minTens = 1;
    else if (min >= 20 && min < 30)
        minTens = 2;
    else if (min >= 30 && min < 40)
        minTens = 3;
    else if (min >= 40 && min < 50)
        minTens = 4;
    else if (min >= 50 && min < 60)
        minTens = 5;

    if (hour >= 10) // Checks to see if we need to draw the 1 for the hour's tens place
        drawHourTensOne(color);

    handleSegmentsHourOnes(hourOnes, color);
    handleSegmentsMinTens(minTens, color);
    handleSegmentsMinOnes(minOnes, color);
}

#endif // SEVEN_SEGMENT_H
