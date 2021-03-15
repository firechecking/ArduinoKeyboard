
#define NUM_INPUT 11
#define NUM_OUTPUT 4
const int keyTime1 = 20;
const int keyTime2 = 500;
const int keyTime3 = 80;
const int pinValueBuffer = 10;
const int outPins[] = {A0, A1, A2, A3};
const int inPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
//const int inPins[] = {PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12};

int pinValues[NUM_INPUT][NUM_INPUT];
int pinState[NUM_INPUT][NUM_INPUT];
int pinUpdateMs[NUM_INPUT][NUM_INPUT];
int sendCount[NUM_INPUT][NUM_INPUT];

void setup() {
  int i, j;
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.println("Start Virtual Keyboard");

  for (i = 0; i < NUM_INPUT; i++) {
    pinMode(inPins[i], INPUT_PULLUP);
  }
  for (i = 0; i < NUM_OUTPUT; i++) {
    pinMode(outPins[i], OUTPUT);
    for (j = 0; j < NUM_INPUT; j++) {
      pinValues[i][j] = 0;
      pinUpdateMs[i][j] = millis();
      sendCount[i][j] = 0;
    }
  }
}

void loop() {
  scanPinValues();
  updateKeyTime();
  delay(1);
}
void updateKeyTime() {
  int i, j, interval;
  for (i = 0; i < NUM_OUTPUT; i++) {
    for (j = 0; j < NUM_INPUT; j++) {
      if (pinValues[i][j] <= 5) {
        pinUpdateMs[i][j] = millis();
        sendCount[i][j] = 0;
      }
      if (pinValues[i][j] > 5) {
        interval = millis() - pinUpdateMs[i][j];
        if ((keyTime1 < interval) and (sendCount[i][j] < 1)) {
          Serial.println(NUM_INPUT * i + j + 1);
          //          Serial.print("9");
          sendCount[i][j] += 1;
        }
        if ((interval > keyTime2) and (sendCount[i][j] < (interval - keyTime2) / keyTime3 + 1)) {
          Serial.print(NUM_INPUT * i + j + 1);
          Serial.println("-Long");
          //          Serial.print("-");
          sendCount[i][j] += 1;
        }
      }
    }
  }
}
void scanPinValues() {
  int i, j;
  for (i = 0; i < NUM_OUTPUT; i++) {
    digitalWrite(outPins[i], LOW);
    delay(1);
    for (j = 0; j < NUM_INPUT; j++) {
      if (digitalRead(inPins[j]) == LOW) {
        pinValues[i][j] += 1;
        if (pinValues[i][j] > pinValueBuffer) pinValues[i][j] = pinValueBuffer;
      }
      else {
        pinValues[i][j] -= 1;
        if (pinValues[i][j] < 0) pinValues[i][j] = 0;
      }
    }
    digitalWrite(outPins[i], HIGH);
  }
}
