#ifndef BUTTERRC2_MODE_SPECIFIC_SETUP_H
#define BUTTERRC2_MODE_SPECIFIC_SETUP_H

#include "keypad.h"


namespace mode_specific_setup {
    bool stationSetup(Keypad** pKeypadToInit);
    bool accessPointSetup();
}

#endif //BUTTERRC2_MODE_SPECIFIC_SETUP_H
