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

        /// Helper method to print given value (or 'off' if value == 0) on given position
        void printValueOrOff(uint8_t value, uint8_t cursorRow, uint8_t cursorCol);

    public:
        Display(uint8_t addr, uint8_t cols = 16, uint8_t rows = 2);

        void begin();
        /// Sets TC value to be displayed.
        void setTC(uint8_t value);
        void setABS(uint8_t value);

        /// Sets best lap time (given in miliseconds).
        void setBestLap(uint32_t value);
        /// Sets last lap time (given in miliseconds).
        void setLastLap(uint32_t value);

        /// Display current number of laps.
        void setLaps(uint32_t value);
    };
}
