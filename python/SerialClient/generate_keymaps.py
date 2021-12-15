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

keycode_maps_arduino = {"SCLN": ';', "QUOT": "\'", "BSLS": "\\", "COMM": ",", "DOT": ".", "SLSH": "/", "LCTL": "KEY_LEFT_CTRL",
                        "LALT": "KEY_LEFT_ALT",
                        "LGUI": "KEY_LEFT_GUI",
                        "LSFT": "KEY_RIGHT_SHIFT",
                        "BSPC": "KEY_BACKSPACE", "SPC": " ", 'MINS': "-", 'EQL': '=', "ENT": "\n", "EXLM": '!', 'at': '@',
                        'DLR': '$', 'PERC': '%', 'PGUP': 'KEY_PAGE_UP', 'LPRN': '(', 'RGHT': 'KEY_RIGHT_ARROW', 'LEFT': 'KEY_LEFT_ARROW',
                        'UP': 'KEY_UP_ARROW', 'DOWN': 'KEY_DOWN_ARROW', 'RPRN': ')', 'PGDN': 'KEY_PAGE_DOWN',
                        'LBRC': '[',
                        'RBRC': ']',
                        'HASH': '#',
                        'LCBR': '{', 'RCBR': '}',
                        'CIRC': '^', 'AMPR': '&', 'ASTR': '*', 'PLUS': '+', "INS": 'KEY_INSERT', "TRNS": "LOWER", 'VOLU': 'CUSTOM_EMPTY',
                        'VOLD': 'CUSTOM_EMPTY',
                        'PSCR': 'CUSTOM_EMPTY', 'SLCK': 'CUSTOM_EMPTY', 'PAUS': 'CUSTOM_EMPTY', 'AT': '@', 'TAB': 'KEY_TAB', 'ESC': 'KEY_ESC'}


def convert_keycode(key, code_maps):
    key = key.split("_")[-1]
    if key.startswith('MO') or key.startswith('TG') or key.startswith('TO'):
        key = '{}_{}'.format(key.split('(')[0], key.split('(')[-1].split(')')[0])
    elif not key in code_maps:
        return key.lower()

    key = code_maps.get(key, key)
    return key


def apply_cpp_template(default_layer_index, keymaps):
    template_cpp = """#ifndef __KEYMAPS__
#define __KEYMAPS__
#include <string.h>

#define default_layer_idx $default_idx
#define NUM_LAYERS $num_layers
#define CUSTOM_EMPTY 0x00
#define MO 0xE0
#define TG 0xE1
#define TO 0xE2
#define LOWER 0xE3

uint8_t keymaps[NUM_LAYERS * $num_keys] = {$keymaps_str};
uint8_t keymapsValue[NUM_LAYERS * $num_keys] = {$keymaps_value};
int layer_state[NUM_LAYERS] = {$layer_state};
#endif"""
    keymaps = keymaps[:2]
    template_cpp = template_cpp.replace('$num_layers', str(len(keymaps))).replace('$default_idx', str(default_layer_index))
    keymap_list = []
    for i, keymap in enumerate(keymaps):
        for j in range(len(keymap)):
            keymap_list.append(keymap[j])
    keymaps_str = ""
    keymaps_value = []
    for char in keymap_list:
        for fn in ('MO', 'TG', 'TO', "LOWER"):
            if char.find(fn) > -1:
                keymaps_str += fn
                if fn == "LOWER":
                    keymaps_value.append(0)
                else:
                    keymaps_value.append(int(char.split('_')[-1]))
                break
        if char.find(fn) < 0:
            if len(char) <= 1:
                keymaps_str += f"'{char}'"
            else:
                keymaps_str += f"{char}"
            keymaps_value.append(0)
        keymaps_str += ','
    keymaps_str = keymaps_str.replace("'\\'", '0x5c').replace('\n', '\\n').replace("'''", "0x27")
    layer_state = [1 if i == default_layer_index else 0 for i, _ in enumerate(keymaps)]
    layer_state = json.dumps(layer_state)
    key_state = [-1 for _ in range(10)]
    key_state = json.dumps(key_state)
    keymaps_value = json.dumps(keymaps_value)

    template_cpp = template_cpp.replace('$keymaps_str', keymaps_str) \
        .replace('$num_keys', str(len(keymap))) \
        .replace('$layer_state', layer_state[1:len(layer_state) - 1]) \
        .replace('$key_state', key_state[1:len(key_state) - 1]) \
        .replace('$keymaps_value', keymaps_value[1:len(keymaps_value) - 1])
    with open('../../KeyMaps.h', 'w') as f:
        f.write(template_cpp)


def get_keymaps(code_maps):
    keymaps = []
    for layer in layers:
        keymap = {}
        for i, key in enumerate(layer):
            keymap[keys_layout[i]] = convert_keycode(key, code_maps)
        keymaps.append(keymap)
    return keymaps


if __name__ == "__main__":
    fn = "keymap_configs/keymaps_custom.json"
    default_layer_index = 0
    layers = json.load(open(fn, 'r'))['layers']
    keymaps = get_keymaps(keycode_maps)
    s = template.format(default_layer_idx=default_layer_index, keymaps=str(keymaps))
    with open('KeyMaps.py', 'w') as f:
        f.write(s)
    keymaps = get_keymaps(keycode_maps_arduino)
    apply_cpp_template(default_layer_index, keymaps)
