#include "serialutil.h"
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "../include/config.h"

void clearSerialRcvBuffer() {
    while (Serial.read() >= 0);
}

String readInput() {
    if (Serial.available() == 0)
        return "";

    return Serial.readString();
}

void handleInput() {
    static auto lastIterationTime = millis();
    auto now = millis();
    if (now - lastIterationTime < 100)
        return;
    lastIterationTime = now;

    String input = readInput();
    if (input == "reset" || input == "restart") {
        Serial.println();
        ESP.restart();
    }
    if (input == "heap") {
        Serial.println();
        Serial.println(ESP.getFreeHeap());
    }
    if (input == "config"){
        Serial.println();
        Serial.println(config::getSafeConfigString());
    }
    if (input.startsWith("set")) {
        Serial.println();

        int startIndex = String("set").length() + 1; // +1 for expected space after "set"
        String kvString = input.substring(startIndex);

        config::parsing::setFromConfFormat(kvString);
        config::dump(); // save current configuration to FS
    }
}
