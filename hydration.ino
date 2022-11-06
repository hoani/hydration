#include "src/Ultrasonic/Ultrasonic.h"
#include <Adafruit_NeoPixel.h>


#define PIN_NEO   5
#define DELAYVAL 50
#define NUMPIXELS 24


#define PIN_LED 0
#define PIN_IR 4

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEO, NEO_GRB + NEO_KHZ800);



void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_IR, INPUT);

  pixels.begin();
}

void loop() {

}
