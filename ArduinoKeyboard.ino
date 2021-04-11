#include "Keyboard.h"
#include "src/Application/KB_KeyApp.h"

const int keyTime1 = 20;
const int keyTime2 = 500;
const int keyTime3 = 80;
const int pinValueBuffer = 10;

KB_KeyApp keyApp;
void setup()
{
    keyApp.reloadKeyLayer(0);
}

void loop()
{
    keyApp.collectSignal();
    keyApp.parseKeyState();
    keyApp.pressKeys();
    delay(1);
}

// void loop() {
//   scanPinValues();
//   updateKeyTime();
//   delay(1);
// }
// void updateKeyTime() {
//   int i, j, interval;
//   String keys = "";
//   int needSend = 0;
//   for (i = 0; i < NUM_OUTPUT; i++) {
//     for (j = 0; j < NUM_INPUT; j++) {
//       if (pinValues[i][j] <= 5) {
//         pinUpdateMs[i][j] = millis();
//         sendCount[i][j] = 0;
//         keys += "0";
//       }
//       if (pinValues[i][j] > 5) {
//         interval = millis() - pinUpdateMs[i][j];
//         if ((keyTime1 < interval) and (sendCount[i][j] < 1)) {
//           needSend = 1;
//           keys += "1";
// //          Keyboard.println(NUM_INPUT * i + j);
// //          Keyboard.write("a");
//           Keyboard.press(KEY_LEFT_SHIFT);
//           Keyboard.press('a');
//           delay(5000);
//           Keyboard.releaseAll();
//           sendCount[i][j] += 1;
//         }
//         if ((interval > keyTime2) and (sendCount[i][j] < (interval - keyTime2) / keyTime3 + 1)) {
//           needSend = 1;
//           keys += "2";
// //          Keyboard.print(NUM_INPUT * i + j);
// //          Keyboard.println("-Long");
// //          Keyboard.print("-");
//           Keyboard.write('a');
//           sendCount[i][j] += 1;
//         }
//       }
//     }
//   }
// //  if (needSend == 1) ConvertKeyToHex(keys);
// }
// void scanPinValues() {
//   int i, j;
//   for (i = 0; i < NUM_OUTPUT; i++) {
//     digitalWrite(outPins[i], LOW);
//     delay(1);
//     for (j = 0; j < NUM_INPUT; j++) {
//       if (digitalRead(inPins[j]) == LOW) {
//         pinValues[i][j] += 1;
//         if (pinValues[i][j] > pinValueBuffer) pinValues[i][j] = pinValueBuffer;
//       }
//       else {
//         pinValues[i][j] -= 1;
//         if (pinValues[i][j] < 0) pinValues[i][j] = 0;
//       }
//     }
//     digitalWrite(outPins[i], HIGH);
//   }
// }
