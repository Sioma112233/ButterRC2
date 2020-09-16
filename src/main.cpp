#include <Arduino.h>
#include <sntp.h>
#include <ESP8266WiFi.h>
#include <retrier.h>
#include <ESP8266mDNS.h>
#include "serialutil.h"
#include "http_server.h"
#include "keypad.h"
#include "../include/config.h"
#include "led.h"
#include "mode_specific_setup.h"

enum SetupMode {
    UNSET,
    STATION,
    ACCESS_POINT
};

bool _didGotStopInput(uint32 totalDelay);
bool _connectToWifi();

static Keypad* keypad;
static SetupMode setupMode = SetupMode::UNSET;

void __unused setup() {
    Serial.begin(115200);
    clearSerialRcvBuffer();
    if (config::load())
        Serial.println("Successfully loaded configuration!");
    else
        ESP.restart();

    int sleepTimeSec = config::DEBUG == 1 ? 4 : 1;
    Serial.println("Initializing... you have " + String(sleepTimeSec) + " seconds to abort. (send 'stop' to abort)");
    Serial.println("Waiting...");

    if (_didGotStopInput(sleepTimeSec)) {
        Serial.println("setup aborted!");
        return;
    }

    if (!_connectToWifi()) {
        Serial.println("Failed connecting to WiFi network: " + config::WIFI_SSID);
        Serial.println("Starting fallback mode (Access Point)...");

        // ACCESS_POINT mode related setup
        if (!mode_specific_setup::accessPointSetup()) {
            Serial.println("Failed setting up own AP: " + config::AP_SSID);

            // failed even the fallback, restart
            ESP.restart();
        } else {
            setupMode = SetupMode::ACCESS_POINT;
            Serial.println("Successfully set up AP!");
            return;
        }
    }

    // STATION mode related setup
    if (mode_specific_setup::stationSetup(&keypad))
        setupMode = SetupMode::STATION;
    else
        ESP.restart(); // can't really run as SetupMode::UNSET, so we just restart and try again
}

void __unused loop() {
    handleInput();
    MDNS.update();
    led::stopIfReady();
    http_server::handleClients();

    if (setupMode == SetupMode::STATION) {
        keypad->iterIfReady();
    }
}

bool _didGotStopInput(uint32 totalDelaySec) {
    uint32 endTime = sntp_get_current_timestamp() + totalDelaySec;

    char msgBuffer[sizeof("stop")];
    while (sntp_get_current_timestamp() < endTime) {
        if (Serial.available() >= (int) sizeof(msgBuffer)) {
            if (Serial.readString() == "stop")
                return true;
        }
        delay(100);
    }

    return false;
}

bool _connectToWifi() {
    auto ssid = config::WIFI_SSID;
    auto pwd = config::WIFI_PASS;
    Serial.print("Connecting to " + String(ssid));
    WiFi.mode(WIFI_STA);

    return retrier::tryFunc(3, "Retrying connecting to WiFi...", [ssid, pwd]() {
        WiFi.begin(ssid, pwd);

        uint32 connTimeoutMS = 9000;
        uint32 connStartTime = millis();

        byte status;
        while ((status = WiFi.status()) != WL_CONNECTED) {
            switch(status) {
                case wl_status_t::WL_NO_SSID_AVAIL:
                    Serial.println("Couldn't find SSID '" + ssid + "'");
                    return false;
                case wl_status_t::WL_CONNECT_FAILED:
                    Serial.println("WiFi connect failed '" + ssid + "'");
                    return false;
                default:
                    delay(250);
                    Serial.print(".");

                    auto passedTime = millis() - connStartTime;
                    if (passedTime > connTimeoutMS) {
                        Serial.println();
                        Serial.println("WiFi connection timeout: " + String(passedTime) + " msecs have passed");
                        Serial.println("Last WiFi status: " + String(status));
                        return false;
                    }

                    // allow input handling during this time
                    handleInput();
            }
        }
        Serial.println();

        Serial.print("Connected, IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    });
}


