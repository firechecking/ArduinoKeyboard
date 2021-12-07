#include <Keyboard.h>
#include "USBKeyboard.h"
#include "KeyMaps.h"

#define KEYCODE(idx, i, j) keymaps[idx * NUM_INPUT * NUM_OUTPUT + i * NUM_INPUT + j]
#define KEYVALUE(idx, i, j) keymapsValue[idx * NUM_INPUT * NUM_OUTPUT + i * NUM_INPUT + j]
#define KEY_PRESSED(i, j) (pinState[i][j] == 0 && pinValues[i][j] > 2)
#define KEY_RELEASED(i, j) (pinState[i][j] == 1 && pinValues[i][j] < 1)

USBKeyboard::USBKeyboard(/* args */)
{
    Keyboard.begin();

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
            pinState[i][j] = 0;
        }
    }
}

USBKeyboard::~USBKeyboard()
{
    Keyboard.end();
}
void USBKeyboard::keyAction()
{
    // 根据真实按键名称，处理quantum状态
    parseQuantum();

    int idx = activatedLayerIdx();
    // 根据pinValues，获取真实按键名称
    for (int i = 0; i < NUM_OUTPUT; i++)
        for (int j = 0; j < NUM_INPUT; j++)
        {
            if (pinState[i][j] == 0 && pinValues[i][j] > 2)
            {
                // Serial.println("press: " + KEYCODE(idx, i, j)+48);
                pinState[i][j] = 1;
                Keyboard.press(KEYCODE(idx, i, j));
            }
            else if (pinState[i][j] == 1 && pinValues[i][j] < 1)
            {
                // Serial.println("release: " + KEYCODE(idx, i, j)+48);
                pinState[i][j] = 0;
                Keyboard.release(KEYCODE(idx, i, j));
            }
        }
}
void USBKeyboard::parseQuantum()
{
    int idx = activatedLayerIdx();
    for (int i = 0; i < NUM_OUTPUT; i++)
        for (int j = 0; j < NUM_INPUT; j++)
        {
            if (KEYCODE(idx, i, j) == MO)
            {
                if (KEY_PRESSED(i, j))
                {
                    pinState[i][j] = 1;
                    int tgt_idx = KEYVALUE(idx, i, j);
                    layer_state[tgt_idx] = 1;
                    Serial.println("press_MO");
                }
                else if (KEY_RELEASED(i, j))
                {
                    pinState[i][j] = 0;
                    int tgt_idx = KEYVALUE(idx, i, j);
                    layer_state[tgt_idx] = 0;
                    Serial.println("release_MO");
                }
            }
            if (KEYCODE(idx, i, j) == LOWER)
            {
                if (KEY_PRESSED(i, j))
                {
                    pinState[i][j] = 1;
                    layer_state[idx] = 0;
                    Serial.println("press_LOWER");
                }
                else if (KEY_RELEASED(i, j))
                {
                    pinState[i][j] = 0;
                    layer_state[idx] = 0;
                    Serial.println("release_LOWER");
                }
            }
            
            // if (KEY_PRESSED(i, j) && (TO_VALUE(KEYCODE(idx, i, j)) > -1))
            // {
            //     Serial.println("press_TO: " + KEYCODE(idx, i, j) + TO_VALUE(KEYCODE(idx, i, j)));
            // }
            // if (KEY_PRESSED(i, j) && (LOWER_VALUE(KEYCODE(idx, i, j)) > -1))
            // {
            //     Serial.println("press_LOWER: " + KEYCODE(idx, i, j) + LOWER_VALUE(KEYCODE(idx, i, j)));
            // }
        }
}
int USBKeyboard::activatedLayerIdx()
{
    // 获取当前激活的keymap layer index
    int idx = NUM_LAYERS - 1;
    while (idx >= 0 && layer_state[idx] == 0)
    {
        idx -= 1;
    }
    if (layer_state[idx] < 1)
    {
        idx = default_layer_idx;
    }
    return idx;
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
    Serial.println();
}
