#ifndef dbgutils_h
#define dbgutils_h

#include <Arduino.h>


#ifdef MY_DEBUG    //Macros are usually in all capital letters.


  #ifndef DEBUG_OUTPUT
    #ifdef DEBUG_ESP_PORT
      #define DEBUG_OUTPUT DEBUG_ESP_PORT
    #else
      #define DEBUG_OUTPUT Serial
    #endif
  #endif
  
  #define DPRINT(...)     DEBUG_OUTPUT.print  (__VA_ARGS__)     //DPRINT is a macro, debug print
  #define DPRINTLN(...)   DEBUG_OUTPUT.println (__VA_ARGS__)   //DPRINTLN is a macro, debug print with new line
  #define DPRINTF(...)    Stream_printf (DEBUG_OUTPUT,__VA_ARGS__)   //DPRINTF is a macro, debug printf
#else
  #define DPRINT(...)     //now defines a blank line
  #define DPRINTLN(...)   //now defines a blank line
  #define DPRINTF(...)   //now defines a blank line
#endif


void Stream_printf(Stream &stream, const char *fmt, ... );
void Stream_printf_args(Stream &stream, const char *fmt, va_list args );
void Stream_printf(Stream &stream, const __FlashStringHelper *fmt, ... );
void Stream_printf_args(Stream &stream, const __FlashStringHelper *fmt, va_list args );



#ifndef ESP32

class WDT
{
  static bool enabled;
public:
  static void reset();

  static void pause();
  static void resume();

  static void enable();
  static void disable();
};

#endif

#endif
