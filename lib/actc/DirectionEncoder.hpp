#pragma once

#include <Arduino.h>
#include <Encoder.h>

namespace actc
{

    /**
     * Class wrapper for Encoder, implementing read() method
     * that simply returns direction, in which the encoder was rotated.
     */
    class DirectionEncoder
    {
    private:
        int32_t oldPosition{-999};
        Encoder enc;
        int32_t encReadDivision;

    public:
        /**
         * Named pins to match the encoder module I am using. Correct pins
         * are needed for correct rotation indication (clockwise/counterclockwise),
         * reversing the pins will reverse the rotation.
         *
         * Read value division can also be specified, to separate division from
         * Config.h from this library.
         */
        DirectionEncoder(uint8_t clkPin, uint8_t dtPin, int32_t encReadDivision = 1);

        /**
         * Method for reading direction. Returns difference in positions:
         * <0 if encoder was rotated counterclockwise
         *  0 if encoder was not rotated
         * >0 if encoder was rotated clockwise
         */
        int32_t read();
    };
}