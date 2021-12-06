#ifndef __KEYMAPS__
#define __KEYMAPS__
#include <string.h>

#define default_layer_idx 0
#define NUM_LAYERS 3

string keymaps[NUM_LAYERS];
    
keymaps[0]={"enter", "/", ";", "p", "=", ".", "l", "o", "-", ",", "k", "i", "MO_1", "m", "j", "u", "shift", "n", "h", "y", "tab", "'", "backspace", "\\", "space", "b", "g", "t", "command", "v", "f", "r", "alt", "c", "d", "e", "ctrl", "x", "s", "w", "esc", "z", "a", "q"}
keymaps[1]={"%", "}", ")", "backspace", "^", "{", "right", "&", "*", "#", "down", "up", "LOWER", "]", "left", "@", "shift", "[", "(", "!", "insert", "=", "backspace", "$", "space", ".", "-", "+", "command", "3", "6", "9", "0", "2", "5", "8", "ctrl", "1", "4", "7", "TG_2", "alt", "pagedown", "pageup"}
keymaps[2]={"pause", "f12", "f11", "f10", "scrolllock", "f3", "f6", "f9", "printscreen", "f2", "f5", "f8", "TO_0", "f1", "f4", "f7", "space", "no", "down", "up", "ctrl", "LOWER", "backspace", "LOWER", "shift", "reset", "pagedown", "pageup", "command", "no", "right", "end", "alt", "no", "down", "up", "volumedown", "volumeup", "left", "home", "no", "no", "del", "insert"}

#endif