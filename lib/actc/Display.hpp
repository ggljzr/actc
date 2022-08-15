#pragma once

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

namespace actc
{
    /// Class that wraps LiquidCrystal_I2C and adds methods for displaying AC specific values like TC.
    class Display
    {
    private:
        LiquidCrystal_I2C lcd;

    public:
        Display(uint8_t addr, uint8_t cols = 16, uint8_t rows = 2);

        void begin();

        /// Sets TC value to be displayed.
        void setTC(uint8_t value);
    };
}
