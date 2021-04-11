# -*- coding: utf-8 -*-
# @Time    : 2021/4/11 7:35 PM
# @Author  : Patrick(贤人)
# @Email   : patrick.zxx@alibaba-inc.com
# @File    : main.py
# @Software: ArduinoKeyboard
# @Description: main

from python.prototype.KB_KeyApp import KB_KeyApp

if __name__ == "__main__":
    app = KB_KeyApp()
    app.initLayer(0)
    app.collectSignal()
    app.parseKeyState()
    app.pressKeys()
