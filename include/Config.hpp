#pragma once

#include <Arduino.h>

/**
 * @file Config.hpp
 * This file defines some basic configuration constants.
 */

namespace config
{
    /// Keypress sent when TC knob is rotated clockwise (increase TC)
    constexpr uint8_t tcUp{'p'};

    /// Keypress sent when TC knob is rotated counterclockwise (decrease TC)
    constexpr uint8_t tcDown{'o'};

    /**
     * Data from Encoder.read() method will be divided by this factor.
     * Seems like the encoder I am using needs divison by 2 to get exactly
     * one keypress witch each click.
     */
    constexpr long encReadDivision{2};

    /// I2C address of the LCD
    constexpr uint8_t displayAddr{0x27};
    constexpr uint8_t displayCols{16};
    constexpr uint8_t displayRows{2};
}