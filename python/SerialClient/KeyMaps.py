# -*- coding: utf-8 -*-
# @Time    : 2021/3/16 12:20 AM
# @Author  : Patrick(贤人)
# @Email   : patrick.zxx@alibaba-inc.com
# @File    : KeyMaps.py
# @Software: Experiments
# @Description: KeyMaps

from pyautogui import KEY_NAMES

default_layer_idx = 0
keymaps = []
keymaps.append({43: "q",    39: 'w',    35: 'e',    31: 'r',    27: 't',                            19: 'y',    15: 'u',    11: 'i',    7: 'o',     3: "p",
                42: "a",    38: "s",    34: "d",    30: "f",    26: "g",                            18: "h",    14: "j",    10: "k",    6: "l",     2: ";",
                41: "z",    37: "x",    33: "c",    29: "v",    25: "b",    21: "`",    23: "\\",   17: "n",    13: "m",    9: ",",     5: ".",     1: "/",
                40: "esc",      36: "tab",      32: "command",      28: "shift",    24: "backspace",    20: "ctrl",
                22: "alt",      16: "space",    12: "MO_1",         8: "-",         4: "'",             0: "enter"})

keymaps.append({43: "!",    39: '@',    35: 'up',   31: '$',    27: '%',                            19: 'pageup',   15: '7',    11: '8',    7: '9',     3: "backspace",
                42: "(",    38: "left", 34: "down", 30: "right",26: ")",                            18: "pagedown", 14: "4",    10: "5",    6: "6",     2: "\\",
                41: "[",    37: "]",    33: "#",    29: "{",    25: "}",    21: "^",    23: "&",    17: "*",        13: "1",    9: "2",     5: "3",     1: "+",
                40: "TG_2",     36: "insert",   32: "command",   28: "shift",       24: "backspace",    20: "ctrl",
                22: "alt",      16: "space",    12: "LOWER",        8: ".",         4: "0",             0: "="})

keymaps.append({43: "insert",   39: 'home',     35: 'up',       31: 'end',      27: 'pageup',                               19: 'up',   15: 'f7',    11: 'f8',    7: 'f9',     3: "f10",
                42: "delete",   38: "left",     34: "down",     30: "right",    26: "pagedown",                             18: "down", 14: "f4",    10: "f5",    6: "f6",     2: "f11",
                41: "",         37: "volumeup", 33: "volumemute", 29: "",       25: "",         21: "LOWER",    23: "LOWER",17: "",     13: "f1",    9: "f2",     5: "f3",     1: "f12",
                40: "",         36: "volumedown",32: "command",   28: "shift",       24: "backspace",    20: "ctrl",
                22: "alt",      16: "space",    12: "TO_0",        8: "printscreen",         4: "scrolllock",             0: "pause"})

assert default_layer_idx < len(keymaps), "default_layer must be smaller than the length of keymaps"
layer_state = [1 if i == default_layer_idx else 0 for i, _ in enumerate(keymaps)]

if __name__ == "__main__":
    print(layer_state)
