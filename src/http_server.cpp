#include "http_server.h"
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include "../include/config.h"


static void restart();


const String EMPTY_STRING = "";

#define IF_FOUND_THEN_SET_AS_STR(CONFIG_NAME) \
    auto CONFIG_NAME = pServer->arg(#CONFIG_NAME); \
    if (CONFIG_NAME != EMPTY_STRING) config::CONFIG_NAME = CONFIG_NAME;
#define IF_FOUND_THEN_SET_AS_IP(CONFIG_NAME) \
    auto CONFIG_NAME = pServer->arg(#CONFIG_NAME); \
    if (CONFIG_NAME != EMPTY_STRING) config::CONFIG_NAME = config::parsing::parseIp(CONFIG_NAME);
#define IF_FOUND_THEN_SET_AS_BOOL(CONFIG_NAME) \
    auto CONFIG_NAME = pServer->arg(#CONFIG_NAME); \
    if (CONFIG_NAME != EMPTY_STRING) config::CONFIG_NAME = config::parsing::parseBool(CONFIG_NAME);


ESP8266WebServer* pServer; // will have infinite lifetime

void http_server::init() {
    pServer = new ESP8266WebServer(80);

    pServer->on("/config", []() {
        pServer->send(200, "text/plain", config::getSafeConfigString());
    });

    pServer->on("/updateConfig", []() {
        IF_FOUND_THEN_SET_AS_STR(WIFI_SSID)
        IF_FOUND_THEN_SET_AS_STR(WIFI_PASS)
        IF_FOUND_THEN_SET_AS_STR(PC_IP)
        IF_FOUND_THEN_SET_AS_STR(LOCAL_HOSTNAME)
        IF_FOUND_THEN_SET_AS_STR(POPCORN_PORT)
        IF_FOUND_THEN_SET_AS_STR(POPCORN_AUTH_USER)
        IF_FOUND_THEN_SET_AS_STR(POPCORN_AUTH_PASS)
        IF_FOUND_THEN_SET_AS_STR(VLC_PORT)
        IF_FOUND_THEN_SET_AS_STR(AP_SSID)
        IF_FOUND_THEN_SET_AS_STR(AP_PASS)
        IF_FOUND_THEN_SET_AS_IP(AP_LOCAL_IP)
        IF_FOUND_THEN_SET_AS_IP(AP_GATEWAY)
        IF_FOUND_THEN_SET_AS_IP(AP_NETMASK)
        IF_FOUND_THEN_SET_AS_BOOL(DEBUG)

        config::dump();
        pServer->send(200, "text/plain");
    });

    pServer->on("/reset", restart);
    pServer->on("/restart", restart);

    pServer->begin();
    Serial.println("[DEBUG] http_server module initiated");
}

void http_server::handleClients() {
    pServer->handleClient();
}

static void restart() {
    pServer->send(200, "text/plain");
    delay(1000);
    ESP.restart();
}
