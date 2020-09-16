#include <ESP8266HTTPClient.h>
#include "vlc_client.h"
#include "../include/config.h"

static HTTPClient http;

int vlc_api::call(const char *apiMethod) {
    String url = "http://" + config::PC_IP + ":" + config::VLC_PORT + "/requests/" + apiMethod;

    WiFiClient wifiClient;
    http.begin(wifiClient, url);

    int statusCode = http.GET();

    http.end();
    return statusCode;
}
