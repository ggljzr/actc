#include "Tasks.hpp"

#include <Encoder.h>
#include <Keyboard.h>
#include <Arduino_FreeRTOS.h>

#include "Command.hpp"
#include "Commands.hpp"
#include "Display.hpp"

#include "Config.hpp"
#include "Pins.hpp"

namespace actcTasks
{
    void shortPress(uint8_t key, unsigned long releaseDelay = 50)
    {
        Keyboard.press(key);
        vTaskDelay(releaseDelay / portTICK_PERIOD_MS);
        Keyboard.release(key);
    }

    void encoderTask(void *pvParameters)
    {
        Keyboard.begin();

        Encoder tcEnc(pins::tcEncClkPin, pins::tcEncDtPin);
        long oldPosition = -999;

        for (;;)
        {
            long newPosition = tcEnc.read() / config::encReadDivision;
            if (newPosition != oldPosition)
            {
                long diff = oldPosition - newPosition;

                // counterclokwise rotation
                if (diff < 0)
                    shortPress(config::tcDown);
                // clockwise rotation
                else if (diff > 0)
                    shortPress(config::tcUp);

                oldPosition = newPosition;
            }
        }
    }

    void serialTask(void *pvParameters)
    {
        Serial.begin(config::serialBaudrate);

        actc::Display lcd(
            config::displayAddr,
            config::displayCols,
            config::displayRows);
        lcd.begin();

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
}