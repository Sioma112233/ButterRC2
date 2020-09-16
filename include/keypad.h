#ifndef BUTTERRC2_KEYPAD_H
#define BUTTERRC2_KEYPAD_H

#include <functional>
#include <c_types.h>

constexpr int calcBtnIndex(int row, int col) { return 3 * row + col; }

enum KeypadButtons {
    hashtag = calcBtnIndex(0, 0),
    zero = calcBtnIndex(0, 1),
    star = calcBtnIndex(0, 2),
    nine = calcBtnIndex(1, 0),
    eight = calcBtnIndex(1, 1),
    seven = calcBtnIndex(1, 2),
    six = calcBtnIndex(2, 0),
    five = calcBtnIndex(2, 1),
    four = calcBtnIndex(2, 2),
    three = calcBtnIndex(3, 0),
    two = calcBtnIndex(3, 1),
    one = calcBtnIndex(3, 2)
};

class Keypad {
public:
    static const int TOTAL_BUTTONS = 12;

    explicit Keypad(uint32 msInterval);
    void iterIfReady(); // iterates the keypad if the time has come (based on interval)
    void onBtnPressed(KeypadButtons btn, const std::function<void(KeypadButtons)>& callback);


private:
    uint32 msInterval_;
    uint32 lastIterTime = 0;

    // key buttons mappings
    std::array<bool, TOTAL_BUTTONS> pressedBtnsMapping_;
    std::array<std::function<void(KeypadButtons)>, TOTAL_BUTTONS> pressedBtnsHandlers_;

    // methods
    bool isBtnPressed_(int row, int col);
    void triggerBtnPressed_(int row, int col);

};

#endif //BUTTERRC2_KEYPAD_H
