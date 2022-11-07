#ifndef _HYD_LEDS_H_
#define _HYD_LEDS_H_

#include <stdint.h>
#include <Adafruit_NeoPixel.h>

enum class Pattern
{
    Idle,
    NoCup,
    Countdown,
    Drink,
};

class Leds
{
public:
    Leds(int pin, int numLeds);
    void setPattern(Pattern pattern);
    void update(float fraction);

private:
    int _numLeds;
    Adafruit_NeoPixel _pixels;
    Pattern _pattern;

    void _doIdle();
    void _doNoCup();
    void _doCountdown(float fraction);
    void _doDrink();
};

#endif