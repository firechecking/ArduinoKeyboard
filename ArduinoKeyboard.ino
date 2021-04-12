
#define NUM_INPUT 4
#define NUM_OUTPUT 11

const int keyTime1 = 20;
const int keyTime2 = 500;
const int keyTime3 = 80;
const int pinValueBuffer = 5;
const int inPins[] = {A0, A1, A2, A3};
const int outPins[] = {3, 4, 5, 6, 7, 8, 9, 10, 16, 14, 15};
//const int inPins[] = {PD2, PD3, PD4, PD5, PD6, PD7, PD8, PD9, PD10, PD11, PD12};

int pinValues[NUM_OUTPUT][NUM_INPUT];
int pinState[NUM_OUTPUT][NUM_INPUT];
int pinUpdateMs[NUM_OUTPUT][NUM_INPUT];
int sendCount[NUM_OUTPUT][NUM_INPUT];
String preKeys = "";
String defaultKeys="";

void setup()
{
    int i, j;
    Serial.begin(115200);
    while (!Serial)
    {
        ;
    }
    for (i = 0; i < NUM_INPUT; i++)
    {
        pinMode(inPins[i], INPUT_PULLUP);
    }
    for (i = 0; i < NUM_OUTPUT; i++)
    {
        pinMode(outPins[i], OUTPUT);
        for (j = 0; j < NUM_INPUT; j++)
        {
            pinValues[i][j] = 0;
            pinUpdateMs[i][j] = millis();
            pinState[i][j] = 0;
            sendCount[i][j] = 0;
            preKeys += '0';
            defaultKeys += '0';
        }
    }
}

void loop()
{
    scanPinValues();
    //   updateKeyTime();
    updateKeyState();
    delay(1);
}

void updateKeyState()
{
    int needSend = 0;
    String keys = "";
    for (int i = 0; i < NUM_OUTPUT; i++)
    {
        for (int j = 0; j < NUM_INPUT; j++)
        {
            if (pinValues[i][j] >= 2)
            {
                keys += '1';
            }
            else if (pinValues[i][j] == 0)
            {
                keys += '0';
            }
            else
                keys += preKeys[i * NUM_INPUT + j];
        }
    }
    if (keys != preKeys){
      preKeys = keys;
//      Serial.println(keys);
      ConvertKeyToHex(keys);
    }
}

void scanPinValues()
{
    int i, j;
    for (i = 0; i < NUM_OUTPUT; i++)
    {
        digitalWrite(outPins[i], LOW);
        delay(1);
        for (j = 0; j < NUM_INPUT; j++)
        {
            if (digitalRead(inPins[j]) == LOW)
            {
                pinValues[i][j] += 1;
                if (pinValues[i][j] > pinValueBuffer)
                    pinValues[i][j] = pinValueBuffer;
            }
            else
            {
                pinValues[i][j] -= 1;
                if (pinValues[i][j] < 0)
                    pinValues[i][j] = 0;
            }
        }
        digitalWrite(outPins[i], HIGH);
    }
}
