#include <coreapi.h>

#define THING_SERVICE_PREFIX                "_thing-"

String _BaseModule::getDiscoveryServiceName() const {
    if (this->ianaDiscoveryServiceName.length()>0) {
        return this->ianaDiscoveryServiceName;
    } else {
        return THING_SERVICE_PREFIX + this->getTitle();
    }
}


_Error _BaseModule::setupBaseModule(const JsonObject &root)
{
    if(!root.isNull())
    {
        const char* mdns_iana_service_name = root["mdns_service_name"];
        if (mdns_iana_service_name) {
            this->ianaDiscoveryServiceName = mdns_iana_service_name;
        }
        return _NoError;
    }
    return _ConfigLoadError;
}

