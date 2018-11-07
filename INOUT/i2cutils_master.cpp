#include <Arduino.h>
#include <Wire.h>//this chip needs Wire

#include "i2cutils.h"
#include "dbgutils.h"


I2CMaster::I2CMaster()
: last_response("")
{}

void I2CMaster::begin()
{
  Wire.begin();

  #ifdef ESP8266
  /*
  fix: http://forum.arduino.cc/index.php?topic=336543.0; https://piandmore.wordpress.com/2016/09/14/esp-i2c-master/
  explain: http://www.digole.com/forum.php?topicID=777

  The ESP8266 doesn't have hardware I2C port, the lib for Arduino used software I2C to accomlish the function, this is a limitation (not a bug) in the lib when reading data from slave:
  if the slave can't prepare data in 230us, the read function will exit without any warning, this makes the ESP8266 can't work with all slow I2C device, for reading a touch panel click, it my need to wait much longer time till the touch panel pressed.
  */
  Wire.setClockStretchLimit(900000);
  #endif

  DPRINTLN(F("I2CMaster Begin done.."));
}

void I2CMaster::wakeupClient(uint8_t slave_addr)
{
  Wire.beginTransmission(slave_addr); // I2C start
  delay(10);
  uint8_t err=Wire.endTransmission(); // I2C end
}

bool I2CMaster::sendCommand(uint8_t slave_addr, const I2cCommand& cmd, uint8_t retryCount)
{
  /*
  err will be one of:
   0: success
   2: address NAK, No slave answered
   3: data NAK, Slave returned NAK, did not acknowledge reception of a byte
   4: other error, here is were The Arduino Library sticks any other error.
       like bus_arbitration_lost,
  The actual error codes that the TWI hardware can express is found here
  C:\Program Files\Arduino\hardware\tools\avr\avr\include\util\twi.h

  They are not returned to the user, all are lumped into err=4.
  */
  //so better coding would be

  bool success=false;
  while(!success&&(retryCount))
  {
    const char* cstr = cmd.c_str();
    uint8_t size = cmd.size();
    Wire.beginTransmission(slave_addr);
    Wire.write(cstr, size);
    uint8_t err=Wire.endTransmission();
    success=(err==0);

    if(success && cmd.require_response)
    {
      delay(100);
      success = retrieveResponse(slave_addr);
    }

    retryCount--;
    if(!success)
    {
      delay(1);    //wait a tiny bit for the 'other' master to complete
    }

  }

  return success;
}


bool I2CMaster::retrieveResponse(uint8_t slave_addr)
{
  uint8_t sizeReceived = Wire.requestFrom(slave_addr, I2cCommand::MAX_COMMAND_SIZE);

  String response_str("");
  while(Wire.available()) // slave may write less than requested
  {
    char c = Wire.read(); // read a byte as character
    //DPRINTF(F("rec:'%c', "), c);
    response_str.concat(c);
  }

  this->last_response.set(response_str);

  //DPRINTF(F(" => last_response: [%s]\n"), last_response.c_str());
  return (response_str.length()>0);
}






////////////////////////////////////////////////
