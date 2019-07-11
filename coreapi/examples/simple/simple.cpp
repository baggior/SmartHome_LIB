#include <Arduino.h>

#include <coreapi.h>
#include <coreapi_ftpmodule.h>

_Application app;
WifiFtpServerModule wifiFtpServer;

void setup() {
    delay(2000);

    app.getLogger().info("\n-----MAIN setup start-----\n");
    app.addModule(&wifiFtpServer);

    _Error ret = app.setup();
    if(ret!=_NoError) {
        app.getLogger().error("\n-----MAIN setup done ERROR-----\n%d: %s\n---------------\n\n", ret.errorCode, ret.message.c_str());
        delay(3000);        
    }
    else {
        app.getLogger().info("\n-----MAIN setup done OK-----\n");
    }
}



void loop() 
{   
   
    app.loop();     
    
}

