#pragma once

#include "baseutils.h"

#include <Ticker.h>

// #ifdef ESP8266
//   #include <Ticker.h>
// #else
// #ifdef ESP32
//   #include <ESP32Ticker.h>
// #endif
// #endif


class Blinker {
 public:

 
  Blinker(uint8_t pin);
  void start(float blinkPace);
  void stop();

  baseutils::LED_t led;
  
 private:
  Ticker _ticker;
  float _lastBlinkPace;

  static void _tick(uint8_t pin);
};

