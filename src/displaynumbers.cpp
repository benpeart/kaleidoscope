#include "main.h"
#include "Kaleidoscope.h"
#include "displaynumbers.h"

/************************************************************
*        1
*     -------
*  2 |       | 3
*    |       |
*  4 *-------*   '*' is used for making the lines straighter
*    |       |     
*  5 |       | 6
*     -------
*        7
************************************************************/

void draw_top_seg_1(getColor color, int offset)
{
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 130] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 130]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 125] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 125]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 96] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 96]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 90] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 90]);
}

void draw_top_seg_2(getColor color, int offset)
{
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 13] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 13]);
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 12] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 12]);
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 18] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 18]);
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 19] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 19]);
}

void draw_top_seg_3(getColor color, int offset)
{
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 61] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 61]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 62] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 62]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 53] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 53]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 52] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 52]);
}

void draw_top_seg_4(getColor color, int offset)
{
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 132] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 132]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 88] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 88]);
}

void draw_top_seg_5(getColor color, int offset)
{
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 20] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 20]);
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 10] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 10]);
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 21] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 21]);
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 9] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 9]);
}

void draw_top_seg_6(getColor color, int offset)
{
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 51] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 51]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 64] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 64]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 50] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 50]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 65] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 65]);
}

void draw_top_seg_7(getColor color, int offset)
{
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 134] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 134]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 120] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 120]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 101] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 101]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 86] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 86]);
}

void draw_top_left_triangle(getColor color, int offset)
{
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 11] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 11]);
}

void draw_top_right_triangle(getColor color, int offset)
{
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 63] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 63]);
}

void draw_bot_seg_1(getColor color, int offset)
{
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 137] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 137]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 118] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 118]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 103] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 103]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 83] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 83]);
}

void draw_bot_seg_2(getColor color, int offset)
{
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 6] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 6]);
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 25] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 25]);
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 5] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 5]);
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 26] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 26]);
}

void draw_bot_seg_3(getColor color, int offset)
{
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 68] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 68]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 46] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 46]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 69] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 69]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 45] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 45]);
}

void draw_bot_seg_4(getColor color, int offset)
{
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 139] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 139]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 81] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 81]);
}

void draw_bot_seg_5(getColor color, int offset)
{
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 27] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 27]);
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 28] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 28]);
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 3] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 3]);
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 2] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 2]);
}

void draw_bot_seg_6(getColor color, int offset)
{
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 44] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 44]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 43] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 43]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 71] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 71]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 72] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 72]);
}

void draw_bot_seg_7(getColor color, int offset)
{
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 141] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 141]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 113] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 113]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 108] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 108]);
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 79] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 79]);
}

void draw_bot_left_triangle(getColor color, int offset)
{
    leds[(0 + offset) * NUM_LEDS_PER_STRIP + 4] = color(leds[(0 + offset) * NUM_LEDS_PER_STRIP + 4]);
}

void draw_bot_right_triangle(getColor color, int offset)
{
    leds[(1 + offset) * NUM_LEDS_PER_STRIP + 70] = color(leds[(1 + offset) * NUM_LEDS_PER_STRIP + 70]);
}

void drawN1(int num, getColor color)
{
    switch (num)
    {
    case 0:
        draw_top_seg_1(color, 0);
        draw_top_seg_2(color, 0);
        draw_top_seg_3(color, 0);
        draw_top_seg_5(color, 0);
        draw_top_seg_6(color, 0);
        draw_top_seg_7(color, 0);
        draw_top_right_triangle(color, 0);
        draw_top_left_triangle(color, 0);
        break;
    case 1:
        draw_top_seg_3(color, 0);
        draw_top_seg_6(color, 0);
        draw_top_right_triangle(color, 0);
        break;
    case 2:
        draw_top_seg_1(color, 0);
        draw_top_seg_3(color, 0);
        draw_top_seg_4(color, 0);
        draw_top_seg_5(color, 0);
        draw_top_seg_7(color, 0);
        draw_top_right_triangle(color, 0);
        draw_top_left_triangle(color, 0);
        break;
    case 3:
        draw_top_seg_1(color, 0);
        draw_top_seg_3(color, 0);
        draw_top_seg_4(color, 0);
        draw_top_seg_6(color, 0);
        draw_top_seg_7(color, 0);
        draw_top_right_triangle(color, 0);
        break;
    case 4:
        draw_top_seg_2(color, 0);
        draw_top_seg_3(color, 0);
        draw_top_seg_4(color, 0);
        draw_top_seg_6(color, 0);
        draw_top_right_triangle(color, 0);
        draw_top_left_triangle(color, 0);
        break;
    case 5:
        draw_top_seg_1(color, 0);
        draw_top_seg_2(color, 0);
        draw_top_seg_4(color, 0);
        draw_top_seg_6(color, 0);
        draw_top_seg_7(color, 0);
        draw_top_right_triangle(color, 0);
        draw_top_left_triangle(color, 0);
        break;
    case 6:
        draw_top_seg_1(color, 0);
        draw_top_seg_2(color, 0);
        draw_top_seg_4(color, 0);
        draw_top_seg_5(color, 0);
        draw_top_seg_6(color, 0);
        draw_top_seg_7(color, 0);
        draw_top_right_triangle(color, 0);
        draw_top_left_triangle(color, 0);
        break;
    case 7:
        draw_top_seg_1(color, 0);
        draw_top_seg_3(color, 0);
        draw_top_seg_6(color, 0);
        draw_top_right_triangle(color, 0);
        break;
    case 8:
        draw_top_seg_1(color, 0);
        draw_top_seg_2(color, 0);
        draw_top_seg_3(color, 0);
        draw_top_seg_4(color, 0);
        draw_top_seg_5(color, 0);
        draw_top_seg_6(color, 0);
        draw_top_seg_7(color, 0);
        draw_top_right_triangle(color, 0);
        draw_top_left_triangle(color, 0);
        break;
    case 9:
        draw_top_seg_1(color, 0);
        draw_top_seg_2(color, 0);
        draw_top_seg_3(color, 0);
        draw_top_seg_4(color, 0);
        draw_top_seg_6(color, 0);
        draw_top_left_triangle(color, 0);
        draw_top_right_triangle(color, 0);
        break;
    }
}

void drawN2(int num, getColor color)
{
    switch (num)
    {
    case 0:
        draw_top_seg_1(color, 1);
        draw_top_seg_2(color, 3);
        draw_top_seg_3(color, 1);
        draw_top_seg_5(color, 3);
        draw_top_seg_6(color, 1);
        draw_top_seg_7(color, 1);
        draw_top_right_triangle(color, 1);
        draw_top_left_triangle(color, 3);
        break;
    case 1:
        draw_top_seg_2(color, 3);
        draw_top_seg_5(color, 3);
        draw_top_left_triangle(color, 3);
        break;
    case 2:
        draw_top_seg_1(color, 1);
        draw_top_seg_2(color, 3);
        draw_top_seg_4(color, 1);
        draw_top_seg_6(color, 1);
        draw_top_seg_7(color, 1);
        draw_top_right_triangle(color, 1);
        draw_top_left_triangle(color, 3);
        break;
    case 3:
        draw_top_seg_1(color, 1);
        draw_top_seg_2(color, 3);
        draw_top_seg_4(color, 1);
        draw_top_seg_5(color, 3);
        draw_top_seg_7(color, 1);
        draw_top_left_triangle(color, 3);
        break;
    case 4:
        draw_top_seg_2(color, 3);
        draw_top_seg_3(color, 1);
        draw_top_seg_4(color, 1);
        draw_top_seg_5(color, 3);
        draw_top_right_triangle(color, 1);
        draw_top_left_triangle(color, 3);
        break;
    case 5:
        draw_top_seg_1(color, 1);
        draw_top_seg_3(color, 1);
        draw_top_seg_4(color, 1);
        draw_top_seg_5(color, 3);
        draw_top_seg_7(color, 1);
        draw_top_right_triangle(color, 1);
        draw_top_left_triangle(color, 3);
        break;
    case 6:
        draw_top_seg_1(color, 1);
        draw_top_seg_4(color, 1);
        draw_top_seg_3(color, 1);
        draw_top_seg_5(color, 3);
        draw_top_seg_6(color, 1);
        draw_top_seg_7(color, 1);
        draw_top_right_triangle(color, 1);
        draw_top_left_triangle(color, 3);
        break;
    case 7:
        draw_top_seg_1(color, 1);
        draw_top_seg_2(color, 3);
        draw_top_seg_5(color, 3);
        draw_top_left_triangle(color, 3);
        break;
    case 8:
        draw_top_seg_1(color, 1);
        draw_top_seg_2(color, 3);
        draw_top_seg_3(color, 1);
        draw_top_seg_4(color, 1);
        draw_top_seg_5(color, 3);
        draw_top_seg_6(color, 1);
        draw_top_seg_7(color, 1);
        draw_top_right_triangle(color, 1);
        draw_top_left_triangle(color, 3);
        break;
    case 9:
        draw_top_seg_1(color, 1);
        draw_top_seg_2(color, 3);
        draw_top_seg_3(color, 1);
        draw_top_seg_4(color, 1);
        draw_top_seg_5(color, 3);
        draw_top_left_triangle(color, 3);
        draw_top_right_triangle(color, 1);
        break;
    }
}

void drawN3(int num, getColor color)
{
    switch (num)
    {
    case 0:
        draw_bot_seg_1(color, 0);
        draw_bot_seg_2(color, 0);
        draw_bot_seg_3(color, 0);
        draw_bot_seg_5(color, 0);
        draw_bot_seg_6(color, 0);
        draw_bot_seg_7(color, 0);
        draw_bot_right_triangle(color, 0);
        draw_bot_left_triangle(color, 0);
        break;
    case 1:
        draw_bot_seg_3(color, 0);
        draw_bot_seg_6(color, 0);
        draw_bot_right_triangle(color, 0);
        break;
    case 2:
        draw_bot_seg_1(color, 0);
        draw_bot_seg_3(color, 0);
        draw_bot_seg_4(color, 0);
        draw_bot_seg_5(color, 0);
        draw_bot_seg_7(color, 0);
        draw_bot_right_triangle(color, 0);
        draw_bot_left_triangle(color, 0);
        break;
    case 3:
        draw_bot_seg_1(color, 0);
        draw_bot_seg_3(color, 0);
        draw_bot_seg_4(color, 0);
        draw_bot_seg_6(color, 0);
        draw_bot_seg_7(color, 0);
        draw_bot_right_triangle(color, 0);
        break;
    case 4:
        draw_bot_seg_2(color, 0);
        draw_bot_seg_3(color, 0);
        draw_bot_seg_4(color, 0);
        draw_bot_seg_6(color, 0);
        draw_bot_right_triangle(color, 0);
        draw_bot_left_triangle(color, 0);
        break;
    case 5:
        draw_bot_seg_1(color, 0);
        draw_bot_seg_2(color, 0);
        draw_bot_seg_4(color, 0);
        draw_bot_seg_6(color, 0);
        draw_bot_seg_7(color, 0);
        draw_bot_right_triangle(color, 0);
        draw_bot_left_triangle(color, 0);
        break;
    case 6:
        draw_bot_seg_1(color, 0);
        draw_bot_seg_2(color, 0);
        draw_bot_seg_4(color, 0);
        draw_bot_seg_5(color, 0);
        draw_bot_seg_6(color, 0);
        draw_bot_seg_7(color, 0);
        draw_bot_right_triangle(color, 0);
        draw_bot_left_triangle(color, 0);
        break;
    case 7:
        draw_bot_seg_1(color, 0);
        draw_bot_seg_3(color, 0);
        draw_bot_seg_6(color, 0);
        draw_bot_right_triangle(color, 0);
        break;
    case 8:
        draw_bot_seg_1(color, 0);
        draw_bot_seg_2(color, 0);
        draw_bot_seg_3(color, 0);
        draw_bot_seg_4(color, 0);
        draw_bot_seg_5(color, 0);
        draw_bot_seg_6(color, 0);
        draw_bot_seg_7(color, 0);
        draw_bot_right_triangle(color, 0);
        draw_bot_left_triangle(color, 0);
        break;
    case 9:
        draw_bot_seg_1(color, 0);
        draw_bot_seg_2(color, 0);
        draw_bot_seg_3(color, 0);
        draw_bot_seg_4(color, 0);
        draw_bot_seg_6(color, 0);
        draw_bot_left_triangle(color, 0);
        draw_bot_right_triangle(color, 0);
        break;
    }
}

void drawN4(int num, getColor color)
{
    switch (num)
    {
    case 0:
        draw_bot_seg_1(color, 1);
        draw_bot_seg_2(color, 3);
        draw_bot_seg_3(color, 1);
        draw_bot_seg_5(color, 3);
        draw_bot_seg_6(color, 1);
        draw_bot_seg_7(color, 1);
        draw_bot_right_triangle(color, 1);
        draw_bot_left_triangle(color, 3);
        break;
    case 1:
        draw_bot_seg_2(color, 3);
        draw_bot_seg_5(color, 3);
        draw_bot_left_triangle(color, 3);
        break;
    case 2:
        draw_bot_seg_1(color, 1);
        draw_bot_seg_2(color, 3);
        draw_bot_seg_4(color, 1);
        draw_bot_seg_6(color, 1);
        draw_bot_seg_7(color, 1);
        draw_bot_right_triangle(color, 1);
        draw_bot_left_triangle(color, 3);
        break;
    case 3:
        draw_bot_seg_1(color, 1);
        draw_bot_seg_2(color, 3);
        draw_bot_seg_4(color, 1);
        draw_bot_seg_5(color, 3);
        draw_bot_seg_7(color, 1);
        draw_bot_left_triangle(color, 3);
        break;
    case 4:
        draw_bot_seg_2(color, 3);
        draw_bot_seg_3(color, 1);
        draw_bot_seg_4(color, 1);
        draw_bot_seg_5(color, 3);
        draw_bot_right_triangle(color, 1);
        draw_bot_left_triangle(color, 3);
        break;
    case 5:
        draw_bot_seg_1(color, 1);
        draw_bot_seg_3(color, 1);
        draw_bot_seg_4(color, 1);
        draw_bot_seg_5(color, 3);
        draw_bot_seg_7(color, 1);
        draw_bot_right_triangle(color, 1);
        draw_bot_left_triangle(color, 3);
        break;
    case 6:
        draw_bot_seg_1(color, 1);
        draw_bot_seg_3(color, 1);
        draw_bot_seg_4(color, 1);
        draw_bot_seg_5(color, 3);
        draw_bot_seg_6(color, 1);
        draw_bot_seg_7(color, 1);
        draw_bot_right_triangle(color, 1);
        draw_bot_left_triangle(color, 3);
        break;
    case 7:
        draw_bot_seg_1(color, 1);
        draw_bot_seg_2(color, 3);
        draw_bot_seg_5(color, 3);
        draw_bot_left_triangle(color, 3);
        break;
    case 8:
        draw_bot_seg_1(color, 1);
        draw_bot_seg_2(color, 3);
        draw_bot_seg_3(color, 1);
        draw_bot_seg_4(color, 1);
        draw_bot_seg_5(color, 3);
        draw_bot_seg_6(color, 1);
        draw_bot_seg_7(color, 1);
        draw_bot_right_triangle(color, 1);
        draw_bot_left_triangle(color, 3);
        break;
    case 9:
        draw_bot_seg_1(color, 1);
        draw_bot_seg_2(color, 3);
        draw_bot_seg_3(color, 1);
        draw_bot_seg_4(color, 1);
        draw_bot_seg_5(color, 3);
        draw_bot_left_triangle(color, 3);
        draw_bot_right_triangle(color, 1);
        break;
    }
}

void displayNumbers(int n1, int n2, int n3, int n4, getColor color)
{
#ifdef DEBUG
    // do some sanity checking
    if (NULL == color)
    {
        DB_PRINTLN("displayNumbers called with NULL color function pointer");
        return;
    }

    if (n1 > 9 || n2 > 9 || n3 > 9 || n4 > 9)
    {
        DB_PRINTF("\rdisplayNumbers called with number that is out of range (0-9): %d, %d, %d, %d\r\n", n1, n2, n3, n4);
        return;
    }
#endif

    if (n1 >= 0)
        drawN1(n1, color);
    if (n2 >= 0)
        drawN2(n2, color);
    if (n3 >= 0)
        drawN3(n3, color);
    if (n4 >= 0)
        drawN4(n4, color);
}
