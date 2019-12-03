#if defined (COREAPI_REST_ENABLED) && !defined (_coreapirest_h)

#define _coreapirest_h

#include "coreapi.h"

class AsyncWebServer;
/*
- This is fully asynchronous server and as such does not run on the loop thread.
- You can not use yield or delay or any function that uses them inside the callbacks
- The server is smart enough to know when to close the connection and free resources
- You can not send more than one response to a single request
*/
class _RestApiModule : public _BaseModule
{
public:
    
    typedef std::function<void(_Application* theApp, const JsonObject* const requestPostBody, const JsonObject* const responseBody)> RestHandlerCallback;
    
    inline _RestApiModule(): _BaseModule( ENUM_TO_STR(_CoreRestApiModule), ("Core Rest Api module"), false, Order_AfterNormal) {}
    inline virtual ~_RestApiModule() { this->shutdown(); }

protected:
    inline _RestApiModule(String _title, String _descr) : _BaseModule(_title, _descr, false) {}

    virtual _Error setup() override;
    virtual void shutdown() override;    
    inline virtual void loop() override final{} //nothing, unused for ASYNC server
    virtual void beforeModuleAdded() override ;

    virtual _Error additionalRestApiMethodSetup();
    void addRestApiMethod(const char* uri, RestHandlerCallback callback, bool isGetMethod=true, size_t maxJsonBufferSize=1024 );

private:
    _Error restApiMethodSetup();

    AsyncWebServer * webServer = NULL;
    unsigned int _server_port = 0;    

};

#endif //