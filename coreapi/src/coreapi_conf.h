#ifndef _coreapiconfig_h
#define _coreapiconfig_h

#ifndef FW_VERSION 
#define FW_VERSION      0.0.1
#endif


#ifndef GIT_SRC_REV 
#define GIT_SRC_REV     GIT_SRC_REV_undefined
#endif


#define MAX_MODULES         20
#define MAX_MDNS_ATTRIBUTES 10 

#define DEFAULT_TASK_LOOP_TIME_MS 100 
#define TASK_LOG_TIME_MS  1000


#include "boards/standard.h"


enum CoreModuleNamesEnum {
    _CoreWifiConnectionModule,
    _CoreOTAUpdateModule,
    _CoreFTPServerModule,
    _CoreRestApiModule,
    _CoreMqttModule
};


#endif




