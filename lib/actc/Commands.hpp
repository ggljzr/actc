#pragma once

#include <Arduino.h>

namespace actc
{
    /// Namespace defining command IDs.
    namespace commands
    {
        constexpr uint8_t TC{0x01};
        constexpr uint8_t ABS{0x02};

        constexpr uint8_t lapCount{0x03};
        constexpr uint8_t bestLap{0x04};
        constexpr uint8_t lastLap{0x05};
    }
}