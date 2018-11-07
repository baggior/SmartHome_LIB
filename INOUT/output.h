#ifndef output_h
#define output_h

class Adafruit_MCP23017;

class DigitalOutput
{
  union
  {
    Adafruit_MCP23017 * mcp;
  };

  bool inverted_logic=false;
  uint8_t pinOut;
public:
  DigitalOutput(uint8_t pinOutput, bool inverted_logic=false, Adafruit_MCP23017 * _mcp=NULL);

  void begin();

  bool setOn(bool isOn=true);
  inline bool setOff() {return setOn(false); }

  bool getIsOn()const;
  inline bool getIsOff() const{return !getIsOn(); }

  bool getIsHIGH()const;
  inline bool getIsLOW() const{return !getIsHIGH(); }

  uint8_t getDigitalValue() const;


};

#endif
