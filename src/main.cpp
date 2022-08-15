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

void displayTc(uint8_t value)
{
  lcd.setCursor(4, 0);

  if (value == 0)
    lcd.print("off");
  else
  {
    char buffer[8];
    snprintf(buffer, 8, "%03d", value);
    lcd.print(buffer);
  }
}

/// Keeps given key pressed for given delay, then releases it.
void shortPress(uint8_t key, unsigned long releaseDelay = 50)
{
  Keyboard.press(key);
  delay(releaseDelay);
  Keyboard.release(key);
}

void setup()
{
  Serial.begin(9600);
  Keyboard.begin();

  lcd.init();
  lcd.backlight();
  lcd.print("TC:");
  displayTc(1);
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