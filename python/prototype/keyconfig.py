# -*- coding: utf-8 -*-
# @Time    : 2021/4/11 7:39 PM
# @Author  : Patrick(贤人)
# @Email   : patrick.zxx@alibaba-inc.com
# @File    : keyconfig.py
# @Software: ArduinoKeyboard
# @Description: keyconfig

import os
import json

root = "src/Config/"
fn = os.path.join(root, "KeymapConfig/keyboardio_atreus_layout_mine.json")

d_layers = json.load(open(fn, 'r'))
layers = d_layers['layers']

NUM_LAYERS = len(layers)
NUM_KEYS = len(layers[0])
