#include "KB_KeyApp.h"
KB_KeyApp::KB_KeyApp() {}
void KB_KeyApp::collectSignal() {}
void KB_KeyApp::reloadKeyLayer(int layer)
{
    currentLayer = layer;
    cout << "currentLayer:" << currentLayer;
    // TODO-加载keymap配置
}
void KB_KeyApp::parseKeyState() {}
void KB_KeyApp::pressKeys() {}
