#include "mjpeg_streamer.h"
#include "camera.h"

MJPEGStreamer::MJPEGStreamer(AsyncWebServer &server) {
    server.on("/stream", HTTP_GET, [this](AsyncWebServerRequest *request) {
        handleStream(request);
    });
}

void MJPEGStreamer::begin() {
    // Nothing needed here for now
}

void MJPEGStreamer::handleStream(AsyncWebServerRequest *request) {
    AsyncClient *client = request->client();

    if (!client || !client->connected()) {
        return;
    }

    client->write(
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n"
        "Connection: close\r\n\r\n");

    // Start MJPEG task
    AsyncClient *rawClient = client;  // Do NOT use new AsyncClient(*client)
    xTaskCreatePinnedToCore([](void *param) {
        AsyncClient *client = (AsyncClient *)param;
    
        while (client->connected()) {
            uint8_t *jpg_buf = nullptr;
            size_t jpg_len = 0;
    
            if (!getJpegFrame(&jpg_buf, &jpg_len) || !jpg_buf || jpg_len == 0) {
                delay(30);
                continue;
            }
    
            if (!client->connected()) {
                if (jpg_buf) free(jpg_buf);
                break;
            }
    
            char header[128];
            int headerLen = snprintf(header, sizeof(header),
                "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n", (unsigned int)jpg_len);
    
            client->write(header, headerLen);
            client->write(reinterpret_cast<const char *>(jpg_buf), jpg_len);
            client->write("\r\n", 2);
    
            // free(jpg_buf);
            delay(100);
        }
    
        delete client;  // only if you know you own it (optional)
        vTaskDelete(NULL);
    }, "mjpeg_stream", 8192, rawClient, 1, NULL, 1);  // <-- pass original
    
    
}
