#include "DirectionEncoder.hpp"

namespace actc
{
    DirectionEncoder::DirectionEncoder(uint8_t clkPin, uint8_t dtPin, int32_t encReadDivision)
        : enc(clkPin, dtPin), encReadDivision(encReadDivision){};

    int32_t DirectionEncoder::read()
    {
        int32_t newPosition = enc.read() / encReadDivision;
        int32_t diff = oldPosition - newPosition;
        oldPosition = newPosition;

        return diff;
    }
}