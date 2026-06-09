#ifndef WEBUI_H
#define WEBUI_H

class AsyncWebServer;
namespace Mycila {
  class ESPConnect;
}

//
// Provide a web UI for getting and setting the kaleidoscope state.
//
void WebUI_setup(AsyncWebServer *server, Mycila::ESPConnect *espConnect);

#endif // WEBUI_H