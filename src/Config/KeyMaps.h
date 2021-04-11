#ifndef __KEY_MAPS__
#define __KEY_MAPS__

// #include "Arduino.h"
#include "keyconfig.h"

class KeyMaps
{
private:
    uint8_t hardid_to_keyid(uint8_t hardId)
    {
        const uint8_t idMap[NUM_KEYS] = {43, 31, 19, 9,
                                         42, 30, 18, 8,
                                         41, 29, 17, 7,
                                         40, 28, 16, 6,
                                         39, 27, 15, 5,
                                         37, 25, 38, 26,
                                         36, 24, 14, 4,
                                         35, 23, 13, 3,
                                         34, 22, 12, 2,
                                         33, 21, 11, 1,
                                         32, 20, 10, 0};
        return idMap[hardId];
    }

public:
    uint8_t keymap_key_to_keycode(uint8_t layer, uint8_t hardId)
    {
        return keyConfig[(layer)][hardid_to_keyid(hardId)];
    }
};

void
#endif