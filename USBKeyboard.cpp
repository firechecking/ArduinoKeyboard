#include <Keyboard.h>
#include "USBKeyboard.h"
#include "KeyMaps.h"

#define KEYCODE(k, i, j) keymaps[k * NUM_INPUT * NUM_OUTPUT + i * NUM_INPUT + j]

USBKeyboard::USBKeyboard(/* args */)
{
    Keyboard.begin();
    for (int i = 0; i < KEYBUFFER; i++)
        prePressedKeyIdxs[i] = 0xFF;
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
            pinState[i][j] = NUM_LAYERS + 10;
        }
    }
}

USBKeyboard::~USBKeyboard()
{
    Keyboard.end();
}
void USBKeyboard::keyAction()
{
    // 记录当前按下的按键
    decodeKeyAction(); // current pressed key index are keep in pressedKeyIdxs[]
    // 根据按键，处理quantum状态
    parseQuantum();
    // 释放按键
    for (uint8_t preKeyIdx : prePressedKeyIdxs)
        if (preKeyIdx < 0xFF)
        {
            if (keymaps[preKeyIdx] == MO || keymaps[preKeyIdx] == TG || keymaps[preKeyIdx] == TO || keymaps[preKeyIdx] == LOWER)
                continue;
            int i;
            for (i = 0; i < KEYBUFFER; i++)
                if (pressedKeyIdxs[i] == 0xFF || pressedKeyIdxs[i] == preKeyIdx)
                    break;
            if (pressedKeyIdxs[i] == 0xFF) // not contains in pressedKeyIdxs
                Keyboard.release(keymaps[preKeyIdx]);
        }
    // 新增按键
    for (uint8_t keyIdx : pressedKeyIdxs)
    {
        if (keyIdx == 0xFF)
            break;
        if (keymaps[keyIdx] == MO || keymaps[keyIdx] == TG || keymaps[keyIdx] == TO || keymaps[keyIdx] == LOWER)
            continue;
        int i;
        for (i = 0; i < KEYBUFFER; i++)
            if (prePressedKeyIdxs[i] == 0xFF || prePressedKeyIdxs[i] == keyIdx)
                break;
        if (prePressedKeyIdxs[i] == 0xFF) // not contains in prePressedKeyIdxs
            Keyboard.press(keymaps[keyIdx]);
    }
    // 保存按键记录
    for (int i = 0; i < KEYBUFFER; i++)
        prePressedKeyIdxs[i] = pressedKeyIdxs[i];
}
void USBKeyboard::parseQuantum()
{
    // 释放按键
    for (uint8_t preKeyIdx : prePressedKeyIdxs)
        if (preKeyIdx < 0xFF)
        {
            if (keymaps[preKeyIdx] == MO)
            {
                int i;
                for (i = 0; i < KEYBUFFER; i++)
                    if (pressedKeyIdxs[i] == 0xFF || pressedKeyIdxs[i] == preKeyIdx)
                        break;
                if (pressedKeyIdxs[i] == 0xFF) // not contains in pressedKeyIdxs
                    layer_state[keymapsValue[preKeyIdx]] = 0;
            }
        }
    // 新增按键
    for (uint8_t keyIdx : pressedKeyIdxs)
    {
        if (keyIdx == 0xFF)
            break;
        if (keymaps[keyIdx] == MO || keymaps[keyIdx] == TG || keymaps[keyIdx] == TO || keymaps[keyIdx] == LOWER)
        {
            int i;
            for (i = 0; i < KEYBUFFER; i++)
                if (prePressedKeyIdxs[i] == 0xFF || prePressedKeyIdxs[i] == keyIdx)
                    break;
            if (prePressedKeyIdxs[i] == 0xFF) // not contains in prePressedKeyIdxs
            {
                int layer_set = keymapsValue[keyIdx];
                if (keymaps[keyIdx] == MO)
                    layer_state[layer_set] = 1;
                else if (keymaps[keyIdx] == TG)
                    layer_state[layer_set] = 1 - layer_state[layer_set];
                else if (keymaps[keyIdx] == TO)
                {
                    for (int j = layer_set + 1; j < NUM_LAYERS; j++)
                        layer_state[j] = 0;
                    layer_state[layer_set] = 1;
                }
                else if (keymaps[keyIdx] == LOWER)
                {
                    int idx = activatedLayerIdx();
                    if (idx > 0)
                        layer_state[idx] = 0;
                }
            }
        }
    }
}
int USBKeyboard::activatedLayerIdx()
{
    // 获取当前激活的keymap layer index
    int idx = NUM_LAYERS - 1;
    while (idx > 0 && layer_state[idx] == 0)
    {
        idx -= 1;
    }
    if (layer_state[idx] < 1)
    {
        idx = default_layer_idx;
    }
    return idx;
}
void USBKeyboard::decodeKeyAction()
{
    /* get the real keycode.
    */
    int idx = activatedLayerIdx();
    int cnt = 0;

    for (int i = 0; i < NUM_OUTPUT; i++)
        for (int j = 0; j < NUM_INPUT; j++)
            if (pinValues[i][j] > 2)
            {
                if (pinState[i][j] > NUM_LAYERS)
                    pinState[i][j] = idx;
                pressedKeyIdxs[cnt] = pinState[i][j] * NUM_INPUT * NUM_OUTPUT + i * NUM_INPUT + j;
                cnt++;
            }
            else
                pinState[i][j] = NUM_LAYERS + 10;
    for (int i = cnt; i < KEYBUFFER; i++)
        pressedKeyIdxs[i] = 0xFF;
#ifdef __DEBUG__
    for (int i = 0; i < KEYBUFFER; i++)
        if (pressedKeyIdxs[i] < 0xFF)
        {
            Serial.print(pressedKeyIdxs[i]);
            Serial.println();
        }
#endif
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
