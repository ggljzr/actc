#include <Arduino.h>
#include <Keyboard.h>

constexpr int buttonPin = 12;

void setup()
{
  Serial.begin(9600);
  Keyboard.begin();

  pinMode(buttonPin, INPUT_PULLUP);
}

void loop()
{
  if (digitalRead(buttonPin) == LOW)
  {
    Serial.println("button...");
    Keyboard.write('o');
    delay(100);
  }
}