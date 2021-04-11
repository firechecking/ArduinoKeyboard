import os
import json

root = "src/Config/"
# fn = "./KeymapConfig/keyboardio_atreus_layout_mine.json"
fn = os.path.join(root, "KeymapConfig/keyboardio_atreus_layout_mine.json")

c_template = '''
#ifndef __KEY_CONFIG__
#define __KEY_CONFIG__

#include "keycode.h"

#define NUM_LAYERS {num_layers}
#define NUM_KEYS {num_keys}
const uint8_t keyConfig[NUM_LAYERS][NUM_KEYS] = {key_config};

#endif
'''
d_layers = json.load(open(fn, 'r'))
layers = d_layers['layers']
num_layers = len(layers)
num_keys = len(layers[0])

s = '{'
for layer in layers:
    s += '{'
    for k in layer:
        s += k + ','
    s = s[:-1] + '},\n'
s = s[:-2]

with open(os.path.join(root, 'keyconfig.h'), 'w') as f:
    f.write(
        c_template.format(num_layers=num_layers,
                          num_keys=num_keys,
                          key_config=s))
