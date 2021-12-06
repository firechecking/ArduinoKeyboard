#include "Keyboard.h"
#include "USBKeyboard.h"

USBKeyboard usbKeyboard;
void setup()
{
    int i, j;
    Serial.begin(115200);
    while (!Serial)
    {
        ;
    }
}

void loop()
{
    usbKeyboard.scanPinValues();
    usbKeyboard.serialDebug();
    usbKeyboard.keyAction();
    delay(100);
}