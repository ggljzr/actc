#include <Arduino.h>
#include <Keyboard.h>
#include <Encoder.h>
#include <math.h>

Encoder tcEnc(8, 9);
long oldPosition = -999;

void setup()
{
  Serial.begin(9600);
  Keyboard.begin();
}

void loop()
{
  long newPosition = tcEnc.read();
  if (newPosition != oldPosition)
  {
    long diff = newPosition - oldPosition;

    if (abs(diff) > 1)
    {
      if (diff < 0)
      {
        Keyboard.press('o');
        delay(50);
        Keyboard.release('o');
        // Serial.println("-");
      }
      else if (diff > 0)
      {
        Keyboard.press('p');
        delay(50);
        Keyboard.release('p');
        // Serial.println("+");
      }
      oldPosition = newPosition;
    }
  }
}