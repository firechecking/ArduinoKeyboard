#include "USBKeyboard.h"
USBKeyboard usbKeyboard;
void setup()
{
    Serial.begin(9600);
    // while (!Serial)
    // {
    //     ;
    // }
}

void loop()
{
    usbKeyboard.scanPinValues();
    // usbKeyboard.serialDebug();
    usbKeyboard.keyAction();
    delay(1);
}
