# -*- coding: utf-8 -*-
# @Time    : 2021/3/15 3:18 AM
# @Author  : Patrick(贤人)
# @Email   : patrick.zxx@alibaba-inc.com
# @File    : serial_keyboard.py
# @Software: Experiments
# @Description: serial_keyboard

import serial
import time
from python.SerialClient.KeyStateDecoder import KeyStateDecoder


def decodeKeys(bytes_values):
    keystates = []
    state_dic = {0: "null", 1: "press", 2: "hold", 3: "error"}
    for i in range(len(bytes_values)):
        value = bytes_values[i]
        for j in range(4):
            state = value % 4
            value = value // 4
            keystates.append(state_dic[state])
    return keystates


if __name__ == "__main__":
    serialPath = '/dev/cu.usbmodem14201'
    ser = serial.Serial(serialPath, 115200)  # open serial port
    print(ser.name)  # check which port was really used

    keyStateDecoder = KeyStateDecoder()

    while True:
        # get byte data
        if ser.inWaiting() > 0:
            value = ser.read(11)
            # get each keyswitch state
            keyState = decodeKeys(value)
            # store current keyswitch state
            keyStateDecoder.addNewState(keyState)

        # do keyboard action
        keyStateDecoder.simulateKeyAction()
        time.sleep(0.01)
