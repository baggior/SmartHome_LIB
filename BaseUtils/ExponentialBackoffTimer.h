#pragma once

#include "Timer.h"

class ExponentialBackoffTimer {
 public:
  ExponentialBackoffTimer(uint16_t initialInterval, uint8_t maxBackoff);
  void activate();
  bool check();
  void deactivate();
  bool isActive() const;

 private:
  Timer _timer;

  uint16_t _initialInterval;
  uint8_t _maxBackoff;
  uint8_t _retryCount;
};

