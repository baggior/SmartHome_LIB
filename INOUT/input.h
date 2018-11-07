#ifndef input_h
#define input_h

class Adafruit_MCP23017;

/**
  OFF = open = HIGH
  ON = closed = LOW
**/

class DigitalInput
{
  union
  {
    Adafruit_MCP23017 * mcp;
  };

  uint8_t pinIn;
  bool use_pullUp;
  bool OPENIsHIGH;

  bool inputChanged=false;
  int inputCurrValue;

public:
  DigitalInput(uint8_t pinInput, bool use_pullUp=true, bool OPENIsHIGH=true, Adafruit_MCP23017 * _mcp=NULL);

  inline bool isChanged() {return inputChanged;}
  bool getIsHIGH() {return (inputCurrValue==HIGH);}
  inline bool getIsLOW() {return !getIsHIGH();}
  bool getIsOpen() {return  (OPENIsHIGH ? (inputCurrValue==HIGH) : (inputCurrValue==LOW) );}
  inline bool getIsClosed() {return !getIsOpen();}

  void begin();
  bool update();
};



#endif
