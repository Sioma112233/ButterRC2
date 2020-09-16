#include <WString.h>
#include <ESP8266HTTPClient.h>
#include <base64.h>
#include "popcorn_client.h"
#include "../include/config.h"

static HTTPClient http;

static String createAuthHeader(const String& user, const String& pass) {
    String userAndPass = user + ":" + pass;

    String encodedUserAndPass = base64::encode(userAndPass);

    return "Basic " + String(encodedUserAndPass);
}

popcorn_api::Result popcorn_api::call(const char *apiMethod) {
    String url = "http://" + config::PC_IP + ":" + config::POPCORN_PORT;

    WiFiClient wifiClient;
    http.begin(wifiClient, url);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", createAuthHeader(config::POPCORN_AUTH_USER, config::POPCORN_AUTH_PASS));

    int statusCode = http.POST(
        "{"
            "\"jsonrpc\":\"2.0\", "
            "\"method\":\"" + String(apiMethod) + "\", "
            "\"params\":{}"
        "}"
    );

    Result result;

    if (statusCode <= 0) {
        result = Result{statusCode, HTTPClient::errorToString(statusCode)};
    } else {
        result = Result{statusCode, http.getString()};
    }

    http.end();
    return result;
}

