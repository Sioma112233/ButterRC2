#include <Arduino.h>
#include "../include/config.h"
#include "led.h"


#define SKIP_IF_DEBUG if (config::DEBUG) return;


void led::init() {
    SKIP_IF_DEBUG

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
}

static uint32 lastBlinkStartTime_ = 0;
static uint32 msInterval_;

void led::blink(uint32 msInterval) {
    SKIP_IF_DEBUG

    msInterval_ = msInterval;
    lastBlinkStartTime_ = millis();
    digitalWrite(LED_PIN, HIGH);
}

void led::stopIfReady() {
    SKIP_IF_DEBUG

    auto now = millis();
    if (now - lastBlinkStartTime_ < msInterval_)
        return;

    digitalWrite(LED_PIN, LOW);
}
