#pragma once

#include <Arduino.h>

namespace actc
{
    constexpr size_t commandLength{5};

    constexpr uint8_t tcCommandId{0x01};

    /**
     * Class for interperting commands from the ACTC plugin.
     * The idead is that you create an instance of this class
     * and then load incoming commands with readFromStream method.
     *
     * When you load new command, you can use class methods to interpret it.
     */
    class Command
    {
    private:
        uint8_t buffer[commandLength];

    public:
        Command();

        /**
         *  Copies command data (commandLength bytes) from given buffer, starting on given offset.
         */
        void readFromBuffer(uint8_t *buffer, size_t offset = 0);

        /**
         * Reads command data (commandLength bytes) from given stream and stores it
         * in the internal buffer. You can then use Command methods
         * to read these data.
         */
        // void readFromStream(&Stream stream);

        uint8_t getCommandId() const;

        /// Returns raw command data from the buffer (without the command ID).
        uint8_t *getCommandData() const;

        /// Returns command data interpreted as an uint32_t.
        uint32_t getUIntData() const;
    };
}