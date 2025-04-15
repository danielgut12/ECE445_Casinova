#ifndef MJPEG_STREAMER_H
#define MJPEG_STREAMER_H

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

class MJPEGStreamer {
public:
    MJPEGStreamer(AsyncWebServer &server);
    void begin();

private:
    void handleStream(AsyncWebServerRequest *request);
};

#endif
