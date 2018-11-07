#include <Arduino.h>
#include <pins_arduino.h>
#include <dbgutils.h>

#include "baseutils.h"


using namespace baseutils;


namespace baseutils {    
    
//EXTERNS
BootMode_t   BOOTMODE;
Firmware_t   FIRMWARE;
OTA_t        OTA;
Reset_t      RESET;


///////

// String is numeric

boolean isNum(String str) {

    boolean isNum = false;

    for (uint8_t i = 0; i < str.length(); i++) {
        isNum = isdigit(str.charAt(i)) || str.charAt(i) == '+' ||
                        str.charAt(i) == '.' || str.charAt(i) == '-';
        if (!isNum)
            return false;
    }

    return isNum;
}

// Convert string para integer

int32_t convStrInt(String str) {

    if (isNum(str)) {

        int32_t valor = str.toInt();

        return valor;

    }

    return 0; // Numero invalido

}

// Converte string para float

float convStrFloat(String str) {

	char floatbuf[5];

	str.toCharArray(floatbuf, sizeof(floatbuf));

	float ret = atof(floatbuf);

	return ret;
}

// Round float

float roundFloat(float value, uint8_t decimals) {

    float factor = round(pow(10, decimals));

    int aux = round(value * factor);

    float ret = ((float) aux / factor);

    return ret;
}

// Convert float to string (dtostrf is not good)

String convFloatStr(float value, uint8_t decimals, boolean comma) {

    // Convert

    String ret = String(roundFloat(value, decimals));

    // Comma ?

    if (comma)
        ret.replace('.', ',');

    // Return

    return ret;
}

String formatFloat(float value, uint8_t intPlaces, uint8_t decPlaces, boolean comma)
{
    String ret = "";

    // Signal

    if (value < 0.0f) {
        value*=-1.0f;
        ret.concat('-');
    }
    // Integer

    float factor = round(pow(10, decPlaces));

    uint32_t aux = (uint32_t) roundFloat(value * factor, 0);

    uint32_t factorInt = (uint32_t) factor;

    uint32_t valInt = (uint32_t) (aux / factorInt);

    // Decimal

    uint32_t valDec = (aux - (valInt * factorInt));

    // Return

    if (intPlaces == 0) {
        ret.concat(valInt);
    } else {
        ret.concat(formatNumber(valInt, intPlaces));
    }

    ret.concat((comma)?',':'.');

    ret.concat(formatNumber(valDec, decPlaces));

    // Return

    return ret;

}

// Format numbers

String formatNumber(uint32_t value, uint8_t size, char insert) {

    // Putting zeroes in left

    String ret = "";

    for (uint8_t i=1; i<=size; i++) {
        uint32_t max = pow(10, i);
        if (value < max) {
            for (uint8_t j=(size - i); j>0; j--) {
                ret.concat(insert);
            }
            break;
        }
    }

    ret.concat(value);

    return ret;
}

// Get field from a string with delimiters

String getFieldStr(String str, uint8_t fieldNum, char delimiter) {

    uint16_t pos = 0;
    uint16_t lastPos = 0;
    uint16_t field = 0;
    uint16_t size = str.length() + 1;

    str.concat(delimiter);

    do {

        pos = str.indexOf(delimiter, lastPos);

        if (pos > 0) {

            if (pos == size) {

                break;

            }

            field++;

            if (field == fieldNum) {

                return (str.substring(lastPos, pos));

            }

            lastPos = pos + 1;

        }
    }
    while (pos > 0);

    return ""; // If not found, return empty

}

// Get field char

char getFieldChar(String str, uint8_t fieldNum, char delimiter) {

    String aux = getFieldStr(str, fieldNum, delimiter);
    return aux.charAt(0);
}


// Get field integer

int32_t getFieldInt(String str, uint8_t fieldNum, char delimiter) {

    String aux = getFieldStr(str, fieldNum, delimiter);

    if (isNum(aux)) { // OK

        return aux.toInt();

    } else { // Err

        String msgErr = "*ArduinoUtil:getFieldInt: value of field ";
        msgErr.concat(fieldNum);
        msgErr.concat(" not is a number: ");
        msgErr.concat(aux);
        Serial.println(msgErr);

        return 0;
    }
}

// Get field float

float getFieldFloat(String str, uint8_t fieldNum, char delimiter) {

    String aux = getFieldStr(str, fieldNum, delimiter);

    if (isNum(aux)) { // Ok

        return aux.toFloat();

    } else { // Err

        String msgErr = "*ArduinoUtil:getFieldFloat: value of field ";
        msgErr.concat(fieldNum);
        msgErr.concat(" not is a float: ");
        msgErr.concat(aux);
        Serial.println(msgErr);

        return 0;
    }
}

////// For ESP8266 or ESP32

#if defined ESP8266 || defined ESP32

#if defined ESP32

static String getESP32ChipID() {
  uint64_t chipid;
  chipid=ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  int chipid_size = 6;
  uint8_t chipid_arr[chipid_size];
  for (uint8_t i=0; i < chipid_size; i++) {
    chipid_arr[i] = (chipid >> (8 * i)) & 0xff;
  }
  return byteToHexString(chipid_arr, chipid_size, "");
}

#endif

String getChipId() {
    #ifdef ESP32 
    return getESP32ChipID();
    #else 
    return String(ESP.getChipId());
    #endif
}

// Open a file

File openFile(const char* fileName, const char* mode) {

    // Open

    File file = SPIFFS.open(fileName, mode);

    if (!file) {
        Serial.println("*ArduinoUtil:openFile:file open failed");
    }

    // Return

    return file;

}

// Remove a file

boolean removeFile(const char* fileName) {

    // Exists ?

    if (SPIFFS.exists(fileName) == false) { // Not exist - considered removed
        return true;
    } else { // Delete
        return SPIFFS.remove(fileName);
    }

}

// Read all the text of file

String readTextFile(const char* fileName) {

    String data = "";

    // File exists ?

    if (SPIFFS.exists(fileName) == false) { // Not exist - return zero string
        return "";
    }

    // Open the file

    File file = baseutils::openFile(fileName, "r");

    if (file) { // Success

        // Get all the text

        data = file.readString();

        // Close file

        file.close();

    } else {

        // Return a empty string

        data = "";
    }

    // Return

    return data;

}


void ESP_restart()
{
    DPRINTLN("------ESP soft restart------");    
    ESP.restart();
    delay(1000);
}


void ESP_eraseConfig()
{
#ifdef ESP32

    DPRINTLN("------ESP eraseConfig not implemented for ESP32------");  

#else

    DPRINTLN("------ESP eraseConfig------");  
    #ifdef WiFi_h
        WiFi.disconnect();  
    #endif
    ESP.eraseConfig();
    ESP_restart();
    
#endif
}


#endif //ESP8266 ESP32


/*
String & leftPadTo(String &str, const uint8_t num, const char paddingChar)
{
    if(num > str.length())
       ;//TODO str.insert(0, num - str.length(), paddingChar);
    return str;
}


String & rightPadTo(String &str, const uint8_t num, const char paddingChar)
{
    if(num > str.length())
       ;//TODO str.concat(0, num - str.length(), paddingChar);
    return str;
}
*/








void abort(const String& message) {
    Serial.begin(115200);
    Serial << message << endl;
    Serial.flush();
    ::abort();
  }
  
  uint8_t rssiToPercentage(int32_t rssi) {
    uint8_t quality;
    if (rssi <= -100) {
      quality = 0;
    } else if (rssi >= -50) {
      quality = 100;
    } else {
      quality = 2 * (rssi + 100);
    }
  
    return quality;
  }
  
  
  bool validateMacAddress(const char* mac) {
    // taken from http://stackoverflow.com/a/4792211
    int i = 0;
    int s = 0;
    while (*mac) {
      if (isxdigit(*mac)) {
        i++;
      } else if (*mac == ':' || *mac == '-') {
        if (i == 0 || i / 2 - 1 != s)
          break;
        ++s;
      } else {
         s = -1;
      }
      ++mac;
    }
    return (i == MAX_MAC_STRING_LENGTH && s == 5);
  }
  
  bool validateMd5(const char* md5) {
    if (strlen(md5) != 32) return false;
  
    for (uint8_t i = 0; i < 32; i++) {
      char c = md5[i];
      bool valid = (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
      if (!valid) return false;
    }
  
    return true;
  }
  
  std::unique_ptr<char[]> cloneString(const String& string) {
    size_t length = string.length();
    std::unique_ptr<char[]> copy(new char[length + 1]);
    memcpy(copy.get(), string.c_str(), length);
    copy.get()[length] = '\0';
  
    return copy;
  }





// SYSTEM INFO




#ifdef ESP8266

  extern void print_esp8266Info(Stream & consolePort);

  void printBoardInfo(Stream &stream, uint8_t detail_level)
  {
    Stream_printf(stream, F("#### ESP8266 BOARDINFO START ####"));
    //reset
    String resetInfo = String("ESP8266 reset Cause:") + ESP.getResetReason() + String("\n\tresetInfo: ") + ESP.getResetInfo();
    Stream_printf(stream, resetInfo.c_str());

    if(detail_level>0)
    {
      WDT::reset();
      //chip
      Stream_printf(stream, F("\nChip ID: %s"), baseutils::getChipId().c_str());
      Stream_printf(stream, F("\nChip speed: %d MHz"), ESP.getCpuFreqMHz()); //in MHz
      Stream_printf(stream, F("\nFree Heap: %d Bytes"), ESP.getFreeHeap()); //bytes
      Stream_printf(stream, F("\nVCC power Voltage: %s V"), String((float)getVcc()).c_str() ); //Measure the power voltage of VDD3P3 pin 3 and 4; unit: 1/1024 V.

      //versions
      Stream_printf(stream, F("\nVersions:\n\t SDK = %s\n\t Core = %s\n\t Boot = %d (Boot in %s Mode)"),
        ESP.getSdkVersion(), ESP.getCoreVersion().c_str(), ESP.getBootVersion(), (ESP.getBootMode()==0 ? "Enhance" : "Normal") );

    }

    if(detail_level>1)
    {
      WDT::reset();
      //sketch
      String sketch_size = String("\nSketch Size: ") + ESP.getSketchSize() + String("(") +  ESP.getFreeSketchSpace() + String(" free)");
      Stream_printf(stream, sketch_size.c_str());

      //Flash
      uint32_t realSize = ESP.getFlashChipRealSize();
      uint32_t ideSize = ESP.getFlashChipSize();
      FlashMode_t ideMode = ESP.getFlashChipMode();
      Stream_printf(stream, F("Flash real id:   %08X\n"), ESP.getFlashChipId());
      Stream_printf(stream, F("Flash real size: %u\n\n"), realSize);
      Stream_printf(stream, F("Flash size: %u\n"), ideSize);
      Stream_printf(stream, F("Flash speed: %u\n"), ESP.getFlashChipSpeed());
      Stream_printf(stream, F("Flash mode:  %s\n"), (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));
      if(ideSize != realSize) {
        Stream_printf(stream, F("Flash Chip configuration wrong!\n"));
      } else {
        Stream_printf(stream, F("Flash Chip configuration ok.\n"));
      }
    }

    if(detail_level>2)
    {
      WDT::reset();
      //Others details
      print_esp8266Info(stream);
    }

    Stream_printf(stream, F("#### ESP8266 BOARDINFO END . ####"));
  }

#elif defined ESP32

extern void print_esp32Info(Stream & consolePort);

void printBoardInfo(Stream &stream, uint8_t detail_level)
{
    
  Stream_printf(stream, F("#### ESP32 BOARDINFO START ####"));
  //reset
  // TODO String resetInfo = String("ESP32reset Cause:") + ESP.getResetReason() + String("\n\tresetInfo: ") + ESP.getResetInfo();
  // DPRINTLN(resetInfo.c_str());

  if(detail_level>0)
  {    
    //chip
    Stream_printf(stream, F("\nChip ID: %s\nChip Revision: %u\n"),
        baseutils::getChipId().c_str(), ESP.getChipRevision() );
    Stream_printf(stream, F("\nChip speed: %d MHz"), ESP.getCpuFreqMHz()); //in MHz
    Stream_printf(stream, F("\nFree Heap: %d Bytes"), ESP.getFreeHeap()); //bytes

    //versions
    Stream_printf(stream, F("\nSDK Version: %s\n"), ESP.getSdkVersion() );

  }

  if(detail_level>1)
  {
   
    //Flash
    // uint32_t realSize = ESP.getFlashChipRealSize();
    uint32_t ideSize = ESP.getFlashChipSize();
    FlashMode_t ideMode = ESP.getFlashChipMode();
    // DPRINTF(F("Flash real id:   %08X\n"), ESP.getFlashChipId());
    // DPRINTF(F("Flash real size: %u\n\n"), realSize);
    Stream_printf(stream, F("Flash size: %u\n"), ideSize);
    Stream_printf(stream, F("Flash speed: %u\n"), ESP.getFlashChipSpeed());
    Stream_printf(stream, F("Flash mode:  %s\n"), (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : ideMode == FM_FAST_READ ? "FAST_READ" : ideMode == FM_SLOW_READ ? "SLOW_READ" : "UNKNOWN"));
    
  }

  if(detail_level>2)
  {
    //Others details
    print_esp32Info(stream);
  }

  Stream_printf(stream, F("#### ESP32 BOARDINFO END . ####"));
}


#else //__AVR__

  void printBoardInfo(uint8_t detail_level)
  {
    WDT::reset();

    DPRINTLN(F("#### AVR BOARDINFO START ####"));
    DPRINTLN(F("Board: GENERIC AVR!"));
    DPRINTF(F("VCC power Voltage: %s V \n"), String( getVcc()).c_str() );
    DPRINTF(F("Temperature: %s °C \n"), String( readTemp()).c_str() );
    DPRINTLN(F("#### AVR BOARDINFO END . ####"));
  }


#endif







} /////// End namespace


