# -*- coding: utf-8 -*-
# @Time    : 2021/4/11 7:36 PM
# @Author  : Patrick(贤人)
# @Email   : patrick.zxx@alibaba-inc.com
# @File    : KB_KeyApp.py
# @Software: ArduinoKeyboard
# @Description: KB_KeyApp

import random
from python.prototype import keyconfig


class KB_KeyApp():
    def initLayer(self, layer):
        self.c_layer = layer
        self.keyState = [0 for i in range(keyconfig.NUM_KEYS)]

    def collectSignal(self):
        for i in range(random.randint(3)):
            rnd_id = random.randint(keyconfig.NUM_KEYS)
            self.keyState[rnd_id] = 1 - self.keyState[rnd_id]
        return self.keyState

    def parseKeyState(self):
        pass

    def pressKeys(self):
        pass


if __name__ == "__main__":
    pass
