#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "mode_specific_setup.h"
#include "vlc_client.h"
#include "http_server.h"
#include "led.h"
#include "popcorn_client.h"
#include "../include/config.h"

static void mapKeyPressesToApiActions();
static bool setAP();

static Keypad* keypad;

bool mode_specific_setup::stationSetup(Keypad** pKeypadToInit) {
    *pKeypadToInit = keypad = new Keypad(50);
    mapKeyPressesToApiActions();

    if (!MDNS.begin(config::LOCAL_HOSTNAME))
        return false;

    http_server::init();

    Serial.println("[DEBUG] station specific setup finished successfully");
    return true;
}

bool mode_specific_setup::accessPointSetup() {
    if (!setAP())
        return false;

    if (!MDNS.begin(config::LOCAL_HOSTNAME))
        return false;

    http_server::init();

    Serial.println("[DEBUG] access_point specific setup finished successfully");
    return true;
}


static void mapSingleKeyPressToApiAction(KeypadButtons key, const char* popcornApiMethod, const char* vlcApiMethod=nullptr) {
    keypad->onBtnPressed(key, [popcornApiMethod, vlcApiMethod] (KeypadButtons x) {
        led::blink(200);

        auto result = popcorn_api::call(popcornApiMethod);
        if (result.status <= 0) {
            Serial.println("Failed calling popcorn api method '" + String(popcornApiMethod) + "', Reason: " + result.content);
        }
        else if (result.status != 200) {
            Serial.println("Received unexpected status code when calling popcorn api method '"
                + String(popcornApiMethod) + "'");
            Serial.println("status: " + String(result.status));
        }

        // on the way, try calling vlc api
        if (vlcApiMethod != nullptr) {
            int resultStatus = vlc_api::call(vlcApiMethod);
            if (resultStatus != 200)
                Serial.println("Received unexpected status code when calling vlc api '" + String(vlcApiMethod) + "'");
        }
    });
}

static void mapKeyPressesToApiActions() {
    led::init();

    mapSingleKeyPressToApiAction(KeypadButtons::hashtag, popcorn_api::API_BACK);
    mapSingleKeyPressToApiAction(KeypadButtons::zero, popcorn_api::API_PLAY, vlc_api::API_PLAY);
    mapSingleKeyPressToApiAction(KeypadButtons::star, popcorn_api::API_FULLSCREEN, vlc_api::API_FULLSCREEN);
    mapSingleKeyPressToApiAction(KeypadButtons::nine, popcorn_api::API_PREVIOUS_SEASON);
    mapSingleKeyPressToApiAction(KeypadButtons::eight, popcorn_api::API_UP, vlc_api::API_VOL_UP);
    mapSingleKeyPressToApiAction(KeypadButtons::seven, popcorn_api::API_QUALITY);
    mapSingleKeyPressToApiAction(KeypadButtons::six, popcorn_api::API_LEFT, vlc_api::API_SEEK_BACKWARD);
    mapSingleKeyPressToApiAction(KeypadButtons::five, popcorn_api::API_ENTER);
    mapSingleKeyPressToApiAction(KeypadButtons::four, popcorn_api::API_RIGHT, vlc_api::API_SEEK_FORWARD);
    mapSingleKeyPressToApiAction(KeypadButtons::three, popcorn_api::API_NEXT_SEASON);
    mapSingleKeyPressToApiAction(KeypadButtons::two, popcorn_api::API_DOWN, vlc_api::API_VOL_DOWN);
    mapSingleKeyPressToApiAction(KeypadButtons::one, popcorn_api::API_FAVOURITES);
}

static bool setAP() {
    WiFi.mode(WIFI_AP);
    if (!WiFi.softAPConfig(config::AP_LOCAL_IP, config::AP_GATEWAY, config::AP_NETMASK))
        return false;

    return WiFi.softAP(config::AP_SSID, config::AP_PASS);
}
