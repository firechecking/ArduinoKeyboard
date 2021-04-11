// #include "Keyboard.h"
#include "KB_USB_Keyboard.h"

#define NUM_INPUT 4
#define NUM_OUTPUT 11

const int inPins[] = {A0, A1, A2, A3};
const int outPins[] = {3, 4, 5, 6, 7, 8, 9, 10, 16, 14, 15};

int pinValues[NUM_OUTPUT][NUM_INPUT];
int pinState[NUM_OUTPUT][NUM_INPUT];
int pinUpdateMs[NUM_OUTPUT][NUM_INPUT];
int sendCount[NUM_OUTPUT][NUM_INPUT];

KB_USB_Keyboard::KB_USB_Keyboard()
{
}
void KB_USB_Keyboard::initPins()
{
    for (int i = 0; i < NUM_INPUT; i++)
    {
        pinMode(inPins[i], INPUT_PULLUP);
    }
    for (int i = 0; i < NUM_OUTPUT; i++)
    {
        pinMode(outPins[i], OUTPUT);
        for (int j = 0; j < NUM_INPUT; j++)
        {
            pinValues[i][j] = 0;
            pinUpdateMs[i][j] = millis();
            sendCount[i][j] = 0;
        }
    }
}