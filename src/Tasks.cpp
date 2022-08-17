#include "Tasks.hpp"

#include <Keyboard.h>
#include <Arduino_FreeRTOS.h>

#include "Command.hpp"
#include "Commands.hpp"
#include "Display.hpp"
#include "DirectionEncoder.hpp"

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

    void encoderTask(void *pvParameters)
    {
        Keyboard.begin();

        actc::DirectionEncoder tcEnc(pins::tcEncClkPin, pins::tcEncDtPin, config::encReadDivision);
        actc::DirectionEncoder absEnc(pins::absEncClkPin, pins::absEncDtPin, config::encReadDivision);

        // just to set correct reading position
        // before starting the loop
        tcEnc.read();
        absEnc.read();

        for (;;)
        {
            int32_t tcDiff = tcEnc.read();
            if (tcDiff < 0)
                shortPress(config::tcDown);
            else if (tcDiff > 0)
                shortPress(config::tcUp);

            int32_t absDiff = absEnc.read();
            if (absDiff < 0)
                shortPress(config::absDown);
            else if (absDiff > 0)
                shortPress(config::absUp);
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