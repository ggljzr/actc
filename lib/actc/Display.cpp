#include "Display.hpp"
#include <LiquidCrystal_I2C.h>

namespace actc
{
    Display::Display(uint8_t addr, uint8_t cols, uint8_t rows) : lcd(addr, cols, rows){};

    void Display::begin()
    {
        lcd.init();
        lcd.backlight();
        lcd.print("TC:");

        lcd.setCursor(12, 0);
        lcd.print("ABS:");

        lcd.setCursor(0, 2);
        lcd.print("Best:");
        lcd.setCursor(0, 3);
        lcd.print("Last:");

        lcd.setCursor(0, 1);
        lcd.print("Laps:");

        setTC(0);
        setABS(0);

        setLaps(0);
        setBestLap(0);
        setLastLap(0);
    }

    void Display::printValueOrOff(uint8_t value, uint8_t cursorRow, uint8_t cursorCol)
    {
        lcd.setCursor(cursorCol, cursorRow);

        if (value == 0)
            lcd.print("off");
        else
        {
            char buffer[8];
            snprintf(buffer, 8, "%03u", value);
            lcd.print(buffer);
        }
    }

    void Display::printTime(uint32_t millis, uint8_t cursorRow, uint8_t cursorCol)
    {
        lcd.setCursor(cursorRow, cursorCol);
        char buffer[16];

        uint32_t hours = millis / (3600000UL);
        uint32_t minutes = (millis % (3600000UL)) / (60000UL);
        uint32_t seconds = (millis % (60000UL)) / (1000);
        uint32_t ms = millis % 1000;

        snprintf(buffer, 16, "%03lu:%02lu:%02lu.%03lu", hours, minutes, seconds, ms);
        lcd.print(buffer);
    }

    void Display::setTC(uint8_t value)
    {
        printValueOrOff((uint32_t)value, 0, 4);
    }

    void Display::setABS(uint8_t value)
    {
        printValueOrOff((uint32_t)value, 0, 17);
    }

    void Display::setBestLap(uint32_t value)
    {
        printTime(value, 7, 2);
    }

    void Display::setLastLap(uint32_t value)
    {
        printTime(value, 7, 3);
    }

    void Display::setLaps(uint32_t value)
    {
        lcd.setCursor(6, 1);
        char buffer[16];
        snprintf(buffer, 16, "%03lu", value);
        lcd.print(buffer);
    }
}