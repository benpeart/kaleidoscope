#ifndef WEBUI_H
#define WEBUI_H

class AsyncWebServer;

//
// Provide a web UI for getting and setting the kaleidoscope state.
//
void WebUI_setup(AsyncWebServer *server);

#endif // WEBUI_H