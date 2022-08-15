#include "Command.hpp"

namespace actc
{
    Command::Command(){};

    uint8_t Command::getCommandId() const
    {
        return buffer[0];
    }

    uint8_t *Command::getCommandData() const
    {
        return (uint8_t *)(buffer) + 1;
    }

    uint32_t Command::getUIntData() const
    {
        uint8_t *data = getCommandData();
        return (data[3] << 24 | data[2] << 16 | data[1] << 8 | data[0]);
    }
}