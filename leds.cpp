#include <arduino.h>

#include "leds.h"

Leds::Leds(int pin, int numLeds) : _pixels(numLeds, pin, NEO_GRB + NEO_KHZ800),
                                   _pattern(Pattern::Idle)
{
}

void Leds::setPattern(Pattern pattern)
{
    _pattern = pattern;
}

void Leds::update(float fraction)
{
    _pixels.clear();
    switch (_pattern) {
      case Pattern::Idle: {
        _doIdle();
        break;
      }
      case Pattern::NoCup: {
        _doNoCup();
        break;
      }
      case Pattern::Countdown: {
        _doCountdown(fraction);
        break;
      }
      case Pattern::Drink: {
        _doDrink();
        break;
      }
      default: {
        _doIdle();
        break;
      }
    }
    _pixels.show();
}

void Leds::_doIdle()
{
  const uint32_t t = millis() % 10000;
  const float theta = (float)t * TWO_PI / 10000.0;
  const float offset = (float)(0x40) * (1.0 + sin(theta))/2.0;
  const int intensity = 0x10 + offset;
  const int hue = (millis()/10) % 0xffff;

  const uint32_t color = _pixels.ColorHSV(hue, 255, intensity);
  _pixels.fill(_pixels.gamma32(color));
}

void Leds::_doNoCup()
{
    const int offset = (millis() / 10);
    for (int i = 0; i < _pixels.numPixels(); i++)
    {
        const int multiplier = 256/_pixels.numPixels();
        const int intensity = (offset + i) % _pixels.numPixels();
        const int hue = (millis() - ((offset * 0x80) + (i * 0x200))) % 0xffff;

        const uint32_t color = _pixels.ColorHSV(hue, 255, intensity * multiplier);
        _pixels.setPixelColor(i, _pixels.gamma32(color));
    }
}

void Leds::_doCountdown(float fraction)
{
  const int targetIndex = (int)((float)_pixels.numPixels() * fraction);
  const int offset = 17;
  for (int i = 0; i < _pixels.numPixels(); i++)
  {
      uint32_t color = 0;
      const int index = (uint)(offset - i + _pixels.numPixels()) % _pixels.numPixels();
      if (index < targetIndex) {
        color = _pixels.Color(0x00, 0x66, 0x00);
      } else if (index > targetIndex) {
        color = _pixels.Color(0x00, 0x00, 0x66);
      } else {
        const float pixelFraction = ((fraction * (float)_pixels.numPixels()) - (float)targetIndex);
        const uint8_t value = (uint8_t)((float)0x66 * pixelFraction);
        color = _pixels.Color(0x00, value, 0x66 - value);
      }

      _pixels.setPixelColor(i, _pixels.gamma32(color));
  }
}

void Leds::_doDrink()
{
  _doNoCup();
}
