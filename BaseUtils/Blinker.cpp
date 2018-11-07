#include <Arduino.h>

#include "Blinker.h"




Blinker::Blinker(uint8_t pin)
: _lastBlinkPace(0), led{ .enabled = false, .pin = pin, .on = 0 }
{
  if(led.pin)
    pinMode(led.pin, OUTPUT);    
}


void Blinker::start(float blinkPace) {
  if(led.pin) {
    if (_lastBlinkPace != blinkPace) {
      _ticker.attach(blinkPace, _tick, led.pin);
      _lastBlinkPace = blinkPace;
    }
  }
}

void Blinker::stop() {
  if(led.pin) {
    if (_lastBlinkPace != 0) {
      _ticker.detach();
      _lastBlinkPace = 0;
      digitalWrite(led.pin, !led.on);
    }
  }
}

void Blinker::_tick(uint8_t pin) {
  if(pin) {
    digitalWrite(pin, !digitalRead(pin));
  }
}
