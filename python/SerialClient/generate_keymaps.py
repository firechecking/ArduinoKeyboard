# -*- coding: utf-8 -*-
# @Time    : 2021/4/13 1:31 PM
# @Author  : Patrick(贤人)
# @Email   : patrick.zxx@alibaba-inc.com
# @File    : generate_keymaps.py
# @Software: ArduinoKeyboard
# @Description: generate_keymaps

import os
import json

template = """
default_layer_idx = {default_layer_idx}
keymaps = {keymaps}

assert default_layer_idx < len(keymaps), "default_layer must be smaller than the length of keymaps"
layer_state = [1 if i == default_layer_idx else 0 for i, _ in enumerate(keymaps)]
"""

keys_layout = [43, 39, 35, 31, 27, 19, 15, 11, 7, 3,
               42, 38, 34, 30, 26, 18, 14, 10, 6, 2,
               41, 37, 33, 29, 25, 21, 23, 17, 13, 9, 5, 1,
               40, 36, 32, 28, 24, 20, 22, 16, 12, 8, 4, 0, ]

keycode_maps = {"SCLN": ';', "QUOT": "'", "BSLS": "\\", "COMM": ",", "DOT": ".", "SLSH": "/", "LCTL": "ctrl", "LALT": "alt", "LGUI": "command",
                "LSFT": "shift",
                "BSPC": "backspace", "SPC": "space", 'MINS': "-", 'EQL': '=', "ENT": "enter", "EXLM": '!', 'at': '@',
                'DLR': '$', 'PERC': '%', 'PGUP': 'pageup', 'LPRN': '(', 'RGHT': 'right', 'RPRN': ')', 'PGDN': 'pagedown', 'LBRC': '[', 'RBRC': ']',
                'HASH': '#',
                'LCBR': '{', 'RCBR': '}',
                'CIRC': '^', 'AMPR': '&', 'ASTR': '*', 'PLUS': '+', "INS": 'insert', "TRNS": "LOWER", 'VOLU': 'volumeup', 'VOLD': 'volumedown',
                'PSCR': 'printscreen', 'SLCK': 'scrolllock', 'PAUS': 'pause', 'AT': '@'}


def convert_keycode(key):
    key = key.split("_")[-1]
    key = keycode_maps.get(key, key)
    if key.startswith('MO') or key.startswith('TG') or key.startswith('TO'):
        key = '{}_{}'.format(key.split('(')[0], key.split('(')[-1].split(')')[0])
    elif key.startswith('LOWER'):
        pass
    else:
        key = key.lower()
    return key


def apply_cpp_template(default_layer_index, keymaps):
    template_cpp = """#ifndef __KEYMAPS__
#define __KEYMAPS__
#include <string.h>

#define default_layer_idx $default_idx
#define NUM_LAYERS $num_layers

string keymaps[NUM_LAYERS];
    
$keymaps_str
#endif"""
    template_cpp = template_cpp.replace('$num_layers', str(len(keymaps))).replace('$default_idx', str(default_layer_index))
    keymaps_str = ''
    for i, keymap in enumerate(keymaps):
        keymap_list = []
        for j in range(len(keymap)):
            keymap_list.append(keymap[j])
        _s = json.dumps(keymap_list)
        _s = _s[1:len(_s) - 1]
        keymaps_str += 'keymaps[{}]={{{}}}\n'.format(i, _s)
    template_cpp = template_cpp.replace('$keymaps_str', keymaps_str)
    with open('../../KeyMaps.h', 'w') as f:
        f.write(template_cpp)


if __name__ == "__main__":
    fn = "keymap_configs/keymaps_custom.json"
    default_layer_index = 0
    layers = json.load(open(fn, 'r'))['layers']
    keymaps = []
    for layer in layers:
        keymap = {}
        for i, key in enumerate(layer):
            keymap[keys_layout[i]] = convert_keycode(key)
        keymaps.append(keymap)
    s = template.format(default_layer_idx=default_layer_index, keymaps=str(keymaps))
    with open('KeyMaps.py', 'w') as f:
        f.write(s)
    apply_cpp_template(default_layer_index, keymaps)
