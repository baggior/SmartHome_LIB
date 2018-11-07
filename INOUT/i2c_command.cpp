#include <Arduino.h>
#include <Wire.h>//this chip needs Wire

#include "i2cutils.h"
#include "dbgutils.h"

const uint8_t I2cCommand::MAX_COMMAND_SIZE = 25;//BUFFER_LENGTH;

bool I2cCommand::checkSize(const String& _message) const
{
  if(size()>MAX_COMMAND_SIZE)
  {
      DPRINTF(F("I2cCommand too big (size > %d bytes) : %s"), MAX_COMMAND_SIZE, _message.c_str());
      return false;
  }
  return true;
}

I2cCommand::I2cCommand(const char* _message, bool _require_response)
: require_response(_require_response)
{
  String newMsg = String(_message);
  this->set(newMsg);
}

I2cCommand::I2cCommand(const String& opcode, const String& argument, bool _require_response)
: I2cCommand(opcode,argument, "", _require_response)
{
}

static String _buildMessageString(const String& opcode, const String& argument, const String& value)
{
    String ret_message = (opcode+"-"+argument);
    if(value.length()>0)
        ret_message+= (":"+value);

    return ret_message;
}

I2cCommand::I2cCommand(const String& opcode, const String& argument, const String& value, bool _require_response)
: I2cCommand(_buildMessageString(opcode, argument, value).c_str(), _require_response)
{
}

const char* I2cCommand::c_str()const
{
  return message.c_str();
}

const uint8_t I2cCommand::size() const
{
  return message.length();
}

void I2cCommand::set(const String& _message )
{
  if(checkSize(_message))
  {
    this->message = _message;
    //TODO update_checksum
  }
}


I2cCommandResponse::I2cCommandResponse(const char* receivedData )
: I2cCommand(receivedData, false)
{
}
