# -*- coding: utf-8 -*-
# @Time    : 2021/3/16 12:14 AM
# @Author  : Patrick(贤人)
# @Email   : patrick.zxx@alibaba-inc.com
# @File    : KeyStateDecoder.py
# @Software: Experiments
# @Description: KeyStateDecoder
# see https://config.qmk.fm/ for keymaps configuration

import time
from python.SerialClient.KeyMaps import keymaps, layer_state, default_layer_idx
import pyautogui


class KeyStateDecoder():
    def __init__(self):
        self.keyStates = [] # store recent 10 keyState values
        self.preKeyActions = {}
        self.history = {"keycodes": {}, "sendCount": {}}
        self.quantums = []  # store activated quantum keys

    def addNewState(self, keyState):
        self.keyStates.append(keyState)
        while len(self.keyStates) > 10:
            self.keyStates.pop(0)

    def decodeKeyAction(self):
        """get activated layer keycode
        
        Returns:
            return the pressed key dict conditioned on activated layer
            dict key is keycode, value is string key with timestamp

            example: {42: 'a_1638784937384636'}
        """
        # get current activated layer, layer_state can be changed by parseQuantum function
        idx = len(layer_state) - 1
        while idx >= 0 and layer_state[idx] < 1:
            idx -= 1
        if layer_state[idx] < 1: idx = default_layer_idx
        keymap = keymaps[idx]

        # get pressed keycode list
        cur_state = self.keyStates[-1]
        keycodes = {}
        for i, v in enumerate(cur_state):
            if i in keymap:
                if v == 'press':  # 记录第一次按下时间
                    if i in self.history['keycodes']:
                        keycodes[i] = self.history['keycodes'][i]
                    else:
                        keycodes[i] = "{}_{}".format(
                            keymap[i], int(round(time.time() * 1000000)))
        self.history['keycodes'] = keycodes.copy()
        return keycodes

    def parseQuantum(self, keycodes):
        """change activate layer

        by change layer_state, will affect next time's decodeKeyAction() result
        
        Args:
            keycodes: dict from decodeKeyAction()
        """
        # delete keys already released in quantum
        for i, keycode in enumerate(self.quantums):
            if keycode not in keycodes.values():
                if keycode.startswith('MO'):
                    layer_state[int(keycode.split('_')[1])] = 0
                self.quantums[i] = ""
        self.quantums = [
            keycode for keycode in self.quantums if len(keycode) > 0
        ]

        # delete keys in not changed in keycodes
        for idx, keycode in keycodes.items():
            if keycode in self.quantums:
                if keycode.startswith("MO"): continue
                keycodes[idx] = ""
        keycodes = {k: v for k, v in keycodes.items() if len(v) > 0}

        for idx, key in keycodes.items():
            if key.startswith('MO'):  # 临时切换layer
                layer_state[int(key.split('_')[1])] = 1
                if key not in self.quantums:
                    self.quantums.append(key)
            if key.startswith('TG'):  # 切换layer开关
                layer_state[int(key.split('_')[1])] = 1 - layer_state[int(
                    key.split('_')[1])]
                if key not in self.quantums:
                    self.quantums.append(key)
            if key.startswith('TO'):  # 打开layer
                layer_set = int(key.split('_')[1])
                for i in range(layer_set, len(layer_state)):
                    layer_state[i] = 0
                layer_state[layer_set] = 1
                if key not in self.quantums:
                    self.quantums.append(key)
            if key.startswith("LOWER"):  # 下降一个layer
                idx = len(layer_state) - 1
                while idx > 0 and layer_state[idx] < 1:
                    idx -= 1
                layer_state[idx] = 0
                if key not in self.quantums:
                    self.quantums.append(key)

    def simulateKeyAction(self):
        keyTime1 = 20 * 1000
        keyTime2 = 80 * 1000
        keyTime3 = 500 * 1000
        if len(self.keyStates) < 1: return
        # get keycode from activated keylayer
        keycodes = self.decodeKeyAction()
        self.parseQuantum(keycodes)
        # release key
        for idx, key in self.preKeyActions.items():
            if not idx in keycodes:
                if key.startswith("MO") or key.startswith(
                        "TG") or key.startswith("TO") or key.startswith(
                            'LOWER'):
                    continue
                if len(key) < 1: continue
                pyautogui.keyUp(key.split("_")[0], _pause=False)
                self.history['sendCount'][idx] = 0
        # press key
        for idx, key in keycodes.items():
            if len(key) < 1: continue
            if (idx in self.preKeyActions) and (key
                                                == self.preKeyActions[idx]):
                interval = int(round(time.time() * 1000000)) - int(
                    key.split("_")[-1])
                sendCount = self.history['sendCount'].get(idx, 0)
                if interval < keyTime1 and sendCount < 1:
                    sendCount += 1
                    self.history['sendCount'][idx] = sendCount
                elif interval > keyTime2 and (
                        sendCount < (interval - keyTime3) / keyTime2 + 1):
                    sendCount += 1
                    self.history['sendCount'][idx] = sendCount
                    if key.find("shift") > -1:
                        continue
                else:
                    continue
            if (len(key) > 0) and (not key in self.preKeyActions):
                if key.startswith("MO") or key.startswith(
                        "TG") or key.startswith("TO") or key.startswith(
                            'LOWER'):
                    continue
                if len(key) < 1: continue
                pyautogui.keyDown(key.split("_")[0], _pause=False)
        self.preKeyActions = keycodes


if __name__ == "__main__":
    for i in range(10):
        pyautogui.keyDown('a')
        time.sleep(1)
