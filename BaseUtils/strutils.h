#ifndef strutils_h
#define strutils_h

#include <Arduino.h>

#define ENUM_TO_STR(ENUM) String(#ENUM)

#define REPLACE_NULL_STR(_TOREPLACE_CHAR_STR_) ( _TOREPLACE_CHAR_STR_ ? _TOREPLACE_CHAR_STR_ : "<NULL>" )

namespace baseutils{

String & leftPadTo(String &str, const uint8_t num, const char paddingChar);
String & rightPadTo(String &str, const uint8_t num, const char paddingChar);


template<class T>
inline Print &operator <<(Print &stream, T arg)
{ stream.print(arg); return stream; }

enum _EndLineCode { endl };

inline Print &operator <<(Print &stream, _EndLineCode arg)
{ stream.println(); return stream; }




String byteToHexString(uint8_t* buf, uint8_t length, String strSeperator="-") ;
void stringToBytes(const char* str, char sep, byte* bytes, int maxBytes, int base);


}

#endif
