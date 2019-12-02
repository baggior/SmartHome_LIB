#include "dbgutils.h"

#include <stdarg.h>

#ifdef i2cscanutils_h
#include <Wire.h> //this chip needs Wire
#include <i2cdetect.h>  //this chip needs i2cdetect
#endif




// #ifndef PRINTF_BUFFER_SIZE
#define PRINTF_BUFFER_SIZE 1024  // resulting string limited to PRINTF_BUFFER_SIZE chars
// #endif

void Stream_printf(Stream &stream, const char *fmt, ... )
{
// char buf[PRINTF_BUFFER_SIZE]; // resulting string limited to PRINTF_BUFFER_SIZE chars
// va_list args;
// va_start (args, fmt );
// vsnprintf(buf, PRINTF_BUFFER_SIZE, fmt, args);
// va_end (args);

// stream.print(buf);
  va_list args;
  va_start (args, fmt);
  Stream_printf_args(stream, fmt, args);
  va_end(args);
}
void Stream_printf_args(Stream &stream, const char *fmt, va_list args )
{
  char buf[PRINTF_BUFFER_SIZE]; // resulting string limited to PRINTF_BUFFER_SIZE chars
  vsnprintf(buf, PRINTF_BUFFER_SIZE, fmt, args);
  stream.print(buf);
}

void Stream_printf(Stream &stream, const __FlashStringHelper *fmt, ... ){
//   char buf[PRINTF_BUFFER_SIZE]; // resulting string limited to PRINTF_BUFFER_SIZE chars
//   va_list args;
//   va_start (args, fmt);
// #ifdef __AVR__
//   vsnprintf_P(buf, sizeof(buf), reinterpret_cast<PGM_P>(fmt), args); // progmem for AVR
// #elif defined(ESP8266)
//   vsnprintf_P(buf, sizeof(buf), reinterpret_cast<PGM_P>(fmt), args);
//   //ets_vsnprintf(buf, sizeof(buf), reinterpret_cast<PGM_P>(fmt), args);
// #else
//   vsnprintf(buf, sizeof(buf), (const char *)fmt, args); // for the rest of the world
// #endif
//   va_end(args);
  
//   stream.print(buf);

  va_list args;
  va_start (args, fmt);
  Stream_printf_args(stream, fmt, args);
  va_end(args);
}
void Stream_printf_args(Stream &stream, const __FlashStringHelper *fmt, va_list args )
{
  char buf[PRINTF_BUFFER_SIZE]; // resulting string limited to PRINTF_BUFFER_SIZE chars

#ifdef __AVR__
  vsnprintf_P(buf, sizeof(buf), reinterpret_cast<PGM_P>(fmt), args); // progmem for AVR
#elif defined(ESP8266)
  vsnprintf_P(buf, sizeof(buf), reinterpret_cast<PGM_P>(fmt), args);
  //ets_vsnprintf(buf, sizeof(buf), reinterpret_cast<PGM_P>(fmt), args);
#else
  vsnprintf(buf, sizeof(buf), (const char *)fmt, args); // for the rest of the world
#endif

  stream.print(buf);
}


#ifdef i2cutils_h
void Serial_scanI2cDevices()
{
    Wire.begin();
    //I2C detect
    const uint8_t first = 0x03;
    const uint8_t last = 0x77;

    DPRINTLN(F("I2C i2cdetect:"));
    DPRINTF(F("I2C Scanning addressrange %02x-%02x\n\n"), first, last);

    WDT::reset();

    i2cdetect(first, last);  // default range from 0x03 to 0x77
    #ifdef __AVR__
    Wire.end();
    #endif
}
#endif




#ifdef __AVR__

  #include <avr/wdt.h>  

#endif


#ifndef ESP32

bool WDT::enabled=false;


void WDT::reset()
{
  if(WDT::enabled)
    wdt_reset();
}
void WDT::enable()
{
  if(!WDT::enabled)
  {
    wdt_enable(WDTO_4S); WDT::enabled = true;
    //DPRINTLN(F("WDT enabled"));
  }
}
void WDT::disable()
{
  if(WDT::enabled)
  {
    // in ESP8266: Please don't stop software watchdog too long (less than 6 seconds),
    /// otherwise it will trigger hardware watchdog reset.
    wdt_disable(); WDT::enabled = false;
    //DPRINTLN(F("WDT disabled"));
  }
}
void WDT::pause()
{
  if(WDT::enabled)
  {
    wdt_disable();
  }
}
void WDT::resume()
{
  if(WDT::enabled)
  {
    wdt_enable(WDTO_4S);
  }
}

#endif