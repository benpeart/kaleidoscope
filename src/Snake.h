/*
	Yet Another Snake Game Library.
	Copyright (C) 2014  Emanuel Knöpfel

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef Snake_h
#define Snake_h

#include "Arduino.h"

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

class Snake
{
public:
    struct colorRGB
    {
        byte r;
        byte g;
        byte b;
        void set(byte R, byte G, byte B)
        {
            r = R;
            g = G;
            b = B;
        }
    };
    struct pixel
    {
        int posX;
        int posY;
        colorRGB pixelColor;
    };

protected:
    byte _direction;
    int _fieldSizeX;
    int _fieldSizeY;
    int _snakeLength;
    int _wasReset;
    colorRGB _headColor;
    colorRGB _bodyColor;
    colorRGB _foodColor;

    unsigned int _snakeTimer;
    unsigned int _moveDelay;
    void checkFood();
    void relocateFood();
    bool isPointOutOfBounds(pixel pointPos);
    bool isPointOnSnake(pixel pointPos);
    bool checkMove(pixel newPos);
    void makeMove(byte direction);
    void setColor(colorRGB color, byte destination);

public:
    pixel *snakeLimbs;
    pixel *food;
    pixel *getSnakeLimbs();
    pixel *getFoodPositions();

    Snake(int fieldSizeX, int fieldSizeY, int moveDelay);

    void goLeft();
    void goRight();
    void goUp();
    void goDown();
    void tick();

    void resetGame();
    bool wasGameReset();
    int getSnakeLenght();
    void increaseSize();
    void setDelay(int moveDelay);
    void setBodyColor(byte r, byte g, byte b)
    {
        colorRGB color = {r, g, b};
        setColor(color, 1);
    };
    void setHeadColor(byte r, byte g, byte b)
    {
        colorRGB color = {r, g, b};
        setColor(color, 2);
    };
    void setFoodColor(byte r, byte g, byte b)
    {
        colorRGB color = {r, g, b};
        setColor(color, 3);
    };
};
#endif
