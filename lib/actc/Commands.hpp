#pragma once

#include <Arduino.h>

namespace actc
{
    /// Namespace defining command IDs.
    namespace commands
    {
        constexpr uint8_t TC{0x01};
        constexpr uint8_t ABS{0x02};
    }
}