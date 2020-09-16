#ifndef BUTTERRC2_LED_H
#define BUTTERRC2_LED_H

#include <c_types.h>

namespace led {
    const int LED_PIN = 1;

    void init();
    void blink(uint32 msInterval);
    void stopIfReady();
}

#endif //BUTTERRC2_LED_H
