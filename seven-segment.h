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
    LEDs.strip[0].setPixelColor(18, color);
    LEDs.strip[0].setPixelColor(19, color);
    LEDs.strip[0].setPixelColor(20, color);
    LEDs.strip[0].setPixelColor(21, color);
    LEDs.strip[0].setPixelColor(22, color);
    LEDs.strip[0].setPixelColor(23, color);
    LEDs.strip[0].setPixelColor(24, color);
    LEDs.strip[0].setPixelColor(25, color);
    LEDs.strip[0].setPixelColor(26, color);
    LEDs.strip[0].setPixelColor(27, color);
}

////////////////////////////////////////////

//HourOnes
void drawHourOnesOne(uint32_t color)
{
    LEDs.strip[1].setPixelColor(88, color);
    LEDs.strip[1].setPixelColor(87, color);
    LEDs.strip[1].setPixelColor(86, color);
    LEDs.strip[1].setPixelColor(85, color);
    LEDs.strip[1].setPixelColor(84, color);
    LEDs.strip[1].setPixelColor(83, color);
    LEDs.strip[1].setPixelColor(82, color);
    LEDs.strip[1].setPixelColor(81, color);
    LEDs.strip[1].setPixelColor(80, color);
    LEDs.strip[1].setPixelColor(79, color);
}

void drawHourOnesSegmentOne(uint32_t color)
{
    LEDs.strip[0].setPixelColor(57, color);
    LEDs.strip[0].setPixelColor(87, color);
    LEDs.strip[0].setPixelColor(88, color);
    LEDs.strip[0].setPixelColor(102, color);
}

void drawHourOnesSegmentTwo(uint32_t color)
{
    LEDs.strip[0].setPixelColor(58, color);
    LEDs.strip[0].setPixelColor(59, color);
    LEDs.strip[0].setPixelColor(60, color);
    LEDs.strip[0].setPixelColor(61, color);
}

void drawHourOnesSegmentThree(uint32_t color)
{
    LEDs.strip[0].setPixelColor(103, color);
    LEDs.strip[0].setPixelColor(104, color);
    LEDs.strip[0].setPixelColor(105, color);
    LEDs.strip[0].setPixelColor(106, color);
}

void drawHourOnesSegmentFour(uint32_t color)
{
    LEDs.strip[0].setPixelColor(84, color);
    LEDs.strip[0].setPixelColor(85, color);
}

void drawHourOnesSegmentFive(uint32_t color)
{
    LEDs.strip[0].setPixelColor(62, color);
    LEDs.strip[0].setPixelColor(63, color);
    LEDs.strip[0].setPixelColor(64, color);
    LEDs.strip[0].setPixelColor(65, color);
}

void drawHourOnesSegmentSix(uint32_t color)
{
    LEDs.strip[0].setPixelColor(107, color);
    LEDs.strip[0].setPixelColor(108, color);
    LEDs.strip[0].setPixelColor(109, color);
}

void drawHourOnesSegmentSeven(uint32_t color)
{
    LEDs.strip[0].setPixelColor(79, color);
    LEDs.strip[0].setPixelColor(80, color);
    LEDs.strip[0].setPixelColor(110, color);
}
////////////////////////////////////// HourOnes^

//MinTens
void drawMinTensOne(uint32_t color)
{
    LEDs.strip[1].setPixelColor(55, color);
    LEDs.strip[1].setPixelColor(56, color);
    LEDs.strip[1].setPixelColor(57, color);
    LEDs.strip[1].setPixelColor(58, color);
    LEDs.strip[1].setPixelColor(59, color);
    LEDs.strip[1].setPixelColor(60, color);
    LEDs.strip[1].setPixelColor(61, color);
    LEDs.strip[1].setPixelColor(62, color);
    LEDs.strip[1].setPixelColor(63, color);
    LEDs.strip[1].setPixelColor(64, color);
}

void drawMinTensSegmentOne(uint32_t color)
{
    LEDs.strip[1].setPixelColor(41, color);
    LEDs.strip[1].setPixelColor(55, color);
    LEDs.strip[1].setPixelColor(56, color);
    LEDs.strip[1].setPixelColor(86, color);
}

void drawMinTensSegmentTwo(uint32_t color)
{
    LEDs.strip[1].setPixelColor(40, color);
    LEDs.strip[1].setPixelColor(39, color);
    LEDs.strip[1].setPixelColor(38, color);
    LEDs.strip[1].setPixelColor(37, color);
}

void drawMinTensSegmentThree(uint32_t color)
{
    LEDs.strip[1].setPixelColor(85, color);
    LEDs.strip[1].setPixelColor(84, color);
    LEDs.strip[1].setPixelColor(83, color);
    LEDs.strip[1].setPixelColor(82, color);
}

void drawMinTensSegmentFour(uint32_t color)
{
    LEDs.strip[1].setPixelColor(59, color);
    LEDs.strip[1].setPixelColor(60, color);
}

void drawMinTensSegmentFive(uint32_t color)
{
    LEDs.strip[1].setPixelColor(36, color);
    LEDs.strip[1].setPixelColor(35, color);
    LEDs.strip[1].setPixelColor(34, color);
    LEDs.strip[1].setPixelColor(33, color);
}

void drawMinTensSegmentSix(uint32_t color)
{
    LEDs.strip[1].setPixelColor(81, color);
    LEDs.strip[1].setPixelColor(80, color);
    LEDs.strip[1].setPixelColor(79, color);
}

void drawMinTensSegmentSeven(uint32_t color)
{
    LEDs.strip[1].setPixelColor(63, color);
    LEDs.strip[1].setPixelColor(74, color);
    LEDs.strip[1].setPixelColor(78, color);
}
////////////////////////////////////// MinTens^

//MinOnes
void drawMinOnesOne(uint32_t color)
{
    LEDs.strip[1].setPixelColor(125, color);
    LEDs.strip[1].setPixelColor(124, color);
    LEDs.strip[1].setPixelColor(123, color);
    LEDs.strip[1].setPixelColor(122, color);
    LEDs.strip[1].setPixelColor(121, color);
    LEDs.strip[1].setPixelColor(120, color);
    LEDs.strip[1].setPixelColor(119, color);
    LEDs.strip[1].setPixelColor(118, color);
    LEDs.strip[1].setPixelColor(117, color);
    LEDs.strip[1].setPixelColor(116, color);
}

void drawMinOnesSegmentOne(uint32_t color)
{

    LEDs.strip[1].setPixelColor(98, color);
    LEDs.strip[1].setPixelColor(124, color);
    LEDs.strip[1].setPixelColor(125, color);
    LEDs.strip[1].setPixelColor(131, color);
}

void drawMinOnesSegmentTwo(uint32_t color)
{
    LEDs.strip[1].setPixelColor(99, color);
    LEDs.strip[1].setPixelColor(100, color);
    LEDs.strip[1].setPixelColor(101, color);
    LEDs.strip[1].setPixelColor(102, color);
}

void drawMinOnesSegmentThree(uint32_t color)
{
    LEDs.strip[1].setPixelColor(132, color);
    LEDs.strip[1].setPixelColor(133, color);
    LEDs.strip[1].setPixelColor(134, color);
    LEDs.strip[1].setPixelColor(135, color);
}

void drawMinOnesSegmentFour(uint32_t color)
{
    LEDs.strip[1].setPixelColor(120, color);
    LEDs.strip[1].setPixelColor(121, color);
}

void drawMinOnesSegmentFive(uint32_t color)
{
    LEDs.strip[1].setPixelColor(103, color);
    LEDs.strip[1].setPixelColor(104, color);
    LEDs.strip[1].setPixelColor(105, color);
    LEDs.strip[1].setPixelColor(106, color);
}

void drawMinOnesSegmentSix(uint32_t color)
{
    LEDs.strip[1].setPixelColor(136, color);
    LEDs.strip[1].setPixelColor(137, color);
    LEDs.strip[1].setPixelColor(138, color);
}

void drawMinOnesSegmentSeven(uint32_t color)
{
    LEDs.strip[1].setPixelColor(116, color);
    LEDs.strip[1].setPixelColor(117, color);
    LEDs.strip[1].setPixelColor(139, color);
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
#endif // SEVEN_SEGMENT_H
