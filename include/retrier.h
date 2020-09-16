#ifndef BUTTERRC2_RETRIER_H
#define BUTTERRC2_RETRIER_H

#include <c_types.h>
#include <functional>

namespace retrier {
    bool tryFunc(uint8 numberOfTries, const char* retryMessage, const std::function<bool(void)>& func);
}

#endif //BUTTERRC2_RETRIER_H
