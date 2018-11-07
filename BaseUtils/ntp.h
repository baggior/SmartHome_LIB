#ifndef ntp_h
#define ntp_h

#include <WiFiUdp.h>

/** WiFiUDP class for NTP server */
extern WiFiUDP ntpUDP;

bool initNTP();
bool tryGetTime();
String digitalClockDisplay();
String digitalTimeDisplay();
String digitalTimeDisplaySec();
String digitalDateDisplay();

#endif