#include "coreapi_otaupdmodule.h"

#include <esp32fota.h>

#define DEFAULT_OTAUPD_TASK_LOOP_TIME_MS    24 * 60 * 60 * 1000     // 86400000 ms in a day


_OtaUpdateModule::_OtaUpdateModule() 
: _TaskModule(ENUM_TO_STR(_CoreOTAUpdateModule), "OTA Firmware update module", DEFAULT_OTAUPD_TASK_LOOP_TIME_MS, true)
{
    // TODO:
    
}

_OtaUpdateModule::~_OtaUpdateModule() 
{
    // TODO:
    
}


_Error _OtaUpdateModule::setup() 
{
    // TODO:


    return _NoError;
}


void _OtaUpdateModule::loop() 
{
    // TODO: esp32FOTA FOTA("<Type of Firme for this device>", <this version>);
    esp32FOTA FOTA("esp32-fota", 1);
    FOTA.useDeviceID = true;
    FOTA.checkURL = "http://server/fota/firmware.json";
    /**
     * firmware.json sample content:
     *  
     * {
            "type": "esp32-fota-http",
            "version": 2,
            "host": "192.168.0.100",
            "port": 80,
            "bin": "/fota/esp32-fota-http-2.bin"
        }
     **/

    bool updatedNeeded = FOTA.execHTTPcheck();
    if (updatedNeeded)
    {
        // if Ota firmware update is OK => ESP resets and don't returns
        FOTA.execOTA();
    }

    delay(2000);
}
