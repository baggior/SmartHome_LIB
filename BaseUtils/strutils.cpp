#include <Arduino.h>
#include "strutils.h"

using namespace baseutils;

String & baseutils::leftPadTo(String &str, const uint8_t num, const char paddingChar)
{
    if(num > str.length())
       ;//TODO str.insert(0, num - str.length(), paddingChar);
    return str;
}


String & baseutils::rightPadTo(String &str, const uint8_t num, const char paddingChar)
{
    if(num > str.length())
       ;//TODO str.concat(0, num - str.length(), paddingChar);
    return str;
}

//////////////////////////////////////////////////////////////////////////////////////


static const char HEX_CHAR_ARRAY[17] = "0123456789ABCDEF";
/**
* convert char array (hex values) to readable string by seperator
* buf:           buffer to convert
* length:        data length
* strSeperator   seperator between each hex value
* return:        formated value as String
*/
String baseutils::byteToHexString(uint8_t* buf, uint8_t length, String strSeperator) {
  String dataString = "";
  for (uint8_t i = 0; i < length; i++) {
    byte v = buf[i] / 16;
    byte w = buf[i] % 16;
    if (i>0) {
      dataString += strSeperator;
    }
    dataString += String(HEX_CHAR_ARRAY[v]);
    dataString += String(HEX_CHAR_ARRAY[w]);
  }
  dataString.toUpperCase();
  return dataString;
} // byteToHexString


void baseutils::stringToBytes(const char* str, char sep, byte* bytes, int maxBytes, int base) {
  // taken from http://stackoverflow.com/a/35236734
  for (int i = 0; i < maxBytes; i++) {
    bytes[i] = strtoul(str, NULL, base);
    str = strchr(str, sep);
    if (str == NULL || *str == '\0') {
      break;
    }
    str++;
  }
}


#include <sstream>
/**
 * @brief Convert an IP address to string.
 * @param ip The 4 byte IP address.
 * @return A string representation of the IP address.
 */
String ipToString(uint8_t *ip) {
	std::stringstream s;
	s << (int) ip[0] << '.' << (int) ip[1] << '.' << (int) ip[2] << '.' << (int) ip[3];
	return s.str().c_str();
} // ipToString


