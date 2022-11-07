#ifndef _HYD_DETECTION_H_
#define _HYD_DETECTION_H_

#include <stdint.h>

// Detection provides a latching detection with a timed hysteresis.
// When a pin is sensed as high, detection triggers true and stays latched.
// When a timeout expires with the pin remaining low, then the latch is released and the detection returns false.
class Detection
{
public:
    Detection(int pin, uint32_t unlatchMs, int active);
    bool update();

private:
    int _pin;
    bool _state;
    uint32_t _unlatchMs;
    uint32_t _lastMs;
    int _active;
};

#endif