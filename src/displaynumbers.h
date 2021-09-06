#ifndef DISPLAY_NUMBERS_H
#define DISPLAY_NUMBERS_H

typedef CRGB (*getColor)(CRGB);
void displayNumbers(int n1, int n2, int n3, int n4, getColor color);

#endif
