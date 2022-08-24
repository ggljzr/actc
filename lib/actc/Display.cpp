#include "Display.hpp"
#include <LiquidCrystal_I2C.h>

namespace actc
{
    Display::Display(uint8_t addr, uint8_t cols, uint8_t rows) : lcd(addr, cols, rows){};

    void Display::begin()
    {
        lcd.init();
        lcd.backlight();
        lcd.print("TC :");

        lcd.setCursor(0, 1);
        lcd.print("ABS:");

        lcd.setCursor(0, 2);
        lcd.print("Best:");
        lcd.setCursor(0, 3);
        lcd.print("Last:");

        setTC(0);
        setABS(0);

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
            snprintf(buffer, 8, "%03d", value);
            lcd.print(buffer);
        }
    }

    void Display::setTC(uint8_t value)
    {
        printValueOrOff((uint32_t)value, 0, 5);
    }

    void Display::setABS(uint8_t value)
    {
        printValueOrOff((uint32_t)value, 1, 5);
    }

    void Display::setBestLap(uint32_t value)
    {
        lcd.setCursor(8, 2);
        lcd.print("--:--:--.---");
    }

    void Display::setLastLap(uint32_t value)
    {
        lcd.setCursor(8, 3);
        lcd.print("--:--:--.---");
    }
}