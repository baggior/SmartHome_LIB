#include <Arduino.h>
#include <Adafruit_MCP23017.h>
#include "input.h"
#include "dbgutils.h"

DigitalInput::DigitalInput(uint8_t pinInput, bool use_pullUp, bool OPENIsHIGH, Adafruit_MCP23017 * _mcp)
: mcp(_mcp)
{
  this->pinIn = pinInput;
  this->use_pullUp = use_pullUp;
  this->OPENIsHIGH = OPENIsHIGH;
}

void DigitalInput::begin()
{
  this->inputChanged = false;
  if(mcp!=NULL)
  {
    mcp->pinMode(pinIn, INPUT);
    if(use_pullUp)
      mcp->pullUp(pinIn, HIGH);
  }
  else
  {
    pinMode(pinIn, (use_pullUp ? INPUT_PULLUP: INPUT) );
    this->inputCurrValue= digitalRead(pinIn);
  }
  DPRINTF(F("Done input[%d] begin.\n"), pinIn);
}


#define FN_digitalRead(pin) fn_digitalRead(pin, mcp)
static uint8_t fn_digitalRead(uint8_t pin, Adafruit_MCP23017 * mcp)
{
  if(mcp)
  {
    return mcp->digitalRead(pin);
  }
  else
  {
    return digitalRead(pin);
  }
}

bool DigitalInput::update()
{
  inputChanged=false;
  int IN=FN_digitalRead(pinIn);

  if(IN != inputCurrValue)
  {
    inputCurrValue=IN;
    inputChanged=true;
  }
  return inputChanged;
}
