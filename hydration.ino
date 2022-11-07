#include "detection.h"
#include "leds.h"

#define PIN_NEO 5
#define NUM_PIXELS 24

#define PIN_IR 4
#define PERSON_UNLATCH_MS (60 * 60 * 1000) // Go idle after an hour.

#define PIN_SWITCH 0
#define CUP_AVALIABLE_MS (5 * 60 * 1000) // Cup has been missing for one minutes.
#define CUP_PRESENT_MS (100)             // Bascially a debounce.

#define DRINK_INTERVAL_MS (10 * 60 * 1000) // Drink every 10 minutes.

Detection cupAvailable = Detection(PIN_SWITCH, CUP_AVALIABLE_MS, LOW);
Detection cupPresent = Detection(PIN_SWITCH, CUP_PRESENT_MS, LOW);
Detection person = Detection(PIN_IR, PERSON_UNLATCH_MS, HIGH);

Leds display = Leds(PIN_NEO, NUM_PIXELS);

float countdown = 1.0;

uint32_t countdownStartMs;
bool active;

enum class State
{
  Idle,
  Countdown,
  DrinkIdle,
  DrinkActive,
  NoCup,
};

State state;

Pattern stateToPattern(State input);

void setup()
{
  Serial.begin(115200);
  pinMode(PIN_IR, INPUT);
  pinMode(PIN_SWITCH, INPUT_PULLUP);
  countdownStartMs = millis();
  active = false;
  state = State::Idle;
}

void loop()
{

  bool isCupAvailable = cupAvailable.update();
  bool isCupPresent = cupPresent.update();
  bool isPersonPresent = person.update();

  if (!isPersonPresent)
  {
    state = State::Idle; // All states should go idle if the room is vacant.
  }

  switch (state)
  {
  case State::Idle:
  {
    if (isPersonPresent)
    {
      state = State::Countdown;
      countdownStartMs = millis();
    }
    break;
  }
  case State::Countdown:
  {
    if (isCupAvailable == false)
    {
      state = State::NoCup;
    }
    else
    {
      const uint32_t deltaMs = millis() - countdownStartMs;
      if (deltaMs > DRINK_INTERVAL_MS)
      {
        state = State::DrinkIdle;
        // Countdown restarts immediately.
        while ((millis() - countdownStartMs) > DRINK_INTERVAL_MS)
        {
          countdownStartMs += DRINK_INTERVAL_MS;
        }
      }
    }
    break;
  }
  case State::DrinkIdle:
  {
    if (isCupPresent == false)
    {
      state = State::DrinkActive;
    }
    break;
  }
  case State::DrinkActive:
  {
    if (isCupPresent)
    {
      state = State::Countdown;
    }
    break;
  }
  case State::NoCup:
  {
    if (isCupAvailable)
    {
      state = State::Countdown;
    }
    break;
  }
  }
  const uint32_t deltaMs = millis() - countdownStartMs;
  float countdown = ((float)deltaMs / (float)DRINK_INTERVAL_MS);

  Serial.print(digitalRead(PIN_IR));
  Serial.print("\t");
  Serial.print(digitalRead(PIN_SWITCH));
  Serial.print("\t");
  Serial.print(countdown);
  Serial.print("\t");
  Serial.print((int)stateToPattern(state));
  Serial.println("");

  display.setPattern(stateToPattern(state));
  display.update(countdown);
  delay(1);
}

Pattern stateToPattern(State input)
{
  switch (input)
  {
  case State::Idle:
  {
    return Pattern::Idle;
  }
  case State::Countdown:
  {
    return Pattern::Countdown;
  }
  case State::DrinkActive:
  case State::DrinkIdle:
  {
    return Pattern::Drink;
  }
  case State::NoCup:
  {
    return Pattern::NoCup;
  }
  }
  return Pattern::Idle;
}
