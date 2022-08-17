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
    void shortPress(uint8_t key, unsigned long releaseDelay)
    {
        Keyboard.press(key);
        vTaskDelay(releaseDelay / portTICK_PERIOD_MS);
        Keyboard.release(key);
    }

    // TODO: needs refactoring (custom Encoder class)
    void encoderTask(void *pvParameters)
    {
        Keyboard.begin();

        Encoder tcEnc(pins::tcEncClkPin, pins::tcEncDtPin);
        long oldTcPosition = -999;

        Encoder absEnc(pins::absEncClkPin, pins::absEncDtPin);
        long oldAbsPosition = -999;

        for (;;)
        {
            long newTcPosition = tcEnc.read() / config::encReadDivision;
            long newAbsPosition = absEnc.read() / config::encReadDivision;

            if (newTcPosition != oldTcPosition)
            {
                long diff = oldTcPosition - newTcPosition;

                // counterclokwise rotation
                if (diff < 0)
                    shortPress(config::tcDown);
                // clockwise rotation
                else if (diff > 0)
                    shortPress(config::tcUp);

                oldTcPosition = newTcPosition;
            }

            if (newAbsPosition != oldAbsPosition)
            {
                long diff = oldAbsPosition - newAbsPosition;

                // counterclokwise rotation
                if (diff < 0)
                    shortPress(config::absDown);
                // clockwise rotation
                else if (diff > 0)
                    shortPress(config::absUp);

                oldAbsPosition = newAbsPosition;
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
                case actc::commands::ABS:
                    lcd.setABS((uint8_t)command.getUIntData());
                    break;
                default:
                    break;
                }
            }

            vTaskDelay(10);
        }
    }
}