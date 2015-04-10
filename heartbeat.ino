#include "math.h"

int ledPin = D0;
int buttonPin = D1;
int buttonVal = 2;
// these are for displaying the heartbeat
int counter = 0;
int val = 0;
float radians = 0;
float PI = 3.1415926;
// top level params
bool isBeating = false;
bool isSending = false;
bool beatRecd = false;

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT);

    // local function, to show you're sending a heartbeat
    Spark.function("sendHeart", sendHeart);
    // function called to show heartbeat has been received
    Spark.function("recHeart", recHeart);
    // function to acknowledge heartbeat and stop 
    Spark.function("gotHeart", gotHeart);


    Spark.subscribe("gotHeart", gotHandler);
    Spark.subscribe("recHeart", recHandler);
   // Spark.subscribe("gotHeart", gotHeart);

    digitalWrite(ledPin, LOW);
}

void gotHandler(const char *event, const char *data) {
    gotHeart("remote");
}
void recHandler(const char *event, const char *data) {
    recHeart("remote");
}

void loop() {
    if (isBeating == true) {
        // breathe the LED
        if (counter > 360) {
            counter = 3;
        }
        // blink the LED to get attention
        if ((isSending == false) && (counter == 90)) {
            ack(2,50);
        }
        // convert to radians
        radians = counter * (PI / 180);
        val = (sin(radians) * 255);
        val = ceil(abs(val));
    
        analogWrite(ledPin, val);
        
        delay(5);

        counter++;
        // if beating is in the receiving mode, pushing the button will acknowledge that you got it
        if (isSending == false) {
            buttonVal = digitalRead(buttonPin); //read the state of the push-button
            if (buttonVal == LOW) { //if push-button pressed
                gotHeart("");
                ack(4, 250);
            }
        }
    } else if (beatRecd == true) {
        ack(4, 250);
        beatRecd = false;
    } else {
        // pushbutton to send
        buttonVal = digitalRead(buttonPin); //read the state of the push-button
        if (buttonVal == LOW) { //if push-button pressed
            ack(4, 100);
            sendHeart("local");
        } else {
            delay(500);
        }  
    }
}

int sendHeart(String origin) {
    isBeating = true;
    isSending = true;
    beatRecd = false;
    if (origin != "remote") {
        Spark.publish("recHeart");
    }
    return 1;
}

int recHeart(String origin) {
    if (isSending != true) {
        isBeating = true;
        isSending = false;
        beatRecd = false;
        ack(5, 50);
        return 1;
    }
    return -1;
}

// stop the hearbeat, let the other one know you've gotten it '
int gotHeart(String origin) {
    isBeating = false;
    isSending = false;
    beatRecd = true;
    if (origin != "remote") {
        Spark.publish("gotHeart");
    }
    return 1;
}

// shorticut to blink the LED a couple times
int ack(int count, int freq) {
    int i = 0;
    for(i = 0; i < count; i++) {
        digitalWrite(ledPin, HIGH);
        delay(freq);
        digitalWrite(ledPin, LOW);
        delay(freq);       
    }
}