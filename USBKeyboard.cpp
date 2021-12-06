#include "USBKeyboard.h"

USBKeyboard::USBKeyboard(/* args */)
{
    for (int i = 0; i < NUM_INPUT; i++)
    {
        pinMode(inPins[i], INPUT_PULLUP);
    }
    for (int i = 0; i < NUM_OUTPUT; i++)
    {
        pinMode(outPins[i], OUTPUT);
        for (int j = 0; j < NUM_INPUT; j++)
        {
            pinValues[i][j] = 0;
            pinUpdateMs[i][j] = millis();
            pinState[i][j] = 0;
            sendCount[i][j] = 0;
            preKeys += '0';
            defaultKeys += '0';
        }
    }
}

USBKeyboard::~USBKeyboard()
{
}
void USBKeyboard::keyAction()
{
    // 获取当前层keymap
    // 根据pinValues，获取真实按键名称
    // 根据真实按键名称，处理quantum状态
    // 释放已取消按键
    // 按下新增按键
}
void USBKeyboard::scanPinValues()
{
    int i, j;
    for (i = 0; i < NUM_OUTPUT; i++)
    {
        digitalWrite(outPins[i], LOW);
        delay(1);
        for (j = 0; j < NUM_INPUT; j++)
        {
            if (digitalRead(inPins[j]) == LOW)
            {
                pinValues[i][j] += 1;
                if (pinValues[i][j] > pinValueBuffer)
                    pinValues[i][j] = pinValueBuffer;
            }
            else
            {
                pinValues[i][j] -= 1;
                if (pinValues[i][j] < 0)
                    pinValues[i][j] = 0;
            }
        }
        digitalWrite(outPins[i], HIGH);
    }
}
void USBKeyboard::serialDebug()
{
    for (int i = 0; i < NUM_OUTPUT; i++)
        for (int j = 0; j < NUM_INPUT; j++)
            Serial.print(pinValues[i][j]);
    Serial.println();
}