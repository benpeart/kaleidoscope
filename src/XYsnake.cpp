// https://wokwi.com/arduino/projects/286181658485326348
#include "main.h"
#include "render.h"
#include "XYsnake.h"
#include "Snake.h"

// Initialize the snake field with WIDTH and HEIGHT with delay=10 ticks
Snake snakeGame(WIDTH, HEIGHT, 10);

unsigned long timerTone = 0;
unsigned long timerReset = 0;
unsigned long timerRefresh = 0;
unsigned long timerMoveTone = 0;
unsigned long timerSnakeRose = 0;

bool isMoveTone = false;
bool isGameReset = false;
bool isMoveToneStop = false;
bool isSnakeRose = false;
bool isSnakeRoseEnd = false;

int lastSnakeLenght = 0;

uint8_t direction = 0;
uint8_t lastDirection = 0;

void inputEvent()
{
#ifdef ENCODER
    // use the left knob to change directions left/right
    static int lastLeftKnob = 0;
    int knob = knobLeft.getCount();
    if (knob != lastLeftKnob)
    {
        // if the knob was twisted left
        if (knob > lastLeftKnob)
        {
            switch (snakeGame.getSnakeDirection())
            {
            case LEFT:
                snakeGame.goDown();
                break;
            case RIGHT:
                snakeGame.goUp();
                break;
            case UP:
                snakeGame.goLeft();
                break;
            case DOWN:
                snakeGame.goRight();
                break;
            }
        }
        else
        {
            switch (snakeGame.getSnakeDirection())
            {
            case LEFT:
                snakeGame.goUp();
                break;
            case RIGHT:
                snakeGame.goDown();
                break;
            case UP:
                snakeGame.goRight();
                break;
            case DOWN:
                snakeGame.goLeft();
                break;
            }
        }
        lastLeftKnob = knob;
    }
#endif

    // Checks whether the snake has changed position
    if (isMoveTone && (direction != lastDirection))
    {
        isMoveTone = false;
        isMoveToneStop = true;
        lastDirection = direction;
        timerMoveTone = millis();
    }

    if ((isMoveToneStop) && ((millis() - timerMoveTone) >= 30))
    {
        isMoveToneStop = false;
    }
}

void setPixel(byte x, byte y, byte r, byte g, byte b)
{
    // invert the Y-Axis as Snake assumes x=0, y=0 is at the bottom left, but my library has it at top left
    leds[XY(x, HEIGHT - 1 - y)].setRGB(r, g, b);
    leds_dirty = true;
}

void changeRGBtoGBR()
{
    for (int whiteLed = 0; whiteLed < NUM_STRIPS * NUM_LEDS_PER_STRIP; whiteLed++)
    {
        leds[whiteLed].setRGB(leds[whiteLed].g, leds[whiteLed].b, leds[whiteLed].r);
    }
    leds_dirty = true;
}

void clearScreen()
{
    for (int whiteLed = 0; whiteLed < NUM_STRIPS * NUM_LEDS_PER_STRIP; whiteLed++)
    {
        leds[whiteLed].setRGB(3, 3, 3);
    }
    leds_dirty = true;
}

void mode_xy_snake()
{
    // do a one time init
    static bool init = false;
    if (!init)
    {
        init = true;
        snakeGame.setBodyColor(255, 0, 255); // Optionally set the color of the snakeparts
        snakeGame.setFoodColor(0, 60, 125);  // Optionally set the color of the food
        snakeGame.setHeadColor(225, 20, 60); // Optionally set the color of the snakeparts
        lastSnakeLenght = snakeGame.getSnakeLenght();
    }

    inputEvent();

    if (!snakeGame.wasGameReset() && (millis() - timerRefresh > 30))
    {
        timerRefresh = millis();

        // This needs to be updated every frame
        Snake::pixel *snakeLimbs = snakeGame.getSnakeLimbs();
        Snake::pixel *snakeFood = snakeGame.getFoodPositions();

        clearScreen();

        setPixel(snakeFood[0].posX, snakeFood[0].posY, snakeFood[0].pixelColor.r, snakeFood[0].pixelColor.g, snakeFood[0].pixelColor.b); // display the food

        for (int i = 0; i < snakeGame.getSnakeLenght(); i++)
        {
            setPixel(snakeLimbs[i].posX, snakeLimbs[i].posY, snakeLimbs[i].pixelColor.r, snakeLimbs[i].pixelColor.g, snakeLimbs[i].pixelColor.b);
        }

        if (lastSnakeLenght != snakeGame.getSnakeLenght())
        {
            lastSnakeLenght = snakeGame.getSnakeLenght();
            isSnakeRose = true;
            timerSnakeRose = millis();
        }

        //FastLED.show();
        snakeGame.tick(); // Main loop for the snake library
    }

    if ((isSnakeRose) && (millis() - timerSnakeRose) >= 30)
    {
        isSnakeRose = false;
        isSnakeRoseEnd = true;
        timerSnakeRose = millis();
    }

    if ((isSnakeRoseEnd) && (millis() - timerSnakeRose) >= 30)
    {
        isSnakeRoseEnd = false;
    }

    // If the snake bit itself or the wall, flash a little
    if (snakeGame.wasGameReset())
    {
        changeRGBtoGBR();
        //FastLED.show();
        isGameReset = true;
        timerReset = millis();
    }

    if ((isGameReset) && (millis() - timerReset) >= 200)
    {
        isGameReset = false;
    }
}