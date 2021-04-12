# -*- coding: utf-8 -*-
# @Time    : 2021/3/16 12:14 AM
# @Author  : Patrick(贤人)
# @Email   : patrick.zxx@alibaba-inc.com
# @File    : KeyStateDecoder.py
# @Software: Experiments
# @Description: KeyStateDecoder

import time
from python.SerialClient.KeyMaps import keymaps, layer_state, default_layer_idx
import pyautogui


class KeyStateDecoder():
    def __init__(self):
        self.keyStates = []
        self.preKeyActions = {}
        self.history = {"keycodes": {}}
        self.quantums = []

    def addNewState(self, keyState):
        self.keyStates.append(keyState)
        while len(self.keyStates) > 10: self.keyStates.pop(0)

    def decodeKeyAction(self):
        # get current activated layer
        idx = len(layer_state) - 1
        while idx >= 0 and layer_state[idx] < 1:
            idx -= 1
        if layer_state[idx] < 1: idx = default_layer_idx
        keymap = keymaps[idx]

        # get pressed keycode list
        cur_state = self.keyStates[-1]
        keycodes = {"example": "keycode_timestamp"}
        for i, v in enumerate(cur_state):
            if i in keymap:
                if v == 'press':
                    if i in self.history['keycodes']:
                        keycodes[i] = self.history['keycodes'][i]
                    else:
                        keycodes[i] = "{}_{}".format(keymap[i], int(round(time.time() * 1000000)))
        self.history['keycodes'] = keycodes
        return keycodes

    def parseQuantum(self, keycodes):
        # delete keys already released in quantum
        for i, keycode in enumerate(self.quantums):
            if keycode not in keycodes.values():
                if keycode.startswith('MO'):
                    layer_state[int(keycode.split('_')[1])] = 0
                self.quantums[i] = ""
        self.quantums = [keycode for keycode in self.quantums if len(keycode) > 0]

        # delete keys in not changed in keycodes
        for idx, keycode in keycodes.items():
            if keycode in self.quantums:
                if keycode.startswith("MO"): continue
                keycodes[idx] = ""
        keycodes = {k: v for k, v in keycodes.items() if len(v) > 0}

        for idx, key in keycodes.items():
            if key.startswith('MO'):
                layer_state[int(key.split('_')[1])] = 1
                self.quantums.append(key)
            if key.startswith('TG'):
                layer_state[int(key.split('_')[1])] = 1 - layer_state[int(key.split('_')[1])]
                self.quantums.append(key)
            if key.startswith('TO'):
                layer_set = int(key.split('_')[1])
                for i in range(layer_set, len(layer_state)):
                    layer_state[i] = 0
                layer_state[layer_set] = 1
                self.quantums.append(key)
            if key.startswith("LOWER"):
                idx = len(layer_state) - 1
                while idx > 0 and layer_state[idx] < 1:
                    idx -= 1
                layer_state[idx] = 0
                self.quantums.append(key)

    def simulateKeyAction(self):
        if len(self.keyStates) < 1: return
        # get keycode from activated keylayer
        keycodes = self.decodeKeyAction()
        self.parseQuantum(keycodes)

        for idx, key in self.preKeyActions.items():
            if not idx in keycodes:
                if key.startswith("MO") or key.startswith("TG") or key.startswith("TO") or key.startswith('LOWER'): continue
                if len(key) < 1: continue
                if key.startswith('example'): continue
                pyautogui.keyUp(key.split("_")[0], _pause=False)
        for idx, key in keycodes.items():
            if (idx in self.preKeyActions) and (key == self.preKeyActions[idx]): continue
            if (len(key) > 0) and (not key in self.preKeyActions):
                if key.startswith("MO") or key.startswith("TG") or key.startswith("TO") or key.startswith('LOWER'): continue
                if len(key) < 1: continue
                if key.startswith('example'): continue
                pyautogui.keyDown(key.split("_")[0], _pause=False)
        self.preKeyActions = keycodes


if __name__ == "__main__":
    for i in range(10):
        pyautogui.keyDown('a')
        time.sleep(1)
