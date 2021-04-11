#ifndef __KB_KEYAPP__
#define __KB_KEYAPP__
#include "../HAL/KB_USB_Keyboard.h"

class KB_KeyApp
{
private:
    int currentLayer;
    KB_USB_Keyboard usbDevice;
    void shortPress();
    void longPress();
    void combinePress();
public:
    KB_KeyApp();
    void collectSignal();
    void reloadKeyLayer(int layer);
    void parseKeyState();
    void pressKeys(); 
};
#endif