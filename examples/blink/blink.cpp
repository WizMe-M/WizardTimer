#include <Arduino.h>
#include <Timer.h>

#define LED_PIN 13

Timer t;
byte state;

void blink() {
  state = !state;
  digitalWrite(LED_PIN, state);
}

void setup() {
  state = HIGH;
  pinMode(LED_PIN, OUTPUT); 
  t.init(1000, blink);
  t.start();
}

void loop() {
  t.update();
}