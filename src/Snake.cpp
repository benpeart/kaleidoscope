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
#include "Snake.h"
#include "render.h"

Snake::Snake(int fieldSizeX, int fieldSizeY, int moveDelay)
{
    randomSeed(analogRead(0));

    _fieldSizeX = fieldSizeX;
    _fieldSizeY = fieldSizeY;

    food = new pixel[1];

    _snakeLength = 1;

    snakeLimbs = new pixel[_fieldSizeX * _fieldSizeY];
    snakeLimbs[0].posX = _fieldSizeX / 2;
    snakeLimbs[0].posY = _fieldSizeY / 2;

    _bodyColor.set(0, 255, 0);
    _headColor.set(0, 240, 60);
    _foodColor.set(255, 0, 0);

    setColor(_bodyColor, 1);
    setColor(_headColor, 2);
    setColor(_foodColor, 3);

    relocateFood();

    _direction = RIGHT;
    _wasReset = false;
    _moveDelay = moveDelay;
    _snakeTimer = _moveDelay;
}

bool Snake::checkMove(pixel newPos)
{
    if (isPointOutOfBounds(newPos))
        return false;
    if (isPointOnSnake(newPos))
        return false;
    if (newPos.posX < 0 || newPos.posX >= _fieldSizeX)
        return false;
    if (newPos.posY < 0 || newPos.posY >= _fieldSizeY)
        return false;

    return true;
}

void Snake::resetGame()
{
    _snakeLength = 1;
    snakeLimbs[0].posX = _fieldSizeX / 2;
    snakeLimbs[0].posY = _fieldSizeY / 2;
    _wasReset = true;
}

void Snake::makeMove(byte direction)
{
    for (int i = _snakeLength - 1; i >= 0; i--)
    {
        if (i == 0)
        {

            pixel newPos = snakeLimbs[0];
            switch (direction)
            {
            case LEFT:
                newPos.posX--;
                break;
            case RIGHT:
                newPos.posX++;
                break;
            case UP:
                newPos.posY++;
                break;
            case DOWN:
                newPos.posY--;
                break;
            }

            if (checkMove(newPos))
            {
                snakeLimbs[0] = newPos;
            }
            else
                resetGame();
        }
        else
        {
            snakeLimbs[i].posX = snakeLimbs[i - 1].posX;
            snakeLimbs[i].posY = snakeLimbs[i - 1].posY;
        }
    }
}

void Snake::tick()
{
    _snakeTimer--;
    if (_snakeTimer == 0)
    {
        makeMove(_direction);
        checkFood();
        _snakeTimer = _moveDelay;
    }
}

void Snake::goLeft()
{
    _direction = LEFT;
}

void Snake::goRight()
{
    _direction = RIGHT;
}

void Snake::goUp()
{
    _direction = UP;
}

void Snake::goDown()
{
    _direction = DOWN;
}

byte Snake::getSnakeDirection()
{
    return _direction;
}

int Snake::getSnakeLenght()
{
    return _snakeLength;
}

Snake::pixel *Snake::getSnakeLimbs()
{
    return snakeLimbs;
}

void Snake::increaseSize()
{
    if (_snakeLength < _fieldSizeX * _fieldSizeY)
    {
        _snakeLength++;
        snakeLimbs[_snakeLength - 1].posX = snakeLimbs[_snakeLength - 2].posX;
        snakeLimbs[_snakeLength - 1].posY = snakeLimbs[_snakeLength - 2].posY;
        snakeLimbs[_snakeLength - 1].pixelColor = _bodyColor;
    }
}

bool Snake::wasGameReset()
{
    if (_wasReset)
    {
        _wasReset = false;
        return true;
    }
    else
    {
        return false;
    }
}

void Snake::checkFood()
{
    if (snakeLimbs[0].posX == food[0].posX && snakeLimbs[0].posY == food[0].posY)
    {
        increaseSize();
        relocateFood();
    }
}

Snake::pixel *Snake::getFoodPositions()
{
    return food;
}

void Snake::relocateFood()
{
    do
    {
        food[0].posX = random(0, _fieldSizeX - 10) + 5;
        food[0].posY = random(0, _fieldSizeY - 10) + 5;
    } while (isPointOnSnake(food[0]) || isPointOutOfBounds(food[0]));
}

bool Snake::isPointOutOfBounds(pixel pointPos)
{
    return XY(pointPos.posX, pointPos.posY) == OUTOFBOUNDS;
}

bool Snake::isPointOnSnake(pixel pointPos)
{
    for (int i = 0; i < _snakeLength; i++)
    {
        if (pointPos.posX == snakeLimbs[i].posX && pointPos.posY == snakeLimbs[i].posY)
        {
            return true;
        }
    }
    return false;
}

void Snake::setColor(colorRGB color, byte destination)
{
    switch (destination)
    {
    case 1:
        _bodyColor = color;
        for (int i = 1; i < _snakeLength; i++)
        {
            snakeLimbs[i].pixelColor = _bodyColor;
        }
        break;
    case 2:
        _headColor = color;
        snakeLimbs[0].pixelColor = _headColor;
        break;
    case 3:
        _foodColor = color;
        food[0].pixelColor = _foodColor;
        break;
    }
}

void Snake::setDelay(int moveDelay)
{
    _moveDelay = moveDelay;
}
