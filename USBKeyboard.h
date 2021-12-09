#ifndef __USBKEYBOARD__
#define __USBKEYBOARD__
#define __DEBUG__
#include <Arduino.h>
#define NUM_INPUT 4
#define NUM_OUTPUT 11

#define KEYBUFFER 20
#define pinValueBuffer 5

class USBKeyboard
{
private:
    int pinValues[NUM_OUTPUT][NUM_INPUT]; // 按键持续计数，按下+1，没按下-1，与pinValueBuffer结合，用于去除噪音抖动
    int pinState[NUM_OUTPUT][NUM_INPUT];
    int pressedKeyIdxs[KEYBUFFER];
    int prePressedKeyIdxs[KEYBUFFER];

    const uint8_t inPins[NUM_INPUT] = {A0, A1, A2, A3};
    const uint8_t outPins[NUM_OUTPUT] = {3, 4, 5, 6, 7, 8, 9, 10, 16, 14, 15}; // （输出，输入）构成二维矩阵，输出pin依次输出LOW，然后在输入pin依次判断电压是否被拉低

    void decodeKeyAction();
    void parseQuantum();
    int activatedLayerIdx();

public:
    USBKeyboard(/* args */);
    ~USBKeyboard();
    void keyAction();
    void scanPinValues();
    void serialDebug();
};
#endif