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

        setTC(0);
    }

    void Display::setTC(uint8_t value)
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
}