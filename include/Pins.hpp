#pragma once

/**
 * @file Pins.hpp
 *
 * This file serves for pin definitions. These definition may vary for different boards,
 * so this file (used with appropriate compile flags) provides some portability.
 *
 */

#include <Arduino.h>

namespace pins
{
    /// CLK pin of TC encoder
    constexpr uint8_t tcEncClkPin{8};

    /// DT pin of TC encoder
    constexpr uint8_t tcEncDtPin{9};
}