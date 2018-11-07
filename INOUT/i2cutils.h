#ifndef i2cutils_h
#define i2cutils_h

class I2CMaster;
class I2CSlave;
class I2cCommandResponse;

class I2cCommand
{
  friend I2CMaster;
  friend I2CSlave;
  friend I2cCommandResponse;

  uint8_t checksum=0; //TODO
  bool const require_response;
  String message;

protected:
  I2cCommand(const char* _message, bool require_response=true);
  void set(const String& message );
  bool checkSize(const String& _message) const;

public:
  I2cCommand(const String& OPCODE, const String& ARGUMENT, bool require_response=true );
  I2cCommand(const String& OPCODE, const String& ARGUMENT, const String& VALUE, bool require_response=true );
  const char* c_str() const;
  const uint8_t size() const;

  static const uint8_t MAX_COMMAND_SIZE;
};

class I2cCommandResponse: public I2cCommand
{
public:
  I2cCommandResponse(const char* receivedData );

};


///////////////////////////////////////////////////////////////////////////////////////////////

class I2CMaster
{
  I2cCommandResponse last_response;
  bool retrieveResponse(uint8_t slave_addr);

public:
  I2CMaster();
  void begin();
  bool sendCommand(uint8_t slave_addr, const I2cCommand& cmd, uint8_t retryCount=4);
  const I2cCommandResponse& getLastResponse() const {return last_response;}

  void wakeupClient(uint8_t slave_addr);
};



class I2CSlave
{
  static I2CSlave* _instance;
  static void onReceive(int numBytes);  //called when Master send data to client using Wire.write()
  static void onRequest();              //called when Master asks for data from client using Wire.requestFrom(). Then client uses Wire.write()

  uint8_t address=0;
  I2cCommand last_command_received;

  I2CSlave();

public:
  static I2CSlave& getInstance();

  void begin(uint8_t _address);
  const I2cCommand& getLastCommand() const {return last_command_received;}
  bool sendCommandResponse(const I2cCommandResponse& response, uint8_t retryCount=4);
};




#endif
