#include "Command.hpp"

namespace actc
{
    Command::Command(){};

    void Command::readFromBuffer(uint8_t *buffer, size_t offset)
    {
        for (size_t i = 0; i < commandLength; i++)
            this->buffer[i] = buffer[i];
    }

    void Command::readFromStream(Stream *stream)
    {
        stream->readBytes(buffer, commandLength);
    }

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
        return (
            (uint32_t)data[0] << 24 |
            (uint32_t)data[1] << 16 |
            (uint32_t)data[2] << 8 |
            data[3]);
    }
}