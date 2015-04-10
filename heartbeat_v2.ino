#include "math.h"
// UI actions
#define UI_OK 1
#define UI_SEND 2
#define UI_REC 3
#define UI_ERROR 4

// LEDs
int incomingLed = D0;
int sendingLed = D2;
int errorLed = D3;
int systemLed = D4;

// Buttons
int buttonPin = D1;
int buttonState = 2;

// configuable partner
String partnerId;

void setup() {
    // set LEDs
    pinMode(incomingLed, OUTPUT);
    pinMode(sendingLed, OUTPUT);
    pinMode(errorLed, OUTPUT);
    pinMode(systemLed, OUTPUT);

    // set Button
    pinMode(buttonPin, INPUT);

    // turn everything off
    digitalWrite(incomingLed, LOW);
    digitalWrite(sendingLed, LOW);
    digitalWrite(errorLed, LOW);
    digitalWrite(systemLed, LOW);

    // set functions and listeners
    String partner = '';



    ui(UI_OK);
}


void loop() {
}

// place to handle UI interactions
int ui(int action) {
    switch(action) {
        case UI_OK: {
            ack(systemLed, 3, 300);
        }

    }
}

// shorticut to blink the LED a couple times
int ack(int pin, int count, int freq) {
    int i = 0;
    for(i = 0; i < count; i++) {
        digitalWrite(pin, HIGH);
        delay(freq);
        digitalWrite(pin, LOW);
        delay(freq);
    }
}
