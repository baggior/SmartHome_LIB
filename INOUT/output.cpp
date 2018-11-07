#include <Arduino.h>
#include <Adafruit_MCP23017.h>
#include "output.h"
#include "dbgutils.h"

DigitalOutput::DigitalOutput(uint8_t _pinOut, bool _inverted_logic, Adafruit_MCP23017 * _mcp)
: mcp(_mcp)
{
  this->inverted_logic=_inverted_logic;
  this->pinOut = _pinOut;
}

void DigitalOutput::begin()
{
  if(mcp!=NULL)
  {
    mcp->pinMode(pinOut, OUTPUT);
  }
  else
  {
    pinMode(pinOut, OUTPUT);
  }
  setOn(false);

  DPRINTF(F("Done output[%d] begin.\n"), pinOut);
}

bool DigitalOutput::setOn(bool isOn)
{
  if(getIsOn() != isOn)
  {
    uint8_t state = inverted_logic ? ( isOn?LOW:HIGH ) : ( isOn?HIGH:LOW );
    if(mcp)
    {
      mcp->digitalWrite(pinOut, state);
    }
    else
    {
      digitalWrite(pinOut, state);
    }
    return true; //changed
  }

  return false; //nessuna modifica
}


uint8_t DigitalOutput::getDigitalValue() const
{
  if(mcp)
  {
    return mcp->digitalRead(pinOut);
  }
  else
  {
    return digitalRead(pinOut);
  }
}

bool DigitalOutput::getIsOn() const
{
  return inverted_logic ? ( LOW== getDigitalValue() ) : ( HIGH==getDigitalValue() );
}

bool DigitalOutput::getIsHIGH() const
{
  return HIGH==getDigitalValue();
}
