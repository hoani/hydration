#include <arduino.h>

#include "detection.h"

Detection::Detection(int pin, uint32_t unlatchMs, int active) : _pin(pin),
                                                                _unlatchMs(unlatchMs),
                                                                _state(false),
                                                                _active(active) {}

bool Detection::update()
{
    const bool sample = (digitalRead(_pin) == _active);
    if (_state)
    {
        if (sample)
        {
            _lastMs = millis();
        }
        else
        {
            const uint32_t deltaMs = millis() - _lastMs;
            if (deltaMs > _unlatchMs)
            {
                _state = false;
            }
        }
    }
    else
    {
        if (sample)
        {
            _lastMs = millis();
            _state = true;
        }
    }
    return _state;
}