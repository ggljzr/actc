#include <Arduino.h>
#include <Keyboard.h>
#include <Encoder.h>
#include <Arduino_FreeRTOS.h>

#include "Command.hpp"
#include "Commands.hpp"

#include "Display.hpp"

/// Keypress sent when knob is rotated clockwise (increase TC)
constexpr uint8_t tcUp = 'p';

/// Keypress sent when knob is rotated counterclockwise (decrease TC)
constexpr uint8_t tcDown = 'o';

/**
 * Data from Encoder.read() method will be divided by this factor.
 * Seems like the encoder I am using needs divison by 2 to get exactly
 * one keypress witch each click.
 */
constexpr long encReadDivision = 2;
constexpr uint8_t tcEncClkPin = 8;
constexpr uint8_t tcEncDtPin = 9;

actc::Display lcd(0x27, 16, 2);

Encoder tcEnc(tcEncClkPin, tcEncDtPin);
long oldPosition = -999;

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
      long diff = oldPosition - newPosition;

      // counterclokwise rotation
      if (diff < 0)
        shortPress(tcDown);
      // clockwise rotation
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
        lcd.setTC((uint8_t)command.getUIntData());
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
  lcd.begin();

  xTaskCreate(encoderTask, "encoderTask", 128, NULL, 1, NULL);
  xTaskCreate(serialTask, "serialTask", 128, NULL, 2, NULL);
}

void loop()
{
}