#ifdef ESP32

#include <Arduino.h>

#include "hw_timer_esp32.h"

void IRAM_ATTR isrTimer0();
void IRAM_ATTR isrTimer1();
void IRAM_ATTR isrTimer2();
void IRAM_ATTR isrTimer3();

/** Number of available timers */
int availTimers = 4;
/** Flags for timers in use */
bool timerInUse[4] = {false, false, false, false};
/** Pointers to timer structures */
hw_timer_t *attachedTimer[4] = {NULL, NULL, NULL, NULL};
/** ISR locks */
portMUX_TYPE timerMux0 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux1 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux2 = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE timerMux3 = portMUX_INITIALIZER_UNLOCKED;
/** User callback functions */
callback_t usrCallback[4] = {NULL, NULL, NULL, NULL};
/** Pointer to the ISR routine */
callback_t isrPtr[4] = {(callback_t) isrTimer0, (callback_t) isrTimer1, (callback_t) isrTimer2, (callback_t) isrTimer3};
/** Flag if timer was set to repeat */
bool doRepeat[4] = {false, false, false, false};

/**
	startTimerMsec
	Start a hardware timer with trigger time in milliseconds
*/
hw_timer_t *startTimerMsec (uint64_t msTriggerTime, callback_t callback, bool repeat) {
	return startTimer (msTriggerTime*1000, callback, repeat);
}

/**
	startTimerSec
	Start a hardware timer with trigger time in seconds
*/
hw_timer_t *startTimerSec (uint64_t sTriggerTime, callback_t callback, bool repeat) {
	return startTimer (sTriggerTime*1000*1000, callback, repeat);
}

/**
	startTimer
	Start a hardware timer with trigger time in microseconds
*/
hw_timer_t *startTimer (uint64_t usTriggerTime, callback_t callback, bool repeat) {
	// Find free timer
	uint8_t timerToUse;
	for (timerToUse = 0; timerToUse < availTimers; timerToUse++) {
		if (timerInUse[timerToUse] == false) {
			break;
		}
	}
	if (timerToUse == availTimers) {
		// No free timer found!
		return NULL;
	}
  // Use timerNum timer of 4 (counted from zero).
	// Save pointer to user callback function
	usrCallback[timerToUse] = callback;

	// Set flag when timer is repeating
	doRepeat[timerToUse] = true;

  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
  // info).
  attachedTimer[timerToUse] = timerBegin(timerToUse, 80, true);

  // Attach onTimer function to our timer.
	timerAttachInterrupt(attachedTimer[timerToUse], (callback_t) isrPtr[timerToUse], true);

  // Set alarm to call onTimer function every usTriggerTime  microseconds.
  // Repeat the alarm (third parameter)
  timerAlarmWrite(attachedTimer[timerToUse], usTriggerTime, repeat);

  // Start an alarm
  timerAlarmEnable(attachedTimer[timerToUse]);
	timerInUse[timerToUse] = true;
	return attachedTimer[timerToUse];
}

/**
	restartTimer
	Restart a hardware timer
*/
void restartTimer(hw_timer_t *timerToRestart) {
	uint8_t timerIndex;
	for (timerIndex = 0; timerIndex < availTimers; timerIndex++) {
		if (attachedTimer[timerIndex] == timerToRestart) {
			if (doRepeat[timerIndex]) {
				timerStop(timerToRestart);
			}
			timerRestart(timerToRestart);
			break;
		}
	}
}

/**
	stopTimer
	Stop a hardware timer
*/
void stopTimer(hw_timer_t *timerToStop) {
	uint8_t timerIndex;
	for (timerIndex = 0; timerIndex < availTimers; timerIndex++) {
		if (attachedTimer[timerIndex] == timerToStop) {
			timerStop(timerToStop);
			timerEnd(timerToStop);
			attachedTimer[timerIndex] = NULL;
			timerInUse[timerIndex] = false;
			doRepeat[timerIndex] = false;
			break;
		}
	}
}

/**
	stopAllTimer
	Stop a all running timers
*/
void stopAllTimers() {
	uint8_t timerIndex;
	for (timerIndex = 0; timerIndex < availTimers; timerIndex++) {
		if (attachedTimer[timerIndex] != NULL) {
			timerStop(attachedTimer[timerIndex]);
			timerEnd(attachedTimer[timerIndex]);
			attachedTimer[timerIndex] = NULL;
			timerInUse[timerIndex] = false;
			doRepeat[timerIndex] = false;
		}
	}
}

/**
	isrTimer0
	ISR routine called by timer 0
	Will lock the ISR, call the user defined routine and then
	unlock the ISR again.
*/
void IRAM_ATTR isrTimer0() {
	portENTER_CRITICAL_ISR(&timerMux0);
	callback_t thisCallBack = (callback_t) usrCallback[0];
	thisCallBack();
	portEXIT_CRITICAL_ISR(&timerMux0);
}

/**
	isrTimer1
	ISR routine called by timer 1
	Will lock the ISR, call the user defined routine and then
	unlock the ISR again.
*/
void IRAM_ATTR isrTimer1() {
	portENTER_CRITICAL_ISR(&timerMux1);
	callback_t thisCallBack = (callback_t) usrCallback[1];
	thisCallBack();
	portEXIT_CRITICAL_ISR(&timerMux1);
}

/**
	isrTimer2
	ISR routine called by timer 2
	Will lock the ISR, call the user defined routine and then
	unlock the ISR again.
*/
void IRAM_ATTR isrTimer2() {
	portENTER_CRITICAL_ISR(&timerMux2);
	callback_t thisCallBack = (callback_t) usrCallback[2];
	thisCallBack();
	portEXIT_CRITICAL_ISR(&timerMux2);
}

/**
	isrTimer3
	ISR routine called by timer 3
	Will lock the ISR, call the user defined routine and then
	unlock the ISR again.
*/
void IRAM_ATTR isrTimer3() {
	portENTER_CRITICAL_ISR(&timerMux3);
	callback_t thisCallBack = (callback_t) usrCallback[3];
	thisCallBack();
	portEXIT_CRITICAL_ISR(&timerMux3);
}

#endif