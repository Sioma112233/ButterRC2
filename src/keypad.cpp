#include <Arduino.h>
#include "keypad.h"

const std::array<int, 3> COLS_GPIO = {15, 5, 4};
const std::array<int, 4> ROWS_GPIO = {0, 2, 13, 12};

Keypad::Keypad(uint32 msInterval) : msInterval_(msInterval) {
    pressedBtnsMapping_.fill(false);
    pressedBtnsHandlers_.fill(nullptr);

    for (int row_gpio : ROWS_GPIO) {
        pinMode(row_gpio, INPUT_PULLUP);
    }
    for (int col_gpio : COLS_GPIO) {
        pinMode(col_gpio, OUTPUT);
        digitalWrite(col_gpio, 1);
    }
}

void Keypad::iterIfReady() {
    // not ready yet
    auto now = millis();
    if (now - lastIterTime < 50)
        return;
    lastIterTime = now;

    for (int col = 0; col < COLS_GPIO.size(); col++) {
        auto col_gpio = COLS_GPIO[col];

        digitalWrite(col_gpio, 0);

        for (int row = 0; row < ROWS_GPIO.size(); row++) {
            auto row_gpio = ROWS_GPIO[row];

            int btnState = digitalRead(row_gpio);
            const int PRESSED = 0;
            const int UNPRESSED = 1;

            // isBtnPressed is more in a sense of 'wasBtnPressed' here (checking it's previous state)
            if (btnState == PRESSED && !isBtnPressed_(row, col)) {
                pressedBtnsMapping_[calcBtnIndex(row, col)] = true;
                triggerBtnPressed_(row, col);
            } else if (btnState == UNPRESSED && isBtnPressed_(row, col)) {
                pressedBtnsMapping_[calcBtnIndex(row, col)] = false;
            }
        }

        digitalWrite(col_gpio, 1);
    }
}

void Keypad::onBtnPressed(KeypadButtons btn, const std::function<void(KeypadButtons)>& callback) {
    pressedBtnsHandlers_[btn] = callback;
}

///
/// Private Methods
///

bool Keypad::isBtnPressed_(int row, int col) {
    return pressedBtnsMapping_[calcBtnIndex(row, col)];
}

void Keypad::triggerBtnPressed_(int row, int col) {
    using namespace std;

    int btnNumericVal = calcBtnIndex(row, col);
    const function<void(KeypadButtons)>& handler = pressedBtnsHandlers_[btnNumericVal];

    if (handler)
        handler((KeypadButtons)btnNumericVal);
}

