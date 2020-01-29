#if !defined (_coreapiotaupd_h)
#define _coreapiotaupd_h

#include <coreapi.h>


class _OtaUpdateModule final : public _TaskModule
{
public:
    _OtaUpdateModule();
    virtual ~_OtaUpdateModule();
    
    // void setup(Stream &serial); 
    // inline void process() { ftpServer.handleFTP(); }

protected:
    virtual _Error setup() override;

    virtual void loop() override;

private:
    
    
};

#endif //_coreapiotaupd_h