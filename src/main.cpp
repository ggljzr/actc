#include <Arduino.h>
#include <Keyboard.h>
#include <Encoder.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino_FreeRTOS.h>

#include "Command.hpp"
#include "Commands.hpp"

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
  vTaskDelay(releaseDelay / portTICK_PERIOD_MS);
  Keyboard.release(key);
}

void encoderTask(void *pvParameters)
{
  for (;;)
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
}

void serialTask(void *pvParameters)
{
  actc::Command command;

  for (;;)
  {
    // wait for at least commandLength bytes
    if (Serial.available() >= 5)
    {
      // construct a command from available bytes
      // this call should consume commandLength bytes
      // from the Serial stream

      command.readFromStream(&Serial);

      // process the command according to command ID
      switch (command.getCommandId())
      {
      case actc::commands::TC:
        displayTc(command.getUIntData());
        break;
      default:
        break;
      }
    }

    vTaskDelay(10);
  }
}

void setup()
{
  Serial.begin(9600);
  Keyboard.begin();

  lcd.init();
  lcd.backlight();
  lcd.print("TC:");
  displayTc(1);

  xTaskCreate(encoderTask, "encoderTask", 128, NULL, 1, NULL);
  xTaskCreate(serialTask, "serialTask", 128, NULL, 2, NULL);
}

void loop()
{
}