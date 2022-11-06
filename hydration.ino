#include <Adafruit_NeoPixel.h>


#define PIN_NEO   5
#define NUMPIXELS 24

#define PIN_IR 4

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEO, NEO_GRB + NEO_KHZ800);

#define ANALOG_READ_RESOLUTION (10)
#define ADC_3V3 (1 << ANALOG_READ_RESOLUTION)

#define IR_THRESHOLD (ADC_3V3/2) // This is needed because the IR sensor is digital, but the output voltage isn't always 3V3 levels.



void setup() {
  Serial.begin(115200);
  pinMode(PIN_IR, INPUT);
  analogReadResolution(ANALOG_READ_RESOLUTION);
  pixels.begin();
}

void loop() {
  pixels.clear();
  if (isPresent() == true) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(100, 0, 0));
    }
  } else {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 100, 100));
    }    
  }
}

bool isPresent() {
  uint16_t reading = analogRead(PIN_IR); // Actually digital, but the IR sensor doesn't always output 3.3V
  return (reading > IR_THRESHOLD);
}
