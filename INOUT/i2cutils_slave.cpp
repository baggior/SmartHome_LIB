#include <Arduino.h>
#include <Wire.h>//this chip needs Wire

#include "i2cutils.h"
#include "dbgutils.h"



I2CSlave* I2CSlave::_instance=NULL;

I2CSlave& I2CSlave::getInstance()
{
  if(!_instance)
  {
    _instance = new I2CSlave();
  }
  return *(_instance);
}

I2CSlave::I2CSlave()
: last_command_received("")
{}

void I2CSlave::begin(uint8_t _address)
{
  address = _address;
  Wire.begin(address);

  Wire.onReceive( I2CSlave::onReceive );
  Wire.onRequest( I2CSlave::onRequest );

  DPRINTF(F("I2CSlave Begin done at address 0x%02X ..\n"), address);
}

static constexpr const char* _hellosonoarduino= "HELLO Sono Arduino Slave";

/*
  I2cCommand::MAX_COMMAND_SIZE byte requested
*/
void I2CSlave::onRequest()
{
  //TODO
  //Echo last command received
  String last = String("ECHO: ") + String(I2CSlave::getInstance().last_command_received.c_str());

  // padding
  char buffer [I2cCommand::MAX_COMMAND_SIZE+1];
  uint8_t size_required=snprintf(buffer, sizeof(buffer),
  // "%-*.*s", I2cCommand::MAX_COMMAND_SIZE, I2cCommand::MAX_COMMAND_SIZE,
    //"%-32s",
    "%-25s",
    last.c_str());

  if(size_required > sizeof(buffer))
  {
    DPRINTF(F("buffer error: required:%d < available:%d \n"), size_required,sizeof(buffer));
  }

  String msg((const char*)buffer);

  WDT::reset();

  DPRINTF(F("I2CSlave::onRequest() -> writing to master: [%s] length:%d.."), msg.c_str(), msg.length()) ;
  Wire.write(msg.c_str(), msg.length());
  DPRINTF(F(" written %s \n"), msg.c_str());
  /*
  for (int idx=0; idx<I2cCommand::MAX_COMMAND_SIZE; ++idx)
  {
    uint8_t w_ret=Wire.write(msg[idx]);
    delay(1);
    DPRINTF(F(" write %c(ret:%d) "), msg[idx], w_ret);
  }
  */
//  uint8_t ret_trasmit =twi_transmit((const uint8_t*)msg.c_str(),(size_t) msg.length());

}


void I2CSlave::onReceive(int numBytes)
{
  //TODO
  String received_str("");
  DPRINTF(F("I2CSlave::onReceive(numBytes=%d): ["),numBytes);
  while (Wire.available()) { // loop through all received bytes
    char c = Wire.read(); // receive byte as a character
    DPRINTF(("%c, "),c);
    received_str.concat(c);
  }
  DPRINTLN(F("]"));
  I2CSlave::getInstance().last_command_received.set(received_str);
}


bool I2CSlave::sendCommandResponse(const I2cCommandResponse& response, uint8_t retryCount)
{
  //TODO
  return false;
}
