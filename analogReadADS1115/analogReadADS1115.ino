#include <IntervalTimer.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads1115;

const int numberOfPins = 4;
int ledpin = 13;
long timeIndex = 0;

IntervalTimer timer; // timer
IntervalTimer blinkTimer; // timer
int startTimerValue = 0;
String txtMsg = "";
char s;

void setup() {
  pinMode(ledpin, OUTPUT);

  //Initialize serial connection
  Serial.begin(115200);

  //Initialize ADS1115
  ads1115.begin();

  //Initialize the blinker interrupt
  blinkTimer.begin(blink_callback, 1000000);

  //Initialize the reader interrupt
  setSampleRate(8000);
}

//elapsedMicros time;

void loop() {

  while (Serial.available() > 0) {
    s = (char)Serial.read();
    if (s == '\n') {
      int samplerate = txtMsg.substring(7, 11).toInt();
      setSampleRate(samplerate);
      txtMsg = "";
    } else {
      txtMsg += s;
    }
  }
}

void setSampleRate(float sampleRate) {
  timer.end();
  for (int i = 0 ; i < 8 ; i++) {
    digitalWrite(ledpin, !digitalRead(13));
    delay(80);
  }
  int microsecondsDelay = 1 / sampleRate * 1000000;
  timer.begin(timer_callback, microsecondsDelay);
}

void timer_callback(void) {
  int values[numberOfPins];
  for (int i = 0 ; i < numberOfPins ; i ++) {
    values[i] = ads1115.readADC_SingleEnded(i);
  }

  Serial.print("T");
  Serial.print(timeIndex, HEX);

  for (int i = 0 ; i < numberOfPins ; i ++) {
    Serial.print(" ");
    Serial.print(values[i], HEX);
  }

  Serial.println();

  timeIndex++;
}

void blink_callback(void) {
  digitalWrite(ledpin, !digitalRead(13));
}
