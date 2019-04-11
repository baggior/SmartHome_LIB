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






// Calculation of CRC16
uint16_t CRC16(uint8_t *puchMsg, uint16_t usDataLen)
{
    static const uint8_t auchCRCHi[] = {
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
        0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40
    } ;
    static const uint8_t auchCRCLo[] = {
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
        0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
        0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
        0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
        0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
        0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
        0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
        0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
        0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
        0x40	
    };

  uint8_t uIndex;
  uint8_t uchCRCHi = 0xFF;
  uint8_t uchCRCLo = 0xFF;

  while (usDataLen--)
  {
    uIndex = uchCRCLo ^ *puchMsg++;
    uchCRCLo = uchCRCHi  ^ auchCRCHi[uIndex];
    uchCRCHi = auchCRCLo[uIndex] ;
  }
  return ((uchCRCHi * 0x100 )| uchCRCLo);
}



unsigned char calculateLRC( const char * frame, size_t usLen )
{
    unsigned char  ucLRC = 0;  /* LRC char initialized */

    while( usLen-- )
    {
        ucLRC += *frame++;   /* Add buffer byte without carry */
    }

    /* Return twos complement */
    ucLRC = ( char ) ( -( ( char ) ucLRC ) );
    return ucLRC;
}


unsigned char char2Binary( unsigned char ucCharacter )
{
    if( ( ucCharacter >= '0' ) && ( ucCharacter <= '9' ) )
    {
        return ( unsigned char )( ucCharacter - '0' );
    }
    else if( ( ucCharacter >= 'A' ) && ( ucCharacter <= 'F' ) )
    {
        return ( unsigned char )( ucCharacter - 'A' + 0x0A );
    }
    else
    {
        return 0xFF;
    }
}

unsigned char binNibble2Char( unsigned char ucByte )
{
    if( ucByte <= 0x09 )
    {
        return ( unsigned char )( '0' + ucByte );
    }
    else if( ( ucByte >= 0x0A ) && ( ucByte <= 0x0F ) )
    {
        return ( unsigned char )( ucByte - 0x0A + 'A' );
    }
    else
    {
        // Programming error.
        assert( 0 );
    }
    return '0';
}


} /////// End namespace


