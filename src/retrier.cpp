#include <HardwareSerial.h>
#include "retrier.h"

bool retrier::tryFunc(uint8 numberOfTries, const char* retryMessage, const std::function<bool(void)>& func) {
    while(numberOfTries > 0) {
        if (func())
            return true;

        numberOfTries--;
        if (numberOfTries > 0)
            Serial.println(retryMessage);
    }

    return false;
}
