#include <Arduino.h>
#include <Keyboard.h>
#include <Encoder.h>
#include <LiquidCrystal_I2C.h>

constexpr uint8_t tcUp = 'o';
constexpr uint8_t tcDown = 'p';

/**
 * Data from Encoder.read() method will be divided by this factor.
 * Seems like the encoder I am using needs divison by 2 to get exactly
 * one keypress witch each click.
 */
constexpr long encReadDivision = 2;

Encoder tcEnc(8, 9);
long oldPosition = -999;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(9600);
  Keyboard.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("Hello, world!");
}

/// Keeps given key pressed for given delay, then releases it.
void shortPress(uint8_t key, unsigned long releaseDelay = 50)
{
  Keyboard.press(key);
  delay(releaseDelay);
  Keyboard.release(key);
}

void loop()
{
  long newPosition = tcEnc.read() / encReadDivision;
  if (newPosition != oldPosition)
  {
    long diff = newPosition - oldPosition;

    if (diff < 0)
      shortPress(tcDown);
    else if (diff > 0)
      shortPress(tcUp);

    oldPosition = newPosition;
  }
}