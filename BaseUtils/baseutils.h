////////
// Header for BaseUtils
///////

#ifndef BaseUtils_h
#define BaseUtils_h

#include <Arduino.h>
#include <Ticker.h>

#include "strutils.h"

#include "LinkedList.h"


#if defined (ESP8266) 
#include <FS.h>
#elif defined (ESP32) 
#include <SPIFFS.h>
#endif



#define MAX_FIRMWARE_VERSION_LENGTH     20
#define MAX_FIRMWARE_NAME_LENGTH        50
#define MAX_MAC_STRING_LENGTH           12

namespace baseutils
{

// Utilities
boolean isNum(String str);

int32_t convStrInt(String str);
float convStrFloat(String str);

float roundFloat(float value, uint8_t decimals = 2);

String convFloatStr(float value, uint8_t decimals = 2, boolean comma = false);

String formatNumber(uint32_t number, uint8_t size, char insert='0');
String formatFloat(float value, uint8_t intPlaces=0, uint8_t decPlaces=2, boolean comma=false);

String getFieldStr(String str, uint8_t fieldNum, char delimiter = ':');
char getFieldChar(String str, uint8_t fieldNum, char delimiter = ':');
int32_t getFieldInt(String str, uint8_t fieldNum, char delimiter = ':');
float getFieldFloat(String str, uint8_t fieldNum, char delimiter = ':');

/////////////////////////////////////////////////////////////////////////////
/*
String & leftPadTo(String &str, const uint8_t num, const char paddingChar);
String & rightPadTo(String &str, const uint8_t num, const char paddingChar);
*/
/////////////////////////////////////////////////////////////////////////////
// For Esp

#if defined (ESP8266) || (ESP32) 

String getChipId();

File openFile(const char*  fileName, const char*  mode = "r");

boolean removeFile(const char*  fileName);
String readTextFile(const char*  fileName);

void ESP_restart();
void ESP_clearConfig();
#endif


/////////////////////////////////////////////////////////////////////////////

void abort(const String& message);

uint8_t rssiToPercentage(int32_t rssi);


bool validateMacAddress(const char* mac);

bool validateMd5(const char* md5);

std::unique_ptr<char[]> cloneString(const String& string);

////////////////////////////////////////////////////////////////////////////

// SYSTEM INFO
void printBoardInfo(Stream &stream, uint8_t detail_level=3);

float getVcc() ;   //Measure the power voltage of VDD3P3 pin 3 and 4; unit: V
float readTemp() ; //Temperature is returned in °C.

/////////////////////////////////////////////////////////////////////////////

struct LED_t {
    bool enabled;
    uint8_t pin;
    uint8_t on;     // ON MODE: 0 LOW
};

struct Firmware_t {
    char name[MAX_FIRMWARE_NAME_LENGTH];
    char version[MAX_FIRMWARE_VERSION_LENGTH];
};    
extern Firmware_t FIRMWARE;

struct Reset_t {
    bool enabled;
    bool idle;
    uint8_t triggerPin;
    uint8_t triggerState;
    uint16_t triggerTime;
    bool flaggedBySketch;
};
extern Reset_t RESET;

struct OTA_t {
    bool enabled;
};
extern OTA_t OTA;

enum class BootMode_t : uint8_t {
    UNDEFINED = 0,
    FACTORYRESET = 1,
    CONFIGURATION = 2,
    NORMAL = 3
};
extern BootMode_t BOOTMODE;
    






}; //endof namespace





#endif 
